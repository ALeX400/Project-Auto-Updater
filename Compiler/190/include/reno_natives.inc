#if defined _reno_natives_included
	#endinput
#endif
#define _reno_natives_included


native bool:RENO_IsWarmupInProgress();
native RENO_GetUserDataCheckInterval();
native bool:RENO_IsUserMocked(id);
native bool:RENO_IsSaveOnPlayedTimeChangeActive();
native RENO_GetDefaultPrefix(Prefix[], len);