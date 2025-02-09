#if defined _ranksclasic
	#endinput
#endif
#define _ranksclasic
#define AUTHOR "renegade"

const KEYSMENU = MENU_KEY_1|MENU_KEY_2|MENU_KEY_3|MENU_KEY_4|MENU_KEY_5|MENU_KEY_6|MENU_KEY_7|MENU_KEY_8|MENU_KEY_9|MENU_KEY_0

#define OFFSET_CS_TEAMS				114
#define OFFSET_LINUX 				5

enum
{
	FM_CS_TEAM_UNASSIGNED = 0,
	FM_CS_TEAM_T,
	FM_CS_TEAM_CT,
	FM_CS_TEAM_SPECTATOR
}

stock register_say(szSay[], szFunction[])
{
	new szTemp[64]
	formatex(szTemp, 63 , "say /%s", szSay)
	register_clcmd(szTemp, szFunction)
	
	formatex(szTemp, 63 , "say .%s", szSay)
	register_clcmd(szTemp, szFunction)
	
	formatex( szTemp, 63 , "say_team /%s", szSay)
	register_clcmd(szTemp, szFunction )
	
	formatex( szTemp, 63 , "say_team .%s", szSay)
	register_clcmd(szTemp, szFunction)
}

stock precache_player_model(const modelname[]) 
{
	static longname[128] 
	formatex(longname, charsmax(longname), "models/player/%s/%s.mdl", modelname, modelname)  	
	precache_model(longname) 
	
	copy(longname[strlen(longname)-4], charsmax(longname) - (strlen(longname)-4), "T.mdl") 
	if (file_exists(longname)) precache_model(longname) 
}

stock fm_cs_get_user_team(id)
	return get_pdata_int(id, OFFSET_CS_TEAMS, OFFSET_LINUX)