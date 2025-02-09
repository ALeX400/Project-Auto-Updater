#if defined _zombie_plague_vip_included
    #endinput
#endif

#define _zombie_plague_vip_included
#define ZV_PLUGIN_HANDLED 97

#define ZV_MAIN (1<<0)
#define ZV_MULTI (1<<1)
#define ZV_UNLIMITED (1<<2)
#define ZV_DAMAGE (1<<3)
#define ZV_TEAM_ZOMBIE (1<<0)
#define ZV_TEAM_HUMAN (1<<1)
#define ZV_TEAM_NEMESIS (1<<2)
#define ZV_TEAM_SURVIVOR (1<<3)


native zv_register_extra_item(const name[], const discription[], cost, team)
native zv_get_user_flags(id)
forward zv_extra_item_selected(id, itemid)