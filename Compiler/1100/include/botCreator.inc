#if defined _botcreator_included
	#endinput 
#endif
#define _botcreator_included


#if !defined _amxmodx_included
	#include <amxmodx>
#endif

#if !defined _amxmisc_included
	#include <amxmisc>
#endif

#if !defined _fakemeta_included
	#include <fakemeta>
#endif
#if !defined _cstrike_included
	#include <cstrike>
#endif


new bool:is_user_fake_bot[33]
new botCount = 0

stock AddBot(numBot)
{
	for (new i = 0; i < numBot; i++)
	{
		createBot(i + 1);
	}
}

stock RemoveBots()
{
	for(new id = 1; id <= get_maxplayers(); id++) 
	{
		if (is_user_fake_bot[id])
		{
			server_print("    DEBUG >> Remove Entity [ %n ]", id)
			server_cmd("wait;kick #%d;wait",get_user_userid(id))
			is_user_fake_bot[id] = false
			botCount--
		}
	}
}

stock RemoveBot(num)
{
	new botName[32];
	format(botName, charsmax(botName), "Bot [%d]", num);

	new id = get_user_index(botName)

	if (is_user_fake_bot[id])
	{
		server_print("    DEBUG >> Remove Entity [ %n ]", id)
		server_cmd("wait;kick #%d;wait",get_user_userid(id))
		is_user_fake_bot[id] = false;
		botCount--;
	}
}

stock createBot(botNumber)
{
	new BotName[32]
	format(BotName, charsmax(BotName), "Bot [%d]", botNumber)

	new Bot = engfunc(EngFunc_CreateFakeClient, BotName)

	if (Bot > 0)
	{
		//Supposed to prevent crashes?
		dllfunc(MetaFunc_CallGameEntity, "player", Bot)
		set_pev(Bot, pev_flags, FL_FAKECLIENT)

		//Make Sure they have no model
		set_pev(Bot, pev_model, "")
		set_pev(Bot, pev_viewmodel2, "")
		set_pev(Bot, pev_modelindex, 0)

		//Make them invisible for good measure
		set_pev(Bot, pev_renderfx, kRenderFxNone)
		set_pev(Bot, pev_rendermode, kRenderTransAlpha)
		set_pev(Bot, pev_renderamt, 0.0)

		//Set the team if we need to for this mod
		set_team(Bot)

		is_user_fake_bot[Bot] = true
		botCount++
	}
}

stock set_team(BotID)
{
	new randomTeam = random_num(1, 2);
	cs_set_user_team(BotID, randomTeam);
}

stock bool:is_player_fake_bot(id)
{
	return is_user_fake_bot[id];
}