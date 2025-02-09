// RENO - Reversed Node Core

#if defined _reno_included
	#endinput
#endif

#define _reno_included

#pragma reqlib reno_core
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib reno_core
#endif

#if !defined _reapi_included
	#include <reapi>
#endif

#define ContainFlag(%1,%2) 			(containi(%1, %2) != -1)
#define ArraySet(%1,%2)				arrayset(_:%1, _:%2, sizeof(%1))

stock const g_szMainDir[] 		= "reno"
stock const g_szExtraCfgDir[] 	= "extra"
stock const g_szConsumersDir[] 	= "consumers"

stock const g_RestrictedWordsFile[] = "restricted_words.ini";
stock const g_ProtectedWordsFile[] 	= "protected_words.ini";

stock const g_RandomNameTagsFile[] 	= "random_nametags.ini";
stock const g_RandomNamesFile[] 	= "random_names.ini";

stock const MIN_WARMUP_TIME = 10
stock const MAX_WARMUP_TIME = 70

enum ReadTypes
{
	CFG_READ,
	CFG_RELOAD
}

new bool:g_bIsUserFrozen[33];

stock UTIL_FreezeUser( id ) 
{ 
    if(is_user_alive(id)) 
    { 
        new iFlags = pev(id, pev_flags) 
        if( ~iFlags & FL_FROZEN ) 
        { 
			g_bIsUserFrozen[id] = true;
			set_pev(id, pev_flags, iFlags | FL_FROZEN);
        } 
    } 
} 

stock UTIL_UnfreezeUser( id ) 
{ 
    if(is_user_alive(id)) 
    { 
        new iFlags = pev(id, pev_flags) 
        if( iFlags & FL_FROZEN ) 
        { 
			g_bIsUserFrozen[id] = false;
			set_pev(id, pev_flags, iFlags & ~FL_FROZEN);
        } 
    } 
}

stock RENO_SetFailState(const szMessage[], any:...)
{
	new szErrorMsg[191]
	vformat(szErrorMsg, charsmax(szErrorMsg), szMessage, 2)

	set_fail_state(szErrorMsg);
}