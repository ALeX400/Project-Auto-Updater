#include <amxmodx>
#include <easy_http>
#include <amxmisc>

#define UPDATE_JSON_URL "https://raw.githubusercontent.com/ALeX400/Project-Auto-Updater/refs/heads/main/Updates.json"
#define PLUGIN_FOLDER "addons/amxmodx/plugins/"
#define CONFIG_FILE "addons/amxmodx/configs/auto_updater.ini"

new const	PluginName[] = "Auto Updater",
			PluginVersion[] = "1.0",
			PluginAuthor[] = "@LeX";

public plugin_init()
{
	register_plugin(PluginName, PluginVersion, PluginAuthor);
	
	if(!file_exists(CONFIG_FILE))
		write_file(CONFIG_FILE, "", -1);

	set_task(10.0, "CheckForUpdates");
	
	new buffer[212]
	PrintVersionInfo(PluginVersion, buffer, charsmax(buffer));
	server_print("^n        THIS MESSAGE IS FOR TESTING IF THE PLUGIN HAS BEEN UPDATED.");
	server_print("^n        Plugin Version: %s", buffer);
	//server_print("^n        This is Plugin from Local Machine.^n");
	server_print("^n        This is Plugin from Github Repository.^n");
}

public CheckForUpdates()
{
	server_print("[ Auto Updater ] Checking for updates...^n");

	new EzHttpOptions:options = ezhttp_create_options();
	ezhttp_option_set_timeout(options, 10000);
	
	ezhttp_get(UPDATE_JSON_URL, "OnUpdateCheckComplete", options);
}

public OnUpdateCheckComplete(EzHttpRequest:request_id)
{
	if (!ezhttp_is_request_exists(request_id))
	{
		server_print("[ Auto Updater ] HTTP request failed.");
		return;
	}

	if (ezhttp_get_http_code(request_id) != 200)
	{
		server_print("[ Auto Updater ] Failed to fetch update JSON. HTTP Code: %d", ezhttp_get_http_code(request_id));
		return;
	}

	new EzJSON:json = ezhttp_parse_json_response(request_id, false);
	if (json == EzInvalid_JSON)
	{
		server_print("[ Auto Updater ] Invalid JSON format.");
		return;
	}

	CheckPluginsForUpdates(json);
	ezjson_free(json);
}

public CheckPluginsForUpdates(EzJSON:json)
{
	new file = fopen(CONFIG_FILE, "rt");
	if (!file)
	{
		server_print("[ Auto Updater ] Failed to open config file: %s", CONFIG_FILE);
		return;
	}

	new plugin_filename[64], plugin_name[64], plugin_version[32], plugin_author[64], plugin_status[32];
	new line[128];

	while (fgets(file, line, charsmax(line)))
	{
		trim(line);
		if (line[0] == ';' || line[0] == '^0')
			continue;

		if (!GetPluginInfo(line, plugin_filename, charsmax(plugin_filename), plugin_name, charsmax(plugin_name), plugin_version, charsmax(plugin_version), plugin_author, charsmax(plugin_author), plugin_status, charsmax(plugin_status)))
		{
			server_print("[ Auto Updater ] Failed to get info for plugin: '%s'", line);
			continue;
		}

		new json_plugin_key[64];
		new total_plugins = ezjson_object_get_count(json);
		
		new bool:isNotFound = false;

		for (new i = 0; i < total_plugins; i++)
		{
			ezjson_object_get_name(json, i, json_plugin_key, charsmax(json_plugin_key));
			new EzJSON:pluginData = ezjson_object_get_value(json, json_plugin_key);
			
			new json_amxx[64], json_version[32];
			if (ezjson_object_has_value(pluginData, "AMXX", EzJSONString))
			{
				ezjson_object_get_string(pluginData, "AMXX", json_amxx, charsmax(json_amxx));
				if (equal(json_amxx, plugin_filename))
				{
					ezjson_object_get_string(pluginData, "Version", json_version, charsmax(json_version));

					if (CompareVersions(plugin_version, json_version))
					{
						server_print("[ Auto Updater ] New update found for '%s': '%s' -> '%s'", plugin_name, plugin_version, json_version);
						UpdatePlugin(pluginData, plugin_filename);
					}
					break;
				} else isNotFound = true;
				
			}
			ezjson_free(pluginData);
		}
		
		if(isNotFound)
		{
			server_print("[ Auto Updater ] No update info found for '%s' (%s).", plugin_name, plugin_filename);
		}
	}
	

	fclose(file);
}

public UpdatePlugin(EzJSON:pluginData, const plugin_filename[])
{
	new EzJSON:downloadData = ezjson_object_get_value(pluginData, "Download");
	if (!ezjson_is_object(downloadData))
	{
		server_print("[ Auto Updater ] No 'Download' object found in JSON for %s.", plugin_filename);
		return;
	}

	new json_download_url[512];
	new version_code = NumConvertAMXXVersion();
	new version_key[8];
	format(version_key, charsmax(version_key), "%d", version_code);

	if (ezjson_object_has_value(downloadData, version_key, EzJSONString))
	{
		ezjson_object_get_string(downloadData, version_key, json_download_url, charsmax(json_download_url));
	}
	else
	{
		server_print("[ Auto Updater ] No download URL for AMXX version %s in JSON.", AMXX_VERSION_STR);
		return;
	}

	if (!BackupOldPlugin(PLUGIN_FOLDER, plugin_filename))
	{
		server_print("[ Auto Updater ] The Download process has been canceled.");
		return;
	}
	server_print("[ Auto Updater ] Downloading update for '%s': ...", plugin_filename);
	DownloadNewVersion(json_download_url);
}

public DownloadNewVersion(const json_download_url[])
{
	new EzHttpOptions:options = ezhttp_create_options();
	ezhttp_option_set_timeout(options, 5000);
	
	ezhttp_get(json_download_url, "OnPluginDownloadComplete", options);
}

public OnPluginDownloadComplete(EzHttpRequest:request_id)
{
	if (!ezhttp_is_request_exists(request_id))
	{
		server_print("[ Auto Updater ] Download request failed.");
		return;
	}

	if (ezhttp_get_http_code(request_id) != 200)
	{
		server_print("[ Auto Updater ] Failed to download plugin. HTTP Code: %d", ezhttp_get_http_code(request_id));
		return;
	}

	new save_path[512], plugin_name[128];

	ezhttp_get_url(request_id, save_path, charsmax(save_path));
	GetFileNameFromURL(save_path, plugin_name, charsmax(plugin_name));

	format(save_path, charsmax(save_path), "%s%s", PLUGIN_FOLDER, plugin_name);
	ezhttp_save_data_to_file(request_id, save_path);

	server_print("[ Auto Updater ] Plugin downloaded successfully: '%s'", save_path);
}

public GetFileNameFromURL(const url[], output[], maxlen)
{
	new len = strlen(url);
	for (new i = len - 1; i >= 0; i--)
	{
		if (url[i] == '/')
		{
			copy(output, maxlen, url[i + 1]);
			return;
		}
	}
	copy(output, maxlen, url);
}

public bool:BackupOldPlugin(const folder[], const filename[])
{
	new file_path[512];
	format(file_path, charsmax(file_path), "%s%s", folder, filename);

	new BcDir[512];
	format(BcDir, charsmax(BcDir), "%sBackups/", folder);
	
	if(!dir_exists(BcDir))
		mkdir(BcDir);

	if (!file_exists(file_path))
	{
		server_print("[ Auto Updater ] File not found: %s", file_path);
		return false;
	}
	
	new pluginVersion[128]
	GetPluginInfo(filename, _, _, _, _, pluginVersion, charsmax(pluginVersion))

	// Some Special characters is not allowed in file names, so we can remove all of them.
	RemoveSpecialCharacters(pluginVersion, charsmax(pluginVersion));

	new name[128], ext[128];
	strtok2(filename, name, charsmax(name), ext, charsmax(ext), '.');

	new new_backup[128]
	format(new_backup, charsmax(new_backup), "%s%s_%s.%s", BcDir, name, pluginVersion, ext);

	if(rename_file(file_path, new_backup, 1))
	{
		server_print("[ Auto Updater ] Backup created: %s", new_backup);
		return true;
	}
	else
	{
		server_print("[ Auto Updater ] Failed to create backup for %s", filename);
		return false;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//                                      S T O C K S                                     //
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Converts a AmxModX version string to a numeric value.
 *
 * @param version_str  The version string to convert.
 *
 * @return             The numeric value of the version string.
 */
public NumConvertAMXXVersion()
{
	new major, minor, patch;
	new pos1, pos2;

	pos1 = strfind(AMXX_VERSION_STR, ".");
	pos2 = strfind(AMXX_VERSION_STR, ".", _, pos1 + 1);

	major = str_to_num(AMXX_VERSION_STR[0]);
	minor = str_to_num(AMXX_VERSION_STR[pos1 + 1]);
	patch = str_to_num(AMXX_VERSION_STR[pos2 + 1]);

	return major * 100 + minor * 10 + patch;
}

/**
 * Retrieves information about a plugin by its filename.
 *
 * @param filename         The filename of the plugin to search for.
 * @param plugin_filename  Buffer to copy the plugin filename to.
 * @param len1             Maximum buffer size for the filename.
 * @param plugin_name      Buffer to copy the plugin name to.
 * @param len2             Maximum buffer size for the name.
 * @param plugin_version   Buffer to copy the plugin version to.
 * @param len3             Maximum buffer size for the version.
 * @param plugin_author    Buffer to copy the plugin author to.
 * @param len4             Maximum buffer size for the author.
 * @param plugin_status    Buffer to copy the plugin status flags to.
 * @param len5             Maximum buffer size for the status.
 *
 * @return                 True if the plugin was found and information retrieved, false otherwise.
 */
stock bool:GetPluginInfo(const filename[], plugin_filename[] = "", len1 = 0, plugin_name[] = "", len2 = 0, plugin_version[] = "", len3 = 0, plugin_author[] = "", len4 = 0, plugin_status[] = "", len5 = 0)
{
	new plugin_id = find_plugin_byfile(filename);

	if (plugin_id == -1)
		return false;

	if (get_plugin(plugin_id, plugin_filename, len1, plugin_name, len2, plugin_version, len3, plugin_author, len4, plugin_status, len5) == -1)
		return false;

	return true;
}

/**
 * Removes all special characters from the input buffer, except spaces and tabs.
 *
 * @param input     The input buffer to process.
 * @param maxlen    The maximum length of the output buffer.
 */
stock RemoveSpecialCharacters(input[], maxlen)
{
	new j = 0;
	for (new i = 0; input[i] != '^0' && j < maxlen - 1; i++)
		if ((input[i] >= '0' && input[i] <= '9') || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z' || input[i] == '.' || input[i] == '-' || input[i] == '_'))
			input[j++] = input[i];
	input[j] = '^0';
}

/**
 * Prints the version information to the server console.
 * ( Using for debugging purposes )
 *
 * @param version  The version string to print.
 * @param output   The buffer to copy the version information to.
 * @param len      The maximum buffer size.
 *
 * @note           If output is provided, the information will be copied to the output buffer.
 *                 If is not provided, the information will be printed to the server console.
 *                 Example:
 *
 *                 PrintVersionInfo("1.0.0", buffer, charsmax(buffer));
 *                 Buffer Output: "DEBUG >> For: '1.0.0' >> Major: 1, Minor: 0, Build: 0"
 *
 *                 PrintVersionInfo("1.2.3.456a");
 *                 Server Print Output: "DEBUG >> For: '1.2.3.456a' >> Major: 1, Minor: 2, Build: 3, Revision: 456, Suffix: a"
 *
 * @noreturn
 */
stock PrintVersionInfo(const version[], output[] = " ", len = 0)
{
	new major, minor, build, revision;
	new suffix[64], buffer[212];

	ParseVersion(version, major, minor, build, revision, suffix, sizeof(suffix));

	format(buffer, charsmax(buffer), "For: '%s' >> Major: %d", version, major);
	
	if (minor >= 0)
		format(buffer, charsmax(buffer), "%s, Minor: %d", buffer, minor);
	if (build >= 0)
		format(buffer, charsmax(buffer), "%s, Build: %d", buffer, build);
	if (revision >= 0)
		format(buffer, charsmax(buffer), "%s, Revision: %d", buffer, revision);
	if (suffix[0] != '^0')
		format(buffer, charsmax(buffer), "%s, Suffix: %s", buffer, suffix);

	
	if(len > 0) copy(output, len, buffer);
	else server_print(output);
}

/**
 * Parses the version string and retrieves the major, minor, build, revision and suffix components.
 * 
 * @param version        The version string to parse.
 * @param major          The buffer to copy the major version to.
 * @param minor          The buffer to copy the minor version to.
 * @param build          The buffer to copy the build number to.
 * @param revision       The buffer to copy the revision number to.
 * @param suffix         The buffer to copy the suffix to.
 * @param maxSuffixLen   The maximum buffer size for the suffix.
 *
 * @noreturn
 */
stock ParseVersion(const version[], &major, &minor, &build, &revision, suffix[], maxSuffixLen)
{
	new parts[4][64]; // Max 4 parts, 64 characters each
	new count = explode_string(version, ".", parts, sizeof(parts), sizeof(parts[]));

	// Initialize the version components
	major = 0; minor = -1; build = -1; revision = -1;
	suffix[0] = '^0';

	new i = 0, temp[16];

	// --- Major release ---
	copy(temp, sizeof(temp), parts[0]);
	i = 0;
	while (temp[i] && isdigit(temp[i])) i++;
	if (i > 0) major = str_to_num(temp);
	if (temp[i] != '^0') copy(suffix, maxSuffixLen, temp[i]);

	// --- Minor release ---
	if (count > 1)
	{
		copy(temp, sizeof(temp), parts[1]);
		i = 0;
		while (temp[i] && isdigit(temp[i])) i++;
		minor = (i > 0) ? str_to_num(temp) : -1;
		if (temp[i] != '^0' && suffix[0] == '^0') copy(suffix, maxSuffixLen, temp[i]);
	}

	// --- Build number ---
	if (count > 2)
	{
		copy(temp, sizeof(temp), parts[2]);
		i = 0;
		while (temp[i] && isdigit(temp[i])) i++;
		build = (i > 0) ? str_to_num(temp) : -1;
		if (temp[i] != '^0' && suffix[0] == '^0') copy(suffix, maxSuffixLen, temp[i]);
	}

	// --- Revision number ---
	if (count > 3)
	{
		copy(temp, sizeof(temp), parts[3]);
		i = 0;
		while (temp[i] && isdigit(temp[i])) i++;
		revision = (i > 0) ? str_to_num(temp) : -1;
		if (temp[i] != '^0' && suffix[0] == '^0') copy(suffix, maxSuffixLen, temp[i]);
	}
}

/**
 * Compares two version strings and returns true if the first version is greater than the second.
 *
 * @param NewVersion  The new version string to compare.
 * @param OldVersion  The old version string to compare.
 *
 * @return            True if the NewVersion is greater than the OldVersion, false otherwise.
 */
stock bool:CompareVersions(const NewVersion[], const OldVersion[])
{
	new major1, minor1, build1, revision1;
	new major2, minor2, build2, revision2;
	new suffix1[16], suffix2[16];

	ParseVersion(NewVersion, major1, minor1, build1, revision1, suffix1, sizeof(suffix1));
	ParseVersion(OldVersion, major2, minor2, build2, revision2, suffix2, sizeof(suffix2));

	// Compare the version components
	if (major1 != major2) return major1 > major2;               // Major version is the most important
	if (minor1 != minor2) return minor1 > minor2;               // Minor version is the second most important
	if (build1 != build2) return build1 > build2;               // Build number is the third most important
	if (revision1 != revision2) return revision1 > revision2;   // Revision number is the least important
	return strcmp( suffix1, suffix2) == 1;                      // Suffix is the least important
}
