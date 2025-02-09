/*
Bancfg Util 1.01
	Hidden part of ExtremeBan
*/

#if defined _banconfig_util_included
    #endinput
#endif

#define _banconfig_util_included

#pragma library "BanConfigUtil"

/*
MaxLens for buffers
*/
#define REASON_LEN 63
#define CFGID_LEN 45

/*
* Time Format
*/
stock const TIMEFORMAT[] = "%Y-%m-%d %H:%M:%S";


enum Report{
	DONE,
	NOT_FOUND,
	BAD_ADMINID
}

/**
 * Initialize bancfg database. It will set sqlx driver and create table for data.
 *
 * @param tuple			connection tuple
 * @param szPrefix		prefix for SQL table
 * @return 			1 on success, 0 on fail
 */
native bancfg_init(Handle:tuple, const szPrefix[]="");


/**
 * Set player bancfg status by cfgid. To unban set both start and stop time to 0.
 *
 * @param id			admin`s id
 * @param cfgid			player`s cfgid
 * @param starttime		start of ban, get_systime()
 * @param stoptime		expire date, get_systime()+BANTIME
 * @param szAdminName		admin`s name
 * @param szReason		reason of ban
 * @return			1 on success, 0 on fail
 */
native Float:bancfg_ban(id, const cfgid[], starttime, stoptime, const szAdminName[], const szReason[]);


/**
 * Shortcut for unban
 *
 * @param id			admin`s id
 * @param cfgid			player`s cfgid
 * @return			1 on success, 0 on fail
 */
stock Float:bancfg_unban(id, const cfgid[])
	return bancfg_ban(id, cfgid, 0,0, "","");
	
/**
 * BancfgUtil throws this forward when receive and validate player`s cfgid.
 *
 * @param cfgid			player`s cfgid
 * @param starttime		start of ban, get_systime()
 * @param stoptime		expire date, get_systime()+BANTIME
 * @param szAdminName		admin`s name
 * @param szReason		reason of ban
 */

forward bancfg_authorized(id, const cfgid[], starttime, stoptime, const szAdmin[], const szReason[]);



/**
 * BancfgUtil throws this forward after bancfg query
 *
 * @param id			admin`s id
 * @param fTime			time of start operation
 * @param status		report status
 */
forward bancfg_reporting(id, Float:fTime, Report:status);


stock bool:isUserBanned(starttime, stoptime){
	new iNow = get_systime();
	return (starttime>1000 && (starttime <= iNow) && ((stoptime > iNow) || stoptime < 1000));
}
stock SQL_PrepareString(const szQuery[], szOutPut[], size){
	copy(szOutPut, size, szQuery);
	replace_all(szOutPut, size, "'", "/'");
	replace_all(szOutPut,size, "`", "/`");
	
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
