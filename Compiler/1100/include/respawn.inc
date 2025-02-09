#if defined _respawn_included
  #endinput
#endif
#define _respawn_included

#if AMXX_VERSION_NUM >= 175
 #pragma reqlib csdm
 #if !defined AMXMODX_NOAUTOLOAD
  #pragma loadlib csdm
 #endif
#else
 #pragma library csdm
#endif

#define	CSDM_VERSION	"1.0"

#define	CFG_READ		1
#define	CFG_RELOAD		1
#define	CFG_DONE		2

#define DEFAULT_ARMOR		100

#define MAX_WEAPONS 		32 
#define MAX_SECONDARY 		8
#define MAX_PRIMARY 		18

#define CSDM_FFA_ENABLE		3
#define CSDM_FFA_DISABLE	2
#define CSDM_ENABLE			1
#define CSDM_DISABLE		0

#define CSDM_DROP_CONTINUE	0
#define CSDM_DROP_REMOVE	1
#define CSDM_DROP_IGNORE	2

forward csdm_Init(const version[]);
forward csdm_CfgInit();
forward csdm_PreDeath(killer, victim, headshot, const weapon[]);
forward csdm_PostDeath(killer, victim, headshot, const weapon[]);
forward csdm_PreSpawn(player, bool:fake);
forward csdm_PostSpawn(player, bool:fake);
forward csdm_RoundRestart(post);
forward csdm_StateChange(csdm_state);
forward csdm_RemoveWeapon(owner, entity_id, boxed_id);
forward csdm_HandleDrop(id, weapon, death);

native csdm_getpos(player, Float:origin[3], ...);
native csdm_setpos(player, const Float:origin[3], ...);
native csdm_spawnstyles();
native csdm_styleinfo(style_index, name[], maxlength);
native csdm_addstyle(const name[], const function[]);
native csdm_setstyle(const name[]);
native csdm_curstyle();
native csdm_respawn(player);
native csdm_fakespawn(player);
native csdm_reg_cfg(const sectionName[], const handler[]);
native csdm_give_item(player, const item_name[]);
native csdm_force_drop(player, const weapon[], remove=1);
native csdm_remove_weapon(player, const weapon[], seconds, delay);
native csdm_reload_cfg(const file[]="");
native csdm_active();
native csdm_set_active(on_or_off);
native csdm_get_ffa();
native csdm_set_ffa(on_or_off);
native Float:csdm_get_spawnwait();
native csdm_set_spawnwait(Float:time);
native csdm_cache(ar[6], vers=2)
native csdm_trace_hull(Float:origin[3], hull_no);
native csdm_set_intromsg(value=-1);
native csdm_main_menu();

#define CSDM_OPTION_SAYRESPAWN	1
#define CSDM_SET_DISABLED		-1
#define CSDM_SET_DEFAULT		0
#define CSDM_SET_ENABLED		1

native csdm_set_mainoption(option, setting, ...);
native csdm_fwd_drop(player, weapon_id, const name[]); 

#define	SLOT_PRIMARY	1
#define	SLOT_SECONDARY	2
#define	SLOT_KNIFE		3
#define	SLOT_GRENADE	4
#define	SLOT_C4			5

#define	_TEAM_T			1
#define	_TEAM_CT		2

stock g_WeaponSlots[] = 
{
		0,
		2,	//CSW_P228
		0,
		1,	//CSW_SCOUT
		4,	//CSW_HEGRENADE
		1,	//CSW_XM1014
		5,	//CSW_C4
		1,	//CSW_MAC10
		1,	//CSW_AUG
		4,	//CSW_SMOKEGRENADE
		2,	//CSW_ELITE
		2,	//CSW_FIVESEVEN
		1,	//CSW_UMP45
		1,	//CSW_SG550
		1,	//CSW_GALIL
		1,	//CSW_FAMAS
		2,	//CSW_USP
		2,	//CSW_GLOCK18
		1,	//CSW_AWP
		1,	//CSW_MP5NAVY
		1,	//CSW_M249
		1,	//CSW_M3
		1,	//CSW_M4A1
		1,	//CSW_TMP
		1,	//CSW_G3SG1
		4,	//CSW_FLASHBANG
		2,	//CSW_DEAGLE
		1,	//CSW_SG552
		1,	//CSW_AK47
		3,	//CSW_KNIFE
		1	//CSW_P90
};
stock g_MaxBPAmmo[] = {
		0,
		52,	//CSW_P228
		0,
		90,	//CSW_SCOUT
		1,	//CSW_HEGRENADE
		32,	//CSW_XM1014
		1,	//CSW_C4
		100,//CSW_MAC10
		90,	//CSW_AUG
		1,	//CSW_SMOKEGRENADE
		120,//CSW_ELITE
		100,//CSW_FIVESEVEN
		100,//CSW_UMP45
		90,	//CSW_SG550
		90,	//CSW_GALIL
		90,	//CSW_FAMAS
		100,//CSW_USP
		120,//CSW_GLOCK18
		30,	//CSW_AWP
		120,//CSW_MP5NAVY
		200,//CSW_M249
		32,	//CSW_M3
		90,	//CSW_M4A1
		120,//CSW_TMP
		90,	//CSW_G3SG1
		2,	//CSW_FLASHBANG
		35,	//CSW_DEAGLE
		90,	//CSW_SG552
		90,	//CSW_AK47
		0,	//CSW_KNIFE
		100//CSW_P90
	};
	
#define getWepId(%1) get_weaponid(%1)

stock bool:IsValidTeam(team)
{
	if (team == 1 || team == 2)
		return true
	return false
}

stock drop_with_shield(player, weapon_id)
{
	new name[32]
	get_weaponname(weapon_id, name, 31)
	csdm_force_drop(player, "weapon_shield", 0)
	csdm_force_drop(player, name, 0)
	csdm_fwd_drop(player, -1, "weapon_shield")
	csdm_fwd_drop(player, weapon_id, name)
	csdm_give_item(player, "weapon_shield")
}
