#include <amxmodx>
#include <amxmisc>
#include <easy_http>
#include <nvault>
#include <regex>
#include <json>

new const	PluginName[] = "Report System",
			PluginVersion[] = "1.6",
			PluginAuthor[] = "Yvezzaint + Edit: @LeX";

new ID_Chosen[33] = {0, ...};
new Reason[33][64];
new bool: Urgent[33] = {false, ...};
new VaultHandle;
new shownMessage[33] = {false, ...};
new ReportCount[33] = {0, ...};
new AdminFlags[64][124];
new FlagsCount;

enum _:CvarString
{
	CVAR_WEBHOOK[504],
	CVAR_IMAGE[128],
	CVAR_SERVER_NAME[128],
	CVAR_FOOTER_TEXT[128],
	CVAR_FLAGS[MAX_NAME_LENGTH],
	ServerDNS[128]
}

enum _:CvarValue
{
	CVAR_MIN_FLAGS,
	CVAR_DYSPLAY_ADMINS,
	CVAR_COUNTDOWN,
	CVAR_ADMIN_NOTIFY,
	CVAR_DISPLAY_FOOTER,
	CVAR_DISPLAY_IMAGE,
	CVAR_FOOTER_TYPE,
	CVAR_ADVANCED_CVAR_CHECK
}

new Val_CvarString[CvarString];
new Val_CvarValue[CvarValue];

new bool:WEBHOOK_STATUS = false;
new bool:IMAGE_STATUS = false;

new bool:is_webhook_valid = false;
new bool:is_image_valid = false;
new bool:webhook_checked = false;
new bool:image_checked = false;

public plugin_init()
{

	register_plugin(PluginName, PluginVersion, PluginAuthor);

	register_clcmd( "say /report", "@Hook_Chat" )
	register_clcmd( "say_team /report", "@Hook_Chat" )

	register_clcmd( "REASON_REPORT", "@Hook_Reason" );

	bind_pcvar_string( register_cvar( "report_webhook", "WEBHOOK_URL" ), Val_CvarString[CVAR_WEBHOOK], charsmax( Val_CvarString[CVAR_WEBHOOK]));
	bind_pcvar_string( register_cvar( "report_image", "https://i.imgur.com/CPCu8U5.gif" ), Val_CvarString[CVAR_IMAGE], charsmax( Val_CvarString[CVAR_IMAGE]));
	bind_pcvar_string( register_cvar( "report_admin_flags", "d" ), Val_CvarString[CVAR_FLAGS], charsmax( Val_CvarString[CVAR_FLAGS]));
	bind_pcvar_string( register_cvar( "report_footer_text", "Gamelife Romania" ), Val_CvarString[CVAR_FOOTER_TEXT], charsmax( Val_CvarString[CVAR_FOOTER_TEXT]));

	bind_pcvar_num( register_cvar( "report_min_flags", "6" ), Val_CvarValue[CVAR_MIN_FLAGS] );
	bind_pcvar_num( register_cvar( "report_admin_display", "4" ), Val_CvarValue[CVAR_DYSPLAY_ADMINS] );
	bind_pcvar_num( register_cvar( "report_cooldown", "30" ), Val_CvarValue[CVAR_COUNTDOWN] );
	bind_pcvar_num( register_cvar( "report_admin_notify", "1" ), Val_CvarValue[CVAR_ADMIN_NOTIFY] );
	bind_pcvar_num( register_cvar( "report_display_footer", "1" ), Val_CvarValue[CVAR_DISPLAY_FOOTER] );
	bind_pcvar_num( register_cvar( "report_display_image", "1" ), Val_CvarValue[CVAR_DISPLAY_IMAGE] );
	bind_pcvar_num( register_cvar( "report_footer_type", "1" ), Val_CvarValue[CVAR_FOOTER_TYPE] );
	bind_pcvar_num( register_cvar( "report_advanced_cvar_check", "1" ), Val_CvarValue[CVAR_ADVANCED_CVAR_CHECK] );

	//get_user_name(-1, Val_CvarString[CVAR_SERVER_NAME], charsmax(Val_CvarString[CVAR_SERVER_NAME]));
	get_cvar_string("hostname", Val_CvarString[CVAR_SERVER_NAME], charsmax(Val_CvarString[CVAR_SERVER_NAME]));
	ExtractDNS(Val_CvarString[CVAR_SERVER_NAME], Val_CvarString[ServerDNS], charsmax(Val_CvarString[ServerDNS]));

	FlagsCount = ParseAccessFlagsFromUsersIni(AdminFlags, sizeof(AdminFlags), charsmax(AdminFlags[]), Val_CvarValue[CVAR_MIN_FLAGS]);

	if(Val_CvarValue[CVAR_ADVANCED_CVAR_CHECK] >= 1)
		StartValidation();
	else
		StandardCheckCvars();
}

