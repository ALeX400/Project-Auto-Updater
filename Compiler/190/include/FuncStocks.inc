#if defined _funcstocks_included
	#endinput
#endif
#define _funcstocks_included

#if !defined _amxmodx_included
	#include <amxmodx>
#endif

// Preluare 
stock str_to_array(Message[], array[][])
{
	new buffer[321];
	format(buffer, charsmax(buffer), "%s", Message)

	if(!Message[0])
	{
		server_print("[Error] Error get data from string, the buffer must not be empty");
		return 1
	}
	
	new totalcount = 0
	new charcount
	charcount = CharsInString(buffer, ',' , ' ')

	for(new i = 0; i < charcount+1; i++)
	{
		parse(buffer, array[i], 32);
		trim(buffer)

		if(!array[i][0])
			continue;

		replace_stringex(buffer, charsmax(buffer), array[i], "")
		trim(buffer)

		if(!ValidMap(array[i])) continue;

		totalcount++;
	}
	return totalcount
}

stock CharsInString(string[], character, replace)
{
	new total = 0;
	for( new i; i<strlen(string); i++ )
	{
		if( string[i] == character )
		{
			string[i]=replace;
			total++;
		}
		if( !string[i] )
			break;
	}
	return total;
}

stock bool:ValidMap(mapname[])
{
	if ( is_map_valid(mapname) )
	{
		return true;
	}
	// If the is_map_valid check failed, check the end of the string
	new len = strlen(mapname) - 4;
	
	// The mapname was too short to possibly house the .bsp extension
	if (len < 0)
	{
		return false;
	}
	if ( equali(mapname[len], ".bsp") )
	{
		// If the ending was .bsp, then cut it off.
		// the string is byref'ed, so this copies back to the loaded text.
		mapname[len] = '^0';
		
		// recheck
		if ( is_map_valid(mapname) )
		{
			return true;
		}
	}
	
	return false;
}

stock ErrorLog(param, Message[], any:...)
{
	new FuncName[192];
	if(param > 2)
		param = 2;

	if(param == 0)
		format(FuncName, charsmax(FuncName), "[ Func Map Changer ]");
	
	if(param == 1)
		format(FuncName, charsmax(FuncName), "[ Func MvP System ]");

	if(param == 2)
		format(FuncName, charsmax(FuncName), "[ Func CS: Competitive Mode ]");


	new LogMessage[2001];
	vformat(LogMessage, charsmax(LogMessage), Message, 3);
	new cfgdir[64], errFile[192], errFolder[128];

	get_localinfo("amxx_configsdir", cfgdir, charsmax(cfgdir));
	//get_configsdir(cfgdir, charsmax(cfgdir));
	format(errFolder, charsmax(errFolder), "%s/Errors", cfgdir);

	if(!dir_exists(errFolder))
		mkdir(errFolder);

	new Data[101], Time[101];
	get_time("%d.%m.%Y", Data, charsmax(Data));
	get_time("%H:%M:%S ", Time, charsmax(Time));
	
	format(errFile, charsmax(errFile), "%s/Error_%s.log", errFolder, Data);

	new MapName[64];
	get_mapname(MapName, charsmax(MapName));
	
	new FormInfo[32];
	format(FormInfo, charsmax(FormInfo), "| %s | %s |", Time, MapName);
	format(LogMessage, charsmax(LogMessage), "%s %s %s", FormInfo, FuncName, LogMessage);
	write_file(errFile, LogMessage, -1);
	replace(LogMessage, charsmax(LogMessage), FormInfo, "");

	format(LogMessage,charsmax(LogMessage),"[ Error ] %s", LogMessage);
	server_print(LogMessage);
}

stock CheckLicence()
{
	new rValue = 0;
	new IP[22];
	new hostname[192];
	get_cvar_string("hostname", hostname, charsmax(hostname));
	strtolower(hostname); // preventiv

	get_user_ip (0, IP , charsmax(IP) , 1 );
	//set_task(1.0, "DelayCheckLicence");
	
	//if(containi(IP, "51.195.97.142") !=-1)
	if(containi(IP, fmt("%c%c%c%c%c%c%c%c%c%c%c%c%c",53,49,46,49,57,53,46,57,55,46,49,52,50)) !=-1)
	{
		rValue = 1
	}
	else
	{
		rValue = 0;
	}

	return rValue;
}