#if defined _util_included
	#endinput
#endif
#define _util_included

#if !defined _amxmodx_included
	#include <amxmodx>
#endif

/**
 * Processes a generic target pattern and tries to match it to a client based
 * on filtering flags. 
 * The specific message from cmd_target() function will not displayed
 *
 * @note Default setting will appear get_target(id, arg)
 *       Default will pass over the immunity of admins, the flag is set automatically
 * @note The function will not be able to execute on bots
 *
 * @note The pattern is first matched case insensitively against client names.
 *       If no match is found it is matched against client authids. If still no
 *       match is found and the pattern starts with '#' it is finally matched
 *       against client userids.
 * @note Since client names are matched by substring the pattern can potentially
 *       match multiple targets. In that case the function will return 0 and ask
 *       the admin to provide a unique pattern.
 * @note The filtering flags are applied after the pattern matching has
 *       finished. That means the pattern has to be unique against all clients
 *       on the server even if some of them are not eligible.
 *
 * @param id        Client index of admin performing an action
 * @param arg       Target pattern
 * @param flags     Filtering flags, see CMDTARGET_* constants above
 *
 * @return          Client index, or 0 if no or multiple clients matched
 */
stock get_target(id, const arg[], flags = (1<<1)|(1<<3))
{
	new player = find_player("bl", arg);
	if (player)
	{
		if (player != find_player("blj", arg))
		{
			return 0;
		}
	}
	else if ((player = find_player("c", arg)) == 0 && arg[0] == '#' && arg[1])
	{
		player = find_player("k", str_to_num(arg[1]));
	}
	if (!player)
	{
		return 0;
	}
	if (flags & (1<<0))
	{
		if ((get_user_flags(player) & (1<<0)) && ((flags & (1<<1)) ? (id != player) : true))
		{
			return 0;
		}
	}
	if (flags & (1<<2))
	{
		if (!is_user_alive(player))
		{
			return 0;
		}
	}
	if (flags & (1<<3))
	{
		if (is_user_bot(player))
		{
			return 0;
		}
	}
	return player;
}
/**
 * Returns if the client has admin flags set without slot reservation
 * Return only true admins with access on commands
 *
 * @param id            Client index
 * @param accesssilent  if is 1 no denied access message will be printed
 *                      if is 0 no printed message
 *                      default value is 0
 * @return              1 if client has admin flags, 0 otherwise
 */
stock is_true_admin(id, accesssilent = 0)
{
	new has_access = 0;
	new __flags = get_user_flags(id);

	// check any access without slot reservation and ADMIN_USER flag "z"
	if(__flags & read_flags("acdefghijklmnopqrstuvwxy"))
	{
		has_access = 1;
	}
	else if(id == (is_dedicated_server() ? 0 : 1)) // Check if the cmd is executed by server
	{
		has_access = 1;
	}

	if(has_access == 0)
	{
		if (accesssilent == 1)
		{
			console_print(id, "You have no access to that command");
		}
	}
	return (__flags > 0 && has_access);
}