// Private Functions
StartValidation()
{
	CheckDiscordWebhook(Val_CvarString[CVAR_WEBHOOK]);

	if (Val_CvarValue[CVAR_DISPLAY_IMAGE] >= 1)
		is_url_image();
	else
	{
		image_checked = true;
		is_image_valid = true;
	}
}

CheckDiscordWebhook(const url[])
{
	new EzHttpOptions:opt = ezhttp_create_options();
	ezhttp_get(url, "onWebhookResponse", opt);
}

is_url_image()
{
	image_checked = true;

	if ( Val_CvarString[CVAR_IMAGE][0] != EOS || !equal(Val_CvarString[CVAR_IMAGE], "IMAGE_URL" ))
		is_image_valid = true;

	CheckValidationCompletion();
}

CheckValidationCompletion()
{
	if (webhook_checked && image_checked)
		OnValidationComplete();
}

OnValidationComplete()
{
	if (is_webhook_valid)
		WEBHOOK_STATUS = true;
	else
		server_print("    [ERROR] >> Cvar 'report_webhook'^t>> does not have a valid value!^n               ( Invalid Token / Unknown Webhook / Not a Webhook )");

	if (is_image_valid)
		IMAGE_STATUS = true;
	else
		server_print("    [ERROR] >> Cvar 'report_image'^t>> does not have a valid value!^n               ( Invalid Image / Unknown Image / Not an Image )");
}

// Public Functions
public StandardCheckCvars()
{
	if ((strcmp(Val_CvarString[CVAR_WEBHOOK], "", false) == 0) || (strcmp(Val_CvarString[CVAR_WEBHOOK], "WEBHOOK_URL", false) == 0) || (!is_valid_discord_webhook(Val_CvarString[CVAR_WEBHOOK])))
		server_print("    [ERROR] >> Cvar 'report_webhook'^t>> does not have a valid value!^n               ( Invalid Token / Unknown Webhook / Not a Webhook )");
	else
		WEBHOOK_STATUS = true;

	if(Val_CvarValue[CVAR_DISPLAY_IMAGE] >= 1)
	{
		if ((strcmp(Val_CvarString[CVAR_IMAGE], "", false) == 0) || (strcmp(Val_CvarString[CVAR_IMAGE], "IMAGE_URL", false) == 0))
			server_print("    [ERROR] >> Cvar 'report_image'^t>> does not have a valid value!^n               ( Invalid Image / Unknown Image / Not an Image )");
		else
			IMAGE_STATUS = true;
	}
}

public plugin_cfg()
{
	VaultHandle = nvault_open( "REPORT_COOLDOWNS" );
	if(VaultHandle == INVALID_HANDLE)
		set_fail_state("[Error] could not open the vault!");

	nvault_prune( VaultHandle, 0, get_systime() - ( 60 * Val_CvarValue[CVAR_COUNTDOWN] ) );
}

public plugin_end()
{
	nvault_close( VaultHandle );
}

public client_putinserver( id )
{
	ID_Chosen[ id ] = 0;
	Reason[ id ][ 0 ] = EOS;
	Urgent[ id ] = false;
}

public onWebhookResponse(EzHttpRequest:req)
{
	webhook_checked = true;

	new EzHttpErrorCode:error_code = ezhttp_get_error_code(req);
	if (error_code != EZH_OK)
		is_webhook_valid = false;
	else
	{
		new http_code = ezhttp_get_http_code(req);
		if (http_code == 200)
			is_webhook_valid = true;
	}

	CheckValidationCompletion();
}

