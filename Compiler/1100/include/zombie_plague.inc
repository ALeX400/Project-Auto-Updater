#if defined _zombie_plague_included
  #endinput
#endif
#define _zombie_plague_included

#define ZP_TEAM_ZOMBIE (1<<0)
#define ZP_TEAM_HUMAN (1<<1)
#define ZP_TEAM_NEMESIS (1<<2)
#define ZP_TEAM_ASSASSIN (1<<3)
#define ZP_TEAM_SURVIVOR (1<<4)
#define ZP_TEAM_SNIPER (1<<5)

enum
{
	MODE_INFECTION = 1,
	MODE_NEMESIS,
	MODE_ASSASSIN,
	MODE_SURVIVOR,
	MODE_SNIPER,
	MODE_SWARM,
	MODE_MULTI,
	MODE_PLAGUE,
	MODE_ARMAGEDDON,
	MODE_APOCALYPSE,
	MODE_NIGHTMARE
}

enum
{
	WIN_NO_ONE = 0,
	WIN_ZOMBIES,
	WIN_HUMANS
}
#define ZP_PLUGIN_HANDLED 97
native zp_get_user_zombie(id)
native zp_get_user_nemesis(id)
native zp_get_user_assassin(id)
native zp_get_user_survivor(id)
native zp_get_user_sniper(id)
native zp_get_user_first_zombie(id)
native zp_get_user_last_zombie(id)
native zp_get_user_last_human(id)
native zp_get_user_zombie_class(id)
native zp_get_user_next_class(id)
native zp_set_user_zombie_class(id, classid)
native zp_get_user_ammo_packs(id)
native zp_set_user_ammo_packs(id, amount)
native zp_get_zombie_maxhealth(id)
native zp_get_user_batteries(id)
native zp_set_user_batteries(id, charge)
native zp_get_user_nightvision(id)
native zp_set_user_nightvision(id, set)
native zp_infect_user(id, infector = 0, silent = 0, rewards = 0)
native zp_disinfect_user(id, silent = 0)
native zp_make_user_nemesis(id)
native zp_make_user_assassin(id)
native zp_make_user_survivor(id)
native zp_make_user_sniper(id)
native zp_respawn_user(id, team)
native zp_force_buy_extra_item(id, itemid, ignorecost = 0)
native zp_override_user_model(id, const newmodel[], modelindex = 0)
native zp_has_round_started()
native zp_is_nemesis_round()
native zp_is_assassin_round()
native zp_is_survivor_round()
native zp_is_sniper_round()
native zp_is_swarm_round()
native zp_is_plague_round()
native zp_is_armageddon_round()
native zp_is_apocalypse_round()
native zp_is_nightmare_round()
native zp_get_zombie_count()
native zp_get_human_count()
native zp_get_nemesis_count()
native zp_get_assassin_count()
native zp_get_survivor_count()
native zp_get_sniper_count()
native zp_register_extra_item(const name[], cost, teams)
native zp_register_zombie_class(const name[], const info[], const model[], const clawmodel[], hp, speed, Float:gravity, Float:knockback)
native zp_get_extra_item_id(const name[])
native zp_get_zombie_class_id(const name[])
native zp_get_zombie_class_info(classid, info[], len)
forward zp_round_started(gamemode, id)
forward zp_round_ended(winteam)
forward zp_user_infected_pre(id, infector, nemesis)
forward zp_user_infected_post(id, infector, nemesis)
forward zp_user_humanized_pre(id, survivor)
forward zp_user_humanized_post(id, survivor)
forward zp_user_infect_attempt(id, infector, nemesis)
forward zp_user_humanize_attempt(id, survivor)
forward zp_extra_item_selected(id, itemid)
forward zp_user_unfrozen(id)
forward zp_user_last_zombie(id)
forward zp_user_last_human(id)
#define ZP_TEAM_ANY 0