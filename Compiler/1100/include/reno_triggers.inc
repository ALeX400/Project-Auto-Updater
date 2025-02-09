#if defined _reno_triggers_included
    #endinput
#endif

#define _reno_triggers_included

/**
 * Registers trigger commands with chat prefixes by passing 2 arguments (1: trigger | 2: callback)
 *
 */
#define RENO_RegisterTriggerCmd(%0,%1) \
	for (new iter = 0; iter < sizeof(BASE_CHAT_TRIGGERS); iter++) \
	{ \
		register_clcmd(fmt("say %s%s", BASE_CHAT_TRIGGERS[iter], %0), %1); \
		register_clcmd(fmt("say_team %s%s", BASE_CHAT_TRIGGERS[iter], %0), %1); \
	}

/**
 * Base chat triggers used for command prefixes.
 */
new stock const BASE_CHAT_TRIGGERS[][] = { "", "/", "\", "!", "." };