#if defined _zombie_plague_xp_included
  #endinput
#endif
#define _zombie_plague_xp_included

native zpxp_register_human_class(const name[], const info[], const model[], const weapons[], hp, speed, Float:gravity, level)
native zpxp_register_zombie_class(const name[], const info[], const model[], const clawmodel[], hp, speed, Float:gravity, Float:knockback, level)
native zp_get_user_human_class(id)
native zpxp_get_user_level(id)
native zpxp_get_user_human_class_name(id)
forward zp_round_started_human(id)

enum
{
	WEAPON_GALIL = 1,
	WEAPON_FAMAS,
	WEAPON_M4A1,
	WEAPON_AK47,
	WEAPON_SG552,
	WEAPON_AUG,
	WEAPON_SCOUT,
	WEAPON_M3,
	WEAPON_XM1040,
	WEAPON_TMP,
	WEAPON_MAC10,
	WEAPON_UMP45,
	WEAPON_MP5NAVY,
	WEAPON_P90,
	WEAPON_GLOCK18,
	WEAPON_USP,
	WEAPON_P228,
	WEAPON_DEAGLE,
	WEAPON_FIVESEVEN,
	WEAPON_ELITE,
	WEAPON_HEGRENADE,
	WEAPON_FLASHBANG,
	WEAPON_SMOKEGRENADE
}