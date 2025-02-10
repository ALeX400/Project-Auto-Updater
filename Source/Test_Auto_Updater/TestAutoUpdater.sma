#include <amxmodx>
#include <easy_http>
#include <amxmisc>

#define UPDATE_JSON_URL "https://raw.githubusercontent.com/ALeX400/Project-Auto-Updater/refs/heads/main/Updates.json"
#define PLUGIN_NAME "Auto_Updater"
#define PLUGIN_FOLDER "addons/amxmodx/plugins/"

new const	PluginName[] = "Auto Updater",
			PluginVersion[] = "1.0",
			PluginAuthor[] = "@LeX";

public plugin_init()
{
	register_plugin(PluginInfo[0], PluginInfo[1], PluginInfo[2]);
	
	// Verifică update-ul la încărcarea pluginului
	set_task(5.0, "CheckForUpdates");
}

public CheckForUpdates()
{
	EzHttpOptions:options = ezhttp_create_options();
	ezhttp_option_set_timeout(options, 10000); // Timeout de 10 secunde
	
	ezhttp_get(UPDATE_JSON_URL, "OnUpdateCheckComplete", options);
}

public OnUpdateCheckComplete(EzHttpRequest:request_id)
{
	if (!ezhttp_is_request_exists(request_id))
	{
		log_amx("[AutoUpdater] HTTP request failed.");
		return;
	}

	if (ezhttp_get_http_code(request_id) != 200)
	{
		log_amx("[AutoUpdater] Failed to fetch update JSON. HTTP Code: %d", ezhttp_get_http_code(request_id));
		return;
	}

	EzJSON:json = ezhttp_parse_json_response(request_id, false);
	if (json == EzInvalid_JSON)
	{
		log_amx("[AutoUpdater] Invalid JSON format.");
		return;
	}

	if (!ezjson_object_has_value(json, PLUGIN_NAME, EzJSONObject))
	{
		log_amx("[AutoUpdater] Plugin '%s' not found in JSON.", PLUGIN_NAME);
		ezjson_free(json);
		return;
	}

	EzJSON:pluginData = ezjson_object_get_value(json, PLUGIN_NAME);
	new json_version[32];
	ezjson_object_get_string(pluginData, "Version", json_version, charsmax(json_version));

	new json_download_url[256];
	//new current_version[32] = AMXX_VERSION_STR;
	new version_code = GetVersionAsNumber(AMXX_VERSION_STR);
	
	// Construim cheia JSON pentru versiune
	new version_key[8];
	format(version_key, charsmax(version_key), "%d", version_code);

	if (ezjson_object_has_value(pluginData, version_key, EzJSONString))
	{
		ezjson_object_get_string(pluginData, version_key, json_download_url, charsmax(json_download_url));
	}
	else
	{
		log_amx("[AutoUpdater] No matching version found in JSON for AMX Mod X %s", AMXX_VERSION_STR);
		ezjson_free(pluginData);
		ezjson_free(json);
		return;
	}

	ezjson_free(pluginData);
	ezjson_free(json);

	new json_numeric_version = GetVersionAsNumber(json_version);

	if (json_numeric_version > version_code)
	{
		log_amx("[AutoUpdater] Found new version %s (current: %s). Starting download...", json_version, AMXX_VERSION_STR);
		DownloadNewVersion(json_download_url);
	}
	else
	{
		log_amx("[AutoUpdater] No update available. Running latest version.");
	}
}

public DownloadNewVersion(const json_download_url[])
{
	new save_path[128], plugin_name[64];
	
	// Extrage numele fișierului din URL
	GetFileNameFromURL(json_download_url, plugin_name, charsmax(plugin_name));
	
	format(save_path, charsmax(save_path), "%s%s", PLUGIN_FOLDER, plugin_name);

	BackupOldPlugin(save_path);
	log_amx("[AutoUpdater] Downloading from: %s", json_download_url);

	EzHttpOptions:options = ezhttp_create_options();
	ezhttp_option_set_timeout(options, 5000);
	
	ezhttp_get(json_download_url, "OnPluginDownloadComplete", options);
}

public OnPluginDownloadComplete(EzHttpRequest:request_id)
{
	if (!ezhttp_is_request_exists(request_id))
	{
		log_amx("[AutoUpdater] Download request failed.");
		return;
	}

	if (ezhttp_get_http_code(request_id) != 200)
	{
		log_amx("[AutoUpdater] Failed to download plugin. HTTP Code: %d", ezhttp_get_http_code(request_id));
		return;
	}

	new save_path[128], plugin_name[64];
	
	// Extrage numele fișierului din URL
	ezhttp_get_url(request_id, save_path, charsmax(save_path));
	GetFileNameFromURL(save_path, plugin_name, charsmax(plugin_name));

	format(save_path, charsmax(save_path), "%s%s", PLUGIN_FOLDER, plugin_name);
	ezhttp_save_data_to_file(request_id, save_path);
	log_amx("[AutoUpdater] Plugin downloaded successfully: %s", save_path);
}

public BackupOldPlugin(const file_path[])
{
	if (!file_exists(file_path))
	{
		return;
	}

	new new_backup[128], i = 0;
	do
	{
		format(new_backup, charsmax(new_backup), "%s.bak%d", file_path, i);
		i++;
	} while (file_exists(new_backup));

	rename_file(file_path, new_backup);
	log_amx("[AutoUpdater] Backup created: %s", new_backup);
}

public GetVersionAsNumber(const version_str[])
{
	new major, minor;
	if (str_to_num(version_str[0]) == 1)
	{
		major = str_to_num(version_str[2]);
		minor = str_to_num(version_str[4]);
		return major * 100 + minor * 10;
	}
	return 0;
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
