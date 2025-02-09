//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ NATIVES ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
native RENO_GetConnectAddress(buffer[], len);
native RENO_GetForumAddress(buffer[], len);
native RENO_GetServerMode(buffer[], len);
native RENO_GetPublicKeyword(buffer[], len);
native RENO_GetDefaultPrefix(buffer[], len);
native RENO_GetInfoPrefix(buffer[], len);
native RENO_GetWarningPrefix(buffer[], len);
native RENO_GetVipPrefix(buffer[], len);
native RENO_GetPlayerSkinTero(buffer[], len);
native RENO_GetPlayerSkinCt(buffer[], len);
native RENO_GetContactPersons(buffer[], len);
native RENO_GetAdblockProtectedFlags(buffer[], len);
native RENO_GetUserDataCheckInterval();
native RENO_GetUserDataVaultName(buffer[], len);
native RENO_GetUserDataVaultType(buffer[], len);
native RENO_GetUserDataVaultSaveType(buffer[], len);

native RENO_GetFeatCustomizeAccessId();
native RENO_GetFeatResetScoreAccessId();
native RENO_GetFeatRefillAmmoAccessId();
native RENO_GetFeatSilentStepsAccessId();
native RENO_GetFeatKillEffectsAccessId();
native RENO_GetFeatHeadshotOnlyAccessId();
native RENO_GetFeatParachuteAccessId();
native RENO_GetFeatShortNadesAccessId();
native RENO_GetFeatNoFallDmgAccessId();
native RENO_GetFeatDisplayDmgAccessId();
native RENO_GetFeatIncreaseDmgAccessId();
native RENO_GetFeatReloadSpeedAccessId();	
native RENO_GetFeatMultiJumpsAccessId();
native RENO_GetFeatKnifeSpeedAccessId();
native RENO_GetFeatHealthBonusAccessId();
native RENO_GetFeatSpawnSuppliesAccessId();
native RENO_GetFeatSoundsAccessId();
native RENO_GetFeatNadesTrailAccessId();
native RENO_GetFeatPlayerSkinsAccessId();

native Float:RENO_GetFeatIncreaseDmgUnits(classId);
native Float:RENO_GetFeatReloadSpeedUnits(classId);
native Float:RENO_GetFeatKnifeSpeedUnits(classId);
native RENO_GetFeatMultiJumpsMaxLoad(classId);
native RENO_GetFeatHealthBonusOnKill(classId);
native RENO_GetFeatHealthBonusOnHS(classId);
native RENO_GetFeatHealthBonusMaxLoad(classId);
native RENO_GetFeatSpawnSupplyArmorUnits(classId);
native RENO_GetFeatSpawnSupplyHeUnits(classId);
native RENO_GetFeatSpawnSupplyFlashUnits(classId);
native RENO_GetFeatSpawnSupplySmokeUnits(classId);
native bool:RENO_GetFeatSpawnSupplyDefuseStatus(classId);

native RENO_GetFeatHappyHourStatusId();
native RENO_GetFeatHappyHourStart();

native RENO_GetChatSymbolsToHide(buffer[], len);
native RENO_GetChatMinCharsRequired();
native RENO_GetChatMaxCharsAllowed();
native RENO_GetChatMaxRepsAllowed();
native RENO_GetChatMaxWarnsAllowed();
native RENO_GetNameMaxChangesAllowed();
native RENO_GetVipWeaponsMenuInitRound();
native RENO_GetRandomName_UNSAFE(buffer[], len);

native RENO_SetUserFreeze(index);
native RENO_SetUserUnfreeze(index);
native RENO_SetUserChangeNickEligibility(index, type);

native Float:RENO_GetChatFloodTime();
native Float:RENO_GetChatCooldownTime();

native bool:RENO_GetUserChangeNickEligibility(index);
native bool:RENO_IsSaveOnPlayedTimeChangeActive();
native bool:RENO_IsPickupWpnSoundBlocked();
native bool:RENO_IsAdminsPrivacyActive();
native bool:RENO_IsAdvertSkippedForVips();
native bool:RENO_IsWarmupInProgress();
native bool:RENO_IsAdblockChatActive();
native bool:RENO_IsAdblockNamesActive();
native bool:RENO_IsChatCaptchaActive();
native bool:RENO_IsCaptchaCodeActive();
native bool:RENO_IsVipWeaponsMenuActive();
native bool:RENO_IsPreserveVipsDuelsActive();
native bool:RENO_IsUserAdBlockProtected(index);
native bool:RENO_IsUserVip(index);
native bool:RENO_IsUserStrictVip(index);
native bool:RENO_IsUserFrozen(index);
native bool:RENO_IsUserMocked(index);
native bool:RENO_IsModeFFA();
native bool:RENO_IsServerModeCSDM();
native bool:RENO_IsServerModeClassic();
native bool:RENO_IsServerModeGungame();
native bool:RENO_HasUserCleanName(name[]);
native bool:RENO_HasUserCleanMessage(message[]);
native bool:RENO_HasUserHiddenMessage(message[]);
native bool:RENO_HasUserAdminLowestAccess(index);

// native RENO_GetUserXP(id);
// native RENO_SetUserXP(id, amount);
native RENO_GetUserPoints(id);
native RENO_SetUserPoints(id, amount);
native RENO_GetUserMvps(id);
native RENO_SetUserMvps(id, amount);
native RENO_IncreaseUserMvps(id);
native RENO_GetUserTimePlayed(id);
native RENO_SetUserTimePlayed(id, amount);
native RENO_GetUserTimeSpec(id);
native RENO_SetUserTimeSpec(id, amount);
native RENO_ForceSaveUserData(id);

native RENO_IsHudDamagerEditable();
native RENO_IsHudKillFadeEditable();
native RENO_IsHudKillsNotifierEditable();
native RENO_IsHudWeaponIconEditable();
native RENO_IsSoundOnKillEditable();
native RENO_IsSoundResetScoreEditable();
native RENO_IsSoundSessionStartEditable();
native RENO_IsSoundSessionWinnerEditable();
native RENO_IsHeadshotOnlyEditable();

/*
native RENO_IsUserHudDamageActive(id);
native RENO_GetUserHudDamageOutcome(id);
native RENO_GetUserHudDamageStyle(id);
native RENO_GetUserHudDamageColor(id);
native RENO_IsUserHudDamageLostActive(id);
native RENO_IsUserHudKillFadeActive(id);
native RENO_GetUserHudKillFadeColor(id);
native RENO_GetUserHudKillFadeDensity(id);
native RENO_IsUserHudKillsNotifierActive(id);
native RENO_IsUserKillSoundActive(id);
native RENO_IsUserResetScoreSoundActive(id);
native RENO_IsUserSessionStartSoundActive(id);
native RENO_IsUserSessionWinnerSoundActive(id);
native RENO_IsUserHeadshotOnlyActive(id);
*/

native RENO_GetSessionLeaderId();
