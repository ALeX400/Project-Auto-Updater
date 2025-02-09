native RegisterCommand( const szName[ ], const szCallBack[ ], const iFlags, const iImmunity )
native AddNewUser( const szID[ ], const szPassword[ ], const szFlags[ ], const szPrefix[ ], const szSkinT[ ], const szSkinCT[ ], const iImmunity, const szExpireDate[ ], szExpireFlags[ ], bool:bKeepPrivileges )

forward AdminAdded( const szName[ ], const szPassword[ ], const szFlags[ ], const szPrefix[ ], const szModel_T[ ], const szModel_CT[ ], const iImmunity, const szExpireDate[ ], szExpireFlags[ ], bool:bKeepPrivileges )