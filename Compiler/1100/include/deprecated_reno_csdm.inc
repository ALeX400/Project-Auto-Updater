// Reversed Node CSDM
#include <reno_natives>

#if defined _reno_csdm_included
	#endinput
#endif

#define _reno_csdm_included

#pragma reqlib reno_csdm_main
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib reno_csdm_main
#endif

#if !defined _reapi_included
	#include <reapi>
#endif

#if !defined RenoCSDM_VERSION
	#define RenoCSDM_VERSION "X1"
#endif

stock const g_szMainDir[] = "csdm"
stock const g_szInitDir[] = "init"
stock const g_szExtraCfgDir[] = "extra"
stock const g_szSpawnDir[] = "spawns"
stock const g_szMainSection[] = "[respawn]"

// configdata
const MAX_LINE_LEN = 64
const MAX_KEY_LEN = 32
const MAX_VALUE_LEN = 32
const MAX_SECTION_LEN = 32

enum ReadTypes
{
	CFG_READ,
	CFG_RELOAD
}

enum EquipTypes
{
	EQUIP_NONE = -1,
	AUTO_EQUIP,
	EQUIP_MENU,
	RANDOM_WEAPONS,
	FREE_BUY
}

enum ColorTypes
{
	DONT_CHANGE = 0,
	RED,
	BLUE,
	GREY
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ FORWARDS ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

/** Called when CSDM initializes.
*
* @param szVersion			It passes in a version string, or a blank string if some load failure occurred.
*/
forward RenoCSDM_Initialized(const szVersion[])

/** Called when core requests a map prefix configuration file.
*
* @param szConfig			Config file
* @param iSize				Size
* @param szMapPrefix		Map prefix
*/
forward RenoCSDM_GetConfigMapPrefixFile(szConfig[], const iSize, const szMapPrefix[])

/** Called when core requests a configuration file.
*
* @param szConfig	Config file
* @param iSize		Size
*/
forward RenoCSDM_GetConfigFile(szConfig[], const iSize)

/** Called when CSDM is ready to accept config hooks.
*
* @param iReadAction		See enum ReadTypes
*/
forward RenoCSDM_ConfigurationLoad(const ReadTypes:iReadAction)

/** Called when CSDM is execute CVar values.
*/
forward RenoCSDM_ExecuteCVarValues()


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ NATIVES ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

/** Read config section.
*
* forward params: public my_config_handler(const szLineData[], const iSectionID)
*
* @param szSectionName 		Section name defined in config.
* @param szHandler 			Name of the handler function.
* @param iSectionID 		section Index. (for multiple handler support)
*
* @return  					Index of section item.
*/
native RenoCSDM_RegisterConfig(const szSectionName[], const szHandler[])

/** Gets config keyvalue.
*
* @param szKey				Keyname
* @param szValue			The variable to store the value in.
* @param iLen				Lenght
*
* @return					true/false
*/
native bool:RenoCSDM_GetConfigKeyValue(const szKey[], szValue[], iLen)

/** Gets equip mode.
*
* @return 					enum EquipTypes
*/
native RenoCSDM_GetEquipmode()

/** Sets equip mode.
*
* @param iNewMode 			enum EquipTypes
*
* @noreturn
*/
native RenoCSDM_SetEquipmode(const EquipTypes:iNewMode)

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ HELPER STUFF ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

// cfg
#define ContainFlag(%1,%2) 			(containi(%1, %2) != -1)
#define ParseConfigKey(%1,%2,%3,%4) (parse(%1, %2, charsmax(%2), %3, charsmax(%3), %4, charsmax(%4)) == 3 && %3[0] == '=')
#define IsCommentLine(%1) 			(%1[0] == ';' || %1[0] == '#' || (%1[0] == '/' && %1[1] == '/'))

#define IsValidTeam(%1) 			(TEAM_TERRORIST <= get_member(%1, m_iTeam) <= TEAM_CT)
#define IsValidWeaponID(%1) 		(any:WEAPON_P228 <= %1 <= any:WEAPON_P90 && !(INVALID_WEAPONS_BS & (1<<any:%1)))

#define ArraySet(%1,%2)				arrayset(_:%1, _:%2, sizeof(%1))
#define Vector(%1,%2,%3)			(Float:{%1.0, %2.0, %3.0})
#define VECTOR_ZERO 				Vector(0, 0, 0)

#define GetEntityKeyID(%1)			get_entvar(%1, var_impulse)
#define SetEntityKeyID(%1,%2)		set_entvar(%1, var_impulse, %2)

// fakemeta
#if !defined _fakemeta_included
	#define ENTITY_THINK(%1) 		dllfunc(DLLFunc_Think, %1)
	#define SPAWN_ENTITY(%1) 		dllfunc(DLLFunc_Spawn, %1)
	#define REMOVE_ENTITY(%1) 		engfunc(EngFunc_RemoveEntity, %1)
	#define SET_ORIGIN(%1,%2) 		engfunc(EngFunc_SetOrigin, %1, %2)
	#define SET_SIZE(%1,%2,%3) 		engfunc(EngFunc_SetSize, %1, %2, %3)
	#define SET_MODEL(%1,%2)		engfunc(EngFunc_SetModel, %1, %2)
#endif

const GRENADE_BS = ((1<<_:WEAPON_HEGRENADE)|(1<<_:WEAPON_SMOKEGRENADE)|(1<<_:WEAPON_FLASHBANG))
const INVALID_WEAPONS_BS = ((1<<_:WEAPON_NONE)|(1<<_:WEAPON_GLOCK)|(1<<_:WEAPON_KNIFE)|(1<<_:WEAPON_C4))

const INVALID_INDEX = -1
const COLOR_CLIENT_INDEX = 34

stock const g_szValidItemNames[][] =
{
	"","weapon_p228","","weapon_scout","weapon_hegrenade","weapon_xm1014","weapon_c4",
	"weapon_mac10","weapon_aug","weapon_smokegrenade","weapon_elite","weapon_fiveseven","weapon_ump45",
	"weapon_sg550","weapon_galil","weapon_famas","weapon_usp","weapon_glock18","weapon_awp",
	"weapon_mp5navy","weapon_m249","weapon_m3","weapon_m4a1","weapon_tmp","weapon_g3sg1",
	"weapon_flashbang","weapon_deagle","weapon_sg552","weapon_ak47","weapon_knife","weapon_p90",
	"weapon_shield","item_kevlar","item_assaultsuit","item_thighpack","item_longjump"
}

stock const g_szTeamNames[ColorTypes][] =
{
	"UNASSIGNED",
	"TERRORIST",
	"CT",
	"SPECTATOR"
}

stock const g_szEquipModes[EquipTypes][] =
{
	"Auto Equip",
	"Equip Menu",
	"Random Weapons",
	"Free buy"
}

stock const g_iMaxBPAmmo[any:WEAPON_P90 + 1] =
{
	-1, 52, -1, 90, 1, 32, 1, 100, 90, 1, 120, 100, 100, 90, 90, 90,
	100, 120, 30, 120, 200, 32, 90, 120, 90, 2, 35, 90, 90, -1, 100
}

stock bool:RenoCSDM_Running()
{
	new szStatus[2], iPluginIndex
	if((iPluginIndex = is_plugin_loaded(RenoCSDM_PLUGIN_NAME)) == INVALID_PLUGIN_ID)
		return false

	if(get_plugin(iPluginIndex, .status = szStatus, .len5 = charsmax(szStatus)) == INVALID_PLUGIN_ID)
		return false

	// 'r' = "running", 'd' = "debug", 'p' = "paused", 's' = "stopped", 'e' = "error", 'b' = "bad load"
	return bool:(szStatus[0] == 'r' || szStatus[0] == 'd')
}

stock RenoCSDM_SetFailState(const szMessage[], any:...)
{
	new szErrorMsg[191]
	vformat(szErrorMsg, charsmax(szErrorMsg), szMessage, 2)

	set_fail_state(szErrorMsg)
}

stock MakeDir(const szDirName[], bool:bPrint = true)
{
	if(dir_exists(szDirName))
		return

	if(bPrint) {
		server_print("[RRENO] Directory ^"%s^" not exist, will be created automatically.", szDirName)
	}
	if(mkdir(szDirName)) {
		server_print("[RRENO] Failed to create directory ^"%s^"", szDirName)
	}
}

stock rg_set_rendering(const pEntity, const fx = kRenderFxNone, const Float:flColor[] = {0.0, 0.0, 0.0}, const Float:iAmount = 0.0)
{
	set_entvar(pEntity, var_renderfx, fx)
	set_entvar(pEntity, var_rendercolor, flColor)
	set_entvar(pEntity, var_renderamt, iAmount)
}
