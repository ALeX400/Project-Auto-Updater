#include <amxmodx>
#include <easy_http>
#include <amxmisc>

#define UPDATE_JSON_URL "https://raw.githubusercontent.com/ALeX400/Project-Auto-Updater/refs/heads/main/Updates.json"
#define PLUGIN_FOLDER "addons/amxmodx/plugins/"

new const PluginName[] = "Test Auto Updater";
new const PluginVersion[] = "1.0";
new const PluginAuthor[] = "@LeX";

public plugin_init()
{
	register_plugin(PluginName, PluginVersion, PluginAuthor);
	
	// Verifică update-ul la încărcarea pluginului
	set_task(5.0, "CheckForUpdates");

	new plugin_filename[64], plugin_name[64], plugin_version[32], plugin_author[64], plugin_status[32];

	if (get_plugin_info_by_file("TestAutoUpdater.amxx",  plugin_filename, charsmax(plugin_filename), plugin_name, charsmax(plugin_name), plugin_version, charsmax(plugin_version), plugin_author, charsmax(plugin_author), plugin_status, charsmax(plugin_status)))
	{
		server_print("    [PLUGIN INFO] Filename: %s", plugin_filename);
		server_print("    [PLUGIN INFO] Name: %s", plugin_name);
		server_print("    [PLUGIN INFO] Version: %s", plugin_version);
		server_print("    [PLUGIN INFO] Author: %s", plugin_author);
		server_print("    [PLUGIN INFO] Status: %s", plugin_status);
	}
	else
	{
		server_print("[PLUGIN INFO] Plugin 'TestAutoUpdater.amxx' not found!");
	}
}

public CheckForUpdates()
{
	new EzHttpOptions:options = ezhttp_create_options();
	ezhttp_option_set_timeout(options, 10000); // Timeout de 10 secunde
	
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

	new plugin_key[64], found = 0;
	new json_plugin_name[64];

	// Caută pluginul în JSON pe baza `PluginName`
	new total_plugins = ezjson_object_get_count(json);
	for (new i = 0; i < total_plugins; i++)
	{
		ezjson_object_get_name(json, i, plugin_key, charsmax(plugin_key));
		new EzJSON:pluginData = ezjson_object_get_value(json, plugin_key);
		
		if (ezjson_object_has_value(pluginData, "PluginName", EzJSONString))
		{
			ezjson_object_get_string(pluginData, "PluginName", json_plugin_name, charsmax(json_plugin_name));
			
			if (equal(json_plugin_name, PluginName))
			{
				found = 1;
				ProcessPluginUpdate(pluginData);
				break;
			}
		}
		ezjson_free(pluginData);
	}

	ezjson_free(json);

	if (!found)
	{
		server_print("[AutoUpdater] Plugin '%s' not found in update JSON.", PluginName);
	} 
}

public ProcessPluginUpdate(EzJSON:pluginData)
{
	new json_version[32], json_source[64], json_download_url[256];
	ezjson_object_get_string(pluginData, "Version", json_version, charsmax(json_version));
	ezjson_object_get_string(pluginData, "Source", json_source, charsmax(json_source)); // Preia numele fișierului

	replace(json_source, charsmax(json_source), ".sma", ".amxx"); // Înlocuiește extensia fișierului

	new json_numeric_version = GetVersionAsNumber(json_version);
	new current_plugin_version = GetVersionAsNumber(PluginVersion);

	if (json_numeric_version <= current_plugin_version)
	{
		server_print("[AutoUpdater] No update available. Running latest version (%s).", PluginVersion);
		return;
	}

	new version_code = GetVersionAsNumber(AMXX_VERSION_STR);
	
	// Verifică existența obiectului "Download"
	new EzJSON:downloadData = ezjson_object_get_value(pluginData, "Download");
	if (!ezjson_is_object(downloadData))
	{
		server_print("[AutoUpdater] No 'Download' object found in JSON.");
		return;
	}

	// Construim cheia JSON pentru versiune AMX Mod X
	new version_key[8];
	format(version_key, charsmax(version_key), "%d", version_code);

	server_print("[DEBUG] Checking for key in Download object: %s", version_key);

	if (ezjson_object_has_value(downloadData, version_key, EzJSONString))
	{
		ezjson_object_get_string(downloadData, version_key, json_download_url, charsmax(json_download_url));
	}
	else
	{
		server_print("[DEBUG] AMXX_VERSION_STR: %s -> Parsed version: %d", AMXX_VERSION_STR, version_code);
		server_print("[AutoUpdater] No matching version found in JSON for AMX Mod X %s", AMXX_VERSION_STR);
		return;
	}

	// Verifică dacă pluginul existent are același nume ca în JSON
	new filename[128];
	get_plugin(-1, filename, charsmax(filename));

	if (equal(filename, json_source))
	{
		new save_path[512];
		format(save_path, charsmax(save_path), "%s%s", PLUGIN_FOLDER, filename);

		// Face backup fișierului vechi înainte de descărcare
		BackupOldPlugin(save_path);
		
		server_print("[AutoUpdater] Found new plugin version %s (current: %s). Starting download...", json_version, PluginVersion);
		DownloadNewVersion(json_download_url, save_path);
	}
	else
	{
		server_print("[AutoUpdater] Plugin filename mismatch: expected %s, but found %s. Skipping update.", json_source, filename);
	}
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
stock bool:get_plugin_info_by_file(const filename[], plugin_filename[] = "", len1 = 0, plugin_name[] = "", len2 = 0, plugin_version[] = "", len3 = 0, plugin_author[] = "", len4 = 0, plugin_status[] = "", len5 = 0)
{
	new plugin_id = find_plugin_byfile(filename);

	if (plugin_id == -1)
		return false;

	if (get_plugin(plugin_id, plugin_filename, len1, plugin_name, len2, plugin_version, len3, plugin_author, len4, plugin_status, len5) == -1)
		return false;

	return true;
}



public DownloadNewVersion(const json_download_url[], const save_path[])
{
	server_print("[AutoUpdater] Downloading from: %s", json_download_url);

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

	// Extrage numele fișierului din URL
	ezhttp_get_url(request_id, save_path, charsmax(save_path));
	GetFileNameFromURL(save_path, plugin_name, charsmax(plugin_name));

	format(save_path, charsmax(save_path), "%s%s", PLUGIN_FOLDER, plugin_name);
	ezhttp_save_data_to_file(request_id, save_path);
	server_print("[AutoUpdater] Plugin downloaded successfully: %s", save_path);
}

public BackupOldPlugin(const file_path[])
{
	if (!file_exists(file_path))
		return;

	new new_backup[128], i = 0;
	do
	{
		format(new_backup, charsmax(new_backup), "%s.%d.bak", i, file_path);
		i++;
	} while (file_exists(new_backup));

	rename_file(file_path, new_backup, 1);
	server_print("[AutoUpdater] Backup created: %s", new_backup);
}

public GetVersionAsNumber(const version_str[])
{
	new major, minor, patch;
	new pos1, pos2;

	// Găsește pozițiile punctelor în versiune
	pos1 = strfind(version_str, ".");
	pos2 = strfind(version_str, ".", _, pos1 + 1);

	// Extrage major, minor și patch (ignorând build-ul)
	major = str_to_num(version_str[0]);
	minor = str_to_num(version_str[pos1 + 1]);
	patch = str_to_num(version_str[pos2 + 1]);

	// Formăm versiunea codificată
	return major * 100 + minor * 10 + patch;
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
