#include <amxmodx>
#include <easy_http>
#include <amxmisc>

#define UPDATE_JSON_URL "https://raw.githubusercontent.com/ALeX400/Project-Auto-Updater/refs/heads/main/Updates.json"
#define PLUGIN_FOLDER "addons/amxmodx/plugins/"
#define CONFIG_FILE "addons/amxmodx/configs/auto_updater.ini"

new const	PluginName[] = "Auto Updater",
			PluginVersion[] = "1.1",
			PluginAuthor[] = "@LeX";

public plugin_init()
{
	register_plugin(PluginName, PluginVersion, PluginAuthor);
	
	if(!file_exists(CONFIG_FILE))
		write_file(CONFIG_FILE, "", -1);

	server_print("      Aceasta Este Versiunea Actualizata A Pluginului Auto Updater");
	server_print("      Acet mesaj este afisat pentru verificarea corecta a decarcarii pluginului");

	// Verifică update-urile la 10 secunde după încărcare
	set_task(10.0, "CheckForUpdates");
}

public CheckForUpdates()
{
	new EzHttpOptions:options = ezhttp_create_options();
	ezhttp_option_set_timeout(options, 10000);
	
	ezhttp_get(UPDATE_JSON_URL, "OnUpdateCheckComplete", options);
}

public OnUpdateCheckComplete(EzHttpRequest:request_id)
{
	if (!ezhttp_is_request_exists(request_id))
	{
		server_print("[AutoUpdater] HTTP request failed.");
		return;
	}

	if (ezhttp_get_http_code(request_id) != 200)
	{
		server_print("[AutoUpdater] Failed to fetch update JSON. HTTP Code: %d", ezhttp_get_http_code(request_id));
		return;
	}

	new EzJSON:json = ezhttp_parse_json_response(request_id, false);
	if (json == EzInvalid_JSON)
	{
		server_print("[AutoUpdater] Invalid JSON format.");
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
		server_print("[AutoUpdater] Failed to open config file: %s", CONFIG_FILE);
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
			server_print("[AutoUpdater] Failed to get info for plugin: %s", line);
			continue;
		}

		new json_plugin_key[64], found = 0;
		new total_plugins = ezjson_object_get_count(json);

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
					found = 1;
					ezjson_object_get_string(pluginData, "Version", json_version, charsmax(json_version));

					new json_numeric_version = GetVersionAsNumber(json_version);
					new current_plugin_version = GetVersionAsNumber(plugin_version);

					if (json_numeric_version > current_plugin_version)
					{
						server_print("[AutoUpdater] New update found for '%s': %s -> %s", plugin_name, plugin_version, json_version);
						UpdatePlugin(pluginData, plugin_filename);
					}
					break;
				}
			}
			ezjson_free(pluginData);
		}

		if (!found)
		{
			server_print("[AutoUpdater] No update info found for %s (%s).", plugin_name, plugin_filename);
		}
	}

	fclose(file);
}

public UpdatePlugin(EzJSON:pluginData, const plugin_filename[])
{
	new EzJSON:downloadData = ezjson_object_get_value(pluginData, "Download");
	if (!ezjson_is_object(downloadData))
	{
		server_print("[AutoUpdater] No 'Download' object found in JSON for %s.", plugin_filename);
		return;
	}

	new json_download_url[512];
	new version_code = GetVersionAsNumber(AMXX_VERSION_STR);
	new version_key[8];
	format(version_key, charsmax(version_key), "%d", version_code);

	if (ezjson_object_has_value(downloadData, version_key, EzJSONString))
	{
		ezjson_object_get_string(downloadData, version_key, json_download_url, charsmax(json_download_url));
	}
	else
	{
		server_print("[AutoUpdater] No download URL for AMXX version %s in JSON.", AMXX_VERSION_STR);
		return;
	}

	//new save_path[512];
	//format(save_path, charsmax(save_path), "%s%s", PLUGIN_FOLDER, plugin_filename);

	BackupOldPlugin(PLUGIN_FOLDER, plugin_filename);
	//BackupOldPlugin(save_path, plugin_filename);
	//BackupOldPluginAndDownload(save_path, json_download_url, plugin_filename);
	server_print("[AutoUpdater] Downloading update for %s ...", plugin_filename);
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
		server_print("[AutoUpdater] Download request failed.");
		return;
	}

	if (ezhttp_get_http_code(request_id) != 200)
	{
		server_print("[AutoUpdater] Failed to download plugin. HTTP Code: %d", ezhttp_get_http_code(request_id));
		return;
	}

	new save_path[512], plugin_name[128];

	ezhttp_get_url(request_id, save_path, charsmax(save_path));
	GetFileNameFromURL(save_path, plugin_name, charsmax(plugin_name));

	format(save_path, charsmax(save_path), "%s%s", PLUGIN_FOLDER, plugin_name);
	ezhttp_save_data_to_file(request_id, save_path);

	server_print("[AutoUpdater] Plugin downloaded successfully: %s", save_path);
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

/*
public BackupOldPlugin(const file_path[])
{
	if (!file_exists(file_path))
		return;

	new new_backup[128], i = 0;
	do
	{
		format(new_backup, charsmax(new_backup), "%s.%d.bak", file_path, i);
		i++;
	} while (file_exists(new_backup));

	rename_file(file_path, new_backup, 1);
	server_print("[AutoUpdater] Backup created: %s", new_backup);
}
*/

public BackupOldPlugin(const folder[], const filename[])
{
	new file_path[512];
	format(file_path, charsmax(file_path), "%s%s", folder, filename);

	new BcDir[512];
	format(BcDir, charsmax(BcDir), "%sBackups/", folder);
	
	if(!dir_exists(BcDir))
		mkdir(BcDir);

	if (!file_exists(file_path))
		return;

	new pluginVersion[128]
	GetPluginInfo(filename, _, _, _, _, pluginVersion, charsmax(pluginVersion))
	RemoveSpecialCharacters(pluginVersion, charsmax(pluginVersion));

	new name[128], ext[128];
	strtok2(filename, name, charsmax(name), ext, charsmax(ext), '.');

	new new_backup[128]
	format(new_backup, charsmax(new_backup), "%s%s_%s.%s", BcDir, name, pluginVersion, ext);

	rename_file(file_path, new_backup, 1);
	server_print("[AutoUpdater] Backup created: %s", new_backup);
}
/*
public BackupOldPluginAndDownload(const file_path[], const json_download_url[], const plugin_filename[])
{
	if (!file_exists(file_path))
		return;

	new new_backup[128], i = 0;
	do
	{
		format(new_backup, charsmax(new_backup), "%s.%d.bak", file_path, i);
		i++;
	} while (file_exists(new_backup));

	if(rename_file(file_path, new_backup, 1))
		server_print("[AutoUpdater] Backup created: %s", new_backup);
	else
		server_print("[AutoUpdater] Failed to create backup for %s", plugin_filename);

	set_task(3.5, "DelayedDownload", _, json_download_url, strlen(json_download_url));

	server_print("[AutoUpdater] Waiting 3.5s before downloading update for %s ...", plugin_filename);
}

public DelayedDownload(const json_download_url[])
{
	server_print("[AutoUpdater] Downloading update ...");
	DownloadNewVersion(json_download_url);
}
*/
public GetVersionAsNumber(const version_str[])
{
	new major, minor, patch;
	new pos1, pos2;

	pos1 = strfind(version_str, ".");
	pos2 = strfind(version_str, ".", _, pos1 + 1);

	major = str_to_num(version_str[0]);
	minor = str_to_num(version_str[pos1 + 1]);
	patch = str_to_num(version_str[pos2 + 1]);

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