public @Hook_Chat( id )
{
	static bool:bConfigError = false;

	if(!WEBHOOK_STATUS) bConfigError = true;
	if(Val_CvarValue[CVAR_DISPLAY_IMAGE] >= 1 && !IMAGE_STATUS) bConfigError = true;

	if(bConfigError)
	{
		client_print_color( id, print_team_red, "^4[^1Report^4]^1 The plugin is ^3not properly configured^1, please contact the ^4server administrator^1 !" );
		return PLUGIN_HANDLED;
	}

	static steam_id[ 35 ];
	get_user_name(id, steam_id, charsmax( steam_id ) );
	static iTimestamp , szVal[ 10 ];
	if(nvault_lookup( VaultHandle , steam_id , szVal , charsmax( szVal ) , iTimestamp ) || ( iTimestamp && ( ( get_systime() - iTimestamp ) >= (60  * Val_CvarValue[CVAR_COUNTDOWN] ) ) ) )
	{
		client_print_color( id, print_team_default, "^4[^1Report^4]^1 Wait at least^4 30^1 minutes before sending a new ^4report^1!" );
		return PLUGIN_HANDLED;
	}

	new bool:isUserAdmin = false;
	new AdminsBuffer[256];
	new c = GetTopAdmins(AdminFlags, FlagsCount, AdminsBuffer, charsmax( AdminsBuffer ), Val_CvarValue[CVAR_DYSPLAY_ADMINS], true, id, isUserAdmin);

	if(isUserAdmin)
	{
		switch(ReportCount[id])
		{
			case 0: { client_print_color(id, print_team_default, "^4[^1Report^4]^1 Whoa, whoa, whoa, you are an admin, what do you think you're doing ?"); }
			case 1: { client_print_color(id, print_team_default, "^4[^1Report^4]^1 Can't you hear ? Are you blind ?"); }
			case 2: { client_print_color(id, print_team_default, "^4[^1Report^4]^1 Alright, fine, I'll let you off this time..."); }
		}

		if(ReportCount[id] < 3)
		{
			ReportCount[id]++;
			return PLUGIN_HANDLED;
		}
	}

	if (c > 0 && !shownMessage[id] && !isUserAdmin )
	{
		client_print_color(id, print_team_default, "^4[^1Report^4]^1 %s admin%s %s ^4online^1 already, try using ^4u@^1!", c == 1 ? "One" : "Multiple", c == 1 ? "" : "s", c == 1 ? "is" : "are");
		client_print_color(id, print_team_default, "^4[^1Report^4]^1 Online Admins (^4%d^1): %s", c, AdminsBuffer);
		client_print_color(id, print_team_default, "^4[^1Info^4]^1 However, if you want, you can use the command again to send a report!");
		shownMessage[id] = true;
		return PLUGIN_HANDLED;
	}

	@Hook_Report(id);
	shownMessage[id] = false;
	return PLUGIN_HANDLED;
}

public @Hook_Report(id)
{
	if (is_user_bot(id))
		return PLUGIN_HANDLED;

	new menu = menu_create("\d﹝\rREPØRT System\d﹞^n\rStep 1.\d -> Fill out all \wfields^n\rStep 2.\d -> Press \wsubmit", "@Handler");

	new placeholder[256];

	if (ID_Chosen[id] == 0)
		menu_additem(menu, "Select a player \d(\rNONE SELECTED\d)", "1");
	else {
		new name[33];
		get_user_name(ID_Chosen[id], name, charsmax(name));
		formatex(placeholder, charsmax(placeholder), "Select a player \d(\r%s\d)", name);
		menu_additem(menu, placeholder, "1");
	}

	formatex(placeholder, charsmax(placeholder), "Reason for the report \d(\r%s\d)", Reason[id][0] == EOS ? "NOT COMPLETED" : Reason[id]);
	menu_additem(menu, placeholder, "2");

	formatex(placeholder, charsmax(placeholder), "Is it urgent? \d(%s\d)", Urgent[id] ? "\rYES\d/NO" : "\dYES/\rNO");
	menu_additem(menu, placeholder, "3");

	menu_addblank(menu, 0);
	menu_additem(menu, "Submit the \rreport", "4");

	menu_display(id, menu);

	return PLUGIN_HANDLED;
}

public @Handler(id, menu, item)
{
	if (item == MENU_EXIT) {
		menu_destroy(menu);
		return PLUGIN_HANDLED;
	}

	new Data[6], Name[64];
	new Access, CallBack;
	menu_item_getinfo(menu, item, Access, Data, 5, Name, 63, CallBack);
	new Key = str_to_num(Data);

	switch (Key)
	{
		case 1: {
			ChooseMenu(id);
		}
		case 2: {
			client_cmd(id, "messagemode REASON_REPORT");
		}
		case 3: {
			Urgent[id] = !Urgent[id];
			@Hook_Report(id);
		}
		case 4: {
			// Verify if the player has selected a target
			new playername[33]; get_user_name(id, playername, charsmax(playername));
			new targetname[33]; get_user_name(ID_Chosen[id], targetname, charsmax(targetname));
			new player_steam[35]; get_user_authid(id, player_steam, charsmax(player_steam));
			new target_steam[35]; get_user_authid(ID_Chosen[id], target_steam, charsmax(target_steam));
			new timer[33]; get_time("%d/%m/%Y - %H:%M %p", timer, charsmax(timer));
			new mapname[33]; get_mapname(mapname, charsmax(mapname));

			// Send the report
			SendReport(playername, player_steam, targetname, target_steam, Reason[id], timer, mapname, Urgent[id]);

			// Notify all admins about the new report
			if(Val_CvarValue[CVAR_ADMIN_NOTIFY] >= 1)
				NotifyAdmins(id, ID_Chosen[id], Reason[id]);

			// Reset the player's data
			ID_Chosen[id] = 0;
			Reason[id][0] = EOS;
			Urgent[id] = false;

			// Set the cooldown
			new steam_id[35];
			get_user_name(id, steam_id, charsmax(steam_id));
			nvault_set(VaultHandle, steam_id, "REPORTED");
		}
	}

	menu_destroy(menu);
	return PLUGIN_HANDLED;
}

public Report_Response(EzHttpRequest:request)
{
	return;
}

public ChooseMenu(id)
{
	new menu = menu_create("\d﹝\rREPØRT System\d﹞^n\dSelect a player", "@ChooseHandler");

	new players[32], n, tempid, counter = 0;
	new szName[32], szUserId[32];

	get_players(players, n, "chi");

	for (new i; i < n; i++)
	{
		tempid = players[i];
		if (tempid == id)
			continue;

		get_user_name(tempid, szName, charsmax(szName));
		formatex(szUserId, charsmax(szUserId), "%d", get_user_userid(tempid));

		menu_additem(menu, szName, szUserId, 0);
		counter++;
	}

	if (counter == 0) {
		client_print_color(id, print_team_default, "^4[^1Report^4]^1 No players online!");
		return PLUGIN_HANDLED;
	}

	menu_display(id, menu, 0);
	return PLUGIN_HANDLED;
}

public @ChooseHandler( id, menu, item )
{
	if ( item == MENU_EXIT )
	{
		menu_destroy( menu );
		return PLUGIN_HANDLED;
	}

	new szData[6], szName[64];
	new _access, item_callback;
	menu_item_getinfo( menu, item, _access, szData,charsmax( szData ), szName,charsmax( szName ), item_callback );

	new userid = str_to_num( szData );

	new player = find_player( "k", userid );

	if ( player )
	{
		ID_Chosen[ id ] = player;
		@Hook_Report( id );
	}

	menu_destroy( menu );
	return PLUGIN_HANDLED;
}

public @Hook_Reason(id)
{
	new szTemp[64];
	read_args(szTemp, charsmax(szTemp));
	remove_quotes(szTemp);

	if (strlen(szTemp) < 3)
	{
		client_print_color(id, print_team_default, "^4[^1Report^4]^1 The reason you provided is too ^4short^1!");
		client_cmd(id, "messagemode REASON_REPORT");
		return PLUGIN_HANDLED;
	}

	copy(Reason[id], charsmax(Reason[]), szTemp);
	@Hook_Report(id);

	return PLUGIN_HANDLED;
}

/**
 *                                  Stocks
 */

 /**
 * The function that validates the URL of a Webhook Discord.
 *
 * @param url       The URL to be validated.
 *
 * @return          TRUE If the URL is valid, FALSE otherwise.
 */
stock bool:is_valid_discord_webhook(const url[])
{
	new const pattern[] = "^^.*(discord|discordapp).com\/api\/webhooks\/([\d]+)\/([a-zA-Z0-9_.-]*)";
	new result = regex_match_simple(url,pattern, PCRE_CASELESS );
	return (result > 0);
}

/**
* Extract the access flags from users.ini and sort them down.
*
* @param buffer       2D buffer for flags (eg G_adminFlags[][24])
* @param bufferSize   Buffer size (maximum number of ranks)
* @param maxFlagLen   Maximum length of a flag (eg: 24)
* @param minFlags     Minimum number of flags to be considered (default: 4)
*
* @return             Number of unique flags found, or -1 to error
*/
stock ParseAccessFlagsFromUsersIni(buffer[][], bufferSize, maxFlagLen, minFlags = 4)
{
	new _dir[128];
	new filename[128];
	get_configsdir(_dir, charsmax(_dir));
	format(filename, charsmax(filename), "%s/users.ini", _dir);

	if (!file_exists(filename))
		return -1;

	new file = fopen(filename, "rt");
	if (!file)
		return -1;

	static tempFlags[64][128];
	new count = 0;
	new line[256];

	while (!feof(file))
	{
		fgets(file, line, charsmax(line));
		trim(line);

		if (line[0] == ';' || line[0] == '#' || !line[0])
			continue;

		new accessFlags[64], dummy[4][64];
		parse(line, dummy[0], charsmax(dummy[]), dummy[1], charsmax(dummy[]), accessFlags, charsmax(accessFlags), dummy[3], charsmax(dummy[]));

		if (strlen(accessFlags) >= minFlags) {
			new bool:exists = false;
			for (new i = 0; i < count; i++)
			{
				if (equal(tempFlags[i], accessFlags))
				{
					exists = true;
					break;
				}
			}

			if (!exists && count < sizeof(tempFlags))
			{
				copy(tempFlags[count], charsmax(tempFlags[]), accessFlags);
				count++;
			}
		}
	}
	fclose(file);

	// Sortare
	for (new i = 0; i < count - 1; i++) {
		for (new j = i + 1; j < count; j++) {
			if (strlen(tempFlags[i]) < strlen(tempFlags[j])) {
				new temp[64];
				copy(temp, charsmax(temp), tempFlags[i]);
				copy(tempFlags[i], charsmax(tempFlags[]), tempFlags[j]);
				copy(tempFlags[j], charsmax(tempFlags[]), temp);
			}
		}
	}

	new validCount = 0;
	for (new i = 0; i < count && validCount < bufferSize; i++)
	{
		copy(buffer[validCount], maxFlagLen, tempFlags[i]);
		validCount++;
	}

	return validCount;
}

/**
* Get the most important admini online, sorted by Rank.
*
* @param adminFlags       - External 2D array with flags for each rank (ex: g_adminFlags[][24])
* @param adminFlagsSize   - The number of ranks in Array (ex: sizeof(g_adminFlags))
* @param buffer           - The buffer where the result is saved
* @param bufferMax        - Maximum buffer size
* @param maxAdminsToDisplay - Maximum number of admini display (default: 4)
* @param color            - Display with color codes (default: TRUE)
* @param id               - Player ID for personal verification (default: -1)
* @param isUserAdmin      - Boolean variable indicating whether the 'ID' player is admin
*
* @return                 - Total number of online admines
*/
stock GetTopAdmins(const adminFlags[][], adminFlagsSize, buffer[], bufferMax, maxAdminsToDisplay = 4, bool:color = true, id = -1, &bool:isUserAdmin = false)
{
	new players[32], numPlayers;
	get_players(players, numPlayers, "ch");

	const MAX_ADMINS = 32;
	new topAdmins[MAX_ADMINS][32];
	new adminRanks[MAX_ADMINS];
	new totalAdmins = 0;
	new sortedAdmins = 0;

	for (new i = 0; i < numPlayers; i++)
	{
		new player = players[i];
		new isAdmin = 0;
		new highestRank = adminFlagsSize;

		for (new rank = 0; rank < adminFlagsSize; rank++)
		{
			if (has_all_flags(player, adminFlags[rank]))
			{
				isAdmin = 1;
				highestRank = rank;
				break;
			}
		}

		if (player == id && isAdmin)
			isUserAdmin = true;

		if (!isAdmin)
			continue;

		totalAdmins++;

		if (sortedAdmins < maxAdminsToDisplay)
		{
			formatex(topAdmins[sortedAdmins], charsmax(topAdmins[]), "%n", player);
			adminRanks[sortedAdmins] = highestRank;
			sortedAdmins++;
		}
		else
		{
			new worstIndex = -1;
			new worstRank = -1;

			for (new j = 0; j < sortedAdmins; j++)
			{
				if (adminRanks[j] > worstRank)
				{
					worstRank = adminRanks[j];
					worstIndex = j;
				}
			}

			if (highestRank < worstRank)
			{
				formatex(topAdmins[worstIndex], charsmax(topAdmins[]), "%n", player);
				adminRanks[worstIndex] = highestRank;
			}
		}
	}

	if (totalAdmins == 0)
	{
		buffer[0] = '^0';
		return 0;
	}

	new pos = 0;
	for (new i = 0; i < sortedAdmins; i++)
	{
		switch(color)
		{
			case true: { pos += formatex(buffer[pos], bufferMax - pos, "%s^4%s^1", i > 0 ? ", " : "", topAdmins[i]); }
			case false: { pos += formatex(buffer[pos], bufferMax - pos, "%s%s", i > 0 ? ", " : "", topAdmins[i]); }
		}
	}

	if (totalAdmins > maxAdminsToDisplay)
	{
		switch(color)
		{
			case true: { pos += formatex(buffer[pos], bufferMax - pos, ",^4 ...^1"); }
			case false: { pos += formatex(buffer[pos], bufferMax - pos, ", ..."); }
		}
	}

	return totalAdmins;
}

/**
 * Stock function to generate and send a report
 *
 * @param reporter_name        The name of the reporter
 * @param reporter_steam       The Steam ID of the reporter
 * @param reported_name        The name of the reported player
 * @param reported_steam       The Steam ID of the reported player
 * @param reason               The reason for the report
 * @param report_time          The time of the report
 * @param map_name             The current map name
 * @param mentionEveryone      (Optional) If true, the message will mention @everyone
 */
stock SendReport(const reporter_name[], const reporter_steam[], const reported_name[], const reported_steam[], const reason[], const report_time[], const map_name[], bool:mentionEveryone = true)
{
	new JSON:json_embed = json_init_object();

	// Embed title and color
	json_object_set_string(json_embed, "title", "Report");
	json_object_set_number(json_embed, "color", 16711680); // Red color

	// Fields array
	new JSON:fields_array = json_init_array();

	// Correct format: User + SteamID in the same field
	json_array_append_value(fields_array, json_create_field("Report realizat de", fmt("%s (`%s`)", reporter_name, reporter_steam), false));
	json_array_append_value(fields_array, json_create_field("Utilizator reclamat", fmt("%s (`%s`)", reported_name, reported_steam), false));
	json_array_append_value(fields_array, json_create_field("Motivul reclamatiei", reason, false));
	json_array_append_value(fields_array, json_create_field("Data & Ora", report_time, false));
	json_array_append_value(fields_array, json_create_field("Harta curenta", map_name, false));


	json_object_set_value(json_embed, "fields", fields_array);

	new JSON:image_obj;
	// Embed image
	if (Val_CvarValue[CVAR_DISPLAY_IMAGE] >= 1 && IMAGE_STATUS)
	{
		image_obj = json_init_object();
		json_object_set_string(image_obj, "url", Val_CvarString[CVAR_IMAGE]);
		json_object_set_value(json_embed, "image", image_obj);
	}

	// Footer
	new JSON:footer_obj
	if(Val_CvarValue[CVAR_DISPLAY_FOOTER] >= 1)
	{
		footer_obj = json_init_object();

		if(Val_CvarString[ServerDNS][0] == EOS)
			json_object_set_string(footer_obj, "text", "Report System");
		else
			json_object_set_string(footer_obj, "text", fmt("Report System for %s", Val_CvarValue[CVAR_FOOTER_TYPE] >= 1 ? (fmt("%s", Val_CvarString[ServerDNS])) : (fmt("%s", Val_CvarString[CVAR_FOOTER_TEXT]))));
		
		json_object_set_value(json_embed, "footer", footer_obj);
	}

	// Root JSON object
	new JSON:root_json = json_init_object();

	// Mention everyone if enabled
	if (mentionEveryone)
		json_object_set_string(root_json, "content", "@everyone");

	json_object_set_string(root_json, "username", "Report System");

	new JSON:embeds_array = json_init_array();
	json_array_append_value(embeds_array, json_embed);
	json_object_set_value(root_json, "embeds", embeds_array);

	// Convert JSON to string
	new json_string[2048];
	json_serial_to_string(root_json, json_string, charsmax(json_string));

	// Send the report via HTTP request
	new EzHttpOptions:options = ezhttp_create_options();
	ezhttp_option_set_body(options, json_string);
	ezhttp_option_set_header(options, "Content-Type", "application/json");

	ezhttp_post(Val_CvarString[CVAR_WEBHOOK], "Report_Response", options);

	// Free JSON memory
	json_free(json_embed);
	json_free(fields_array);
	if(Val_CvarValue[CVAR_DISPLAY_IMAGE] >= 1) json_free(image_obj);
	if(Val_CvarValue[CVAR_DISPLAY_FOOTER] >= 1) json_free(footer_obj);
	json_free(root_json);
	json_free(embeds_array);

	return PLUGIN_HANDLED;
}

// Function to create a properly formatted JSON field
stock JSON:json_create_field(const key[], const value[], bool:inline = false)
{
	new JSON:field = json_init_object();
	json_object_set_string(field, "name", key);
	json_object_set_string(field, "value", value);
	json_object_set_bool(field, "inline", inline);
	return field;
}

stock bool:ExtractDNS(const hostname[], buffer[], len)
{
	new const patterns[][] = {
		"([a-zA-Z0-9-]+\.(?:com|net|org|ro|club|zone|gaming))",
		"([a-zA-Z0-9-]+\.[a-zA-Z0-9-]+\.[a-zA-Z]{2,})"
	};

	new bool:found = false;
	new Regex:regex;
	new error[128], errcode;
	new result[128];

	for (new i = 0; i < sizeof(patterns); i++)
	{
		regex = regex_match(hostname, patterns[i], errcode, error, charsmax(error));

		if (regex > REGEX_NO_MATCH)
		{
			regex_substr(regex, 0, result, charsmax(result));
			strtolower(result);

			if (!found)
			{
				formatex(buffer, len, "%s", result);
				found = true;
			}
			regex_free(regex);
			break;
		}
	}
	return found;
}

/**
 * Notify all admins about a new report
 *
 * @param id          The ID of the player who made the report
 * @param ID_Chosen   The ID of the player who was reported
 * @param reason      The reason for the report
**/
stock NotifyAdmins(id, ID_Chosen, const reason[])
{
	new players[32], inum, pl;
	get_players(players, inum, "ch")

	for (new i; i < inum; i++)
	{
		pl = players[i]
		if (access(pl, read_flags(Val_CvarString[CVAR_FLAGS])))
		{
			client_print_color(pl, print_team_red, "^4[^1Report^4]^1 ^4%n^1 has submitted a new report against player ^3%n^1 with the reason: ^4%s^1", id, ID_Chosen, reason);
			client_cmd(pl, "spk ^"vox/warning message for administration^"");
		}
	}
}