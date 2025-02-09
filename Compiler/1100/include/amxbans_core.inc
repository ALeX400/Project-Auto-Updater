#pragma reqlib AMXBansCore
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib AMXBansCore
#endif

#if defined _amxbans_core_included
	#endinput
#endif
#define _amxbans_core_included
native amxbans_get_db_prefix(output[], len=32);
native amxbans_get_admin_nick(id, output[], len=32);
native amxbans_get_static_bantime(id);
forward amxbans_admin_connect(id);
forward amxbans_admin_disconnect(id);