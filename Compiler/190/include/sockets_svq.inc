#include <amxmodx>
#include <sockets>

#define MAX_INFO_LEN 1400
#define MAX_INFO_FORMAT 100

// A2S_INFO definitions for source according to http://developer.valvesoftware.com/wiki/Server_Queries#Source_servers_2
#define A2S_INFO "^xFF^xFF^xFF^xFF^x54Source Engine Query^x00"
#define A2S_INFO_LEN 25

#define A2S_INFO_SOURCE_REPLY_FORMAT "411ssss21111111s" // there are some extra flags after this but we don't care

enum {
	A2S_INFO_SOURCE_IDX_HEADER=1, // Should be FF FF FF FF
	A2S_INFO_SOURCE_IDX_TYPE, // Should be equal to 'I' (0x49)
	A2S_INFO_SOURCE_IDX_VERSION, // Network version. 0x07 is the current Steam version.
	A2S_INFO_SOURCE_IDX_SERVERNAME, // The Source server's name
	A2S_INFO_SOURCE_IDX_MAP,  // The current map being played "
	A2S_INFO_SOURCE_IDX_GAMEDIR, // Game files folder, eg: "cstrike"
	A2S_INFO_SOURCE_IDX_GAMEDESC, // Game type, eg: "Counter-Strike"
	A2S_INFO_SOURCE_IDX_APPID, // Steam Application ID, see http://developer.valvesoftware.com/wiki/Steam_Application_IDs
	A2S_INFO_SOURCE_IDX_NUMPLAYERS, // Currently players on the server
	A2S_INFO_SOURCE_IDX_MAXPLAYERS, // Max players for the server
	A2S_INFO_SOURCE_IDX_NUMBOTS, // Number of bot players on the server
	A2S_INFO_SOURCE_IDX_DEDICATED, // 'l' listen, 'd' dedicated, 'p' SourceTV
	A2S_INFO_SOURCE_IDX_OS, // Host OS. 'l' for Linux, 'w' for Windows
	A2S_INFO_SOURCE_IDX_PASSWORD, // If set to 0x01, a password is required to join this server
	A2S_INFO_SOURCE_IDX_SECURE, // if set to 0x01, this server is VAC secured
	A2S_INFO_SOURCE_IDX_GAMEVERSION // The version of the game, eg: "1.0.0.22"
}

#define QUERY_TIMEOUT 0.1
#define TASK_SOCKET 145

static socket_id;

// Dinamic Array to fix	
static functions[100][32];
static funcid = 0;

enum {
	SOCKET=0,
	INFO,
	ID,
	FUNC
};

stock socket_info_servername (host[], port, const cb[])
{
	socket_info_request(host, port, cb, A2S_INFO_SOURCE_IDX_SERVERNAME);
}

stock socket_info_map (host[], port, const cb[])
{
	socket_info_request(host, port, cb, A2S_INFO_SOURCE_IDX_MAP);
}

stock socket_info_numplayers (host[], port, const cb[])
{
	socket_info_request(host, port, cb, A2S_INFO_SOURCE_IDX_NUMPLAYERS);
}

stock socket_info_maxplayers (host[], port, const cb[])
{
	socket_info_request(host, port, cb, A2S_INFO_SOURCE_IDX_MAXPLAYERS);
}

static socket_info_request (host[], port, const cb[], info)
{
	static args[4];

	args[SOCKET] = socket_create(host, port);
	args[INFO] = info;
	args[ID] = TASK_SOCKET + socket_id;

	formatex(functions[funcid], charsmax(functions[]), "%s", cb);

	args[FUNC] = funcid;

	if (funcid == charsmax(functions))
		funcid = 0;
	else
		funcid++;
	
	socket_send2(args[SOCKET], A2S_INFO, A2S_INFO_LEN);

	set_task(QUERY_TIMEOUT, "socket_check", args[ID], args, sizeof(args), "b");
}

static socket_create (host[], port)
{
	static socket, error;

	socket = socket_open(host, port, SOCKET_UDP, error);
	socket_id++;

	if(socket < 0 || error)
		return 0;

	return socket;
}

public socket_check(args[])
{
	static socket, info, id;
	static data[MAX_INFO_LEN], len, indexes[MAX_INFO_FORMAT];

	socket = args[SOCKET];
	info = args[INFO];
	id = args[ID];

	if (!socket_is_readable(socket, 100))
		return;

	remove_task(id);

	len = socket_recv(socket, data, charsmax(data));

	index_create(data, len, A2S_INFO_SOURCE_REPLY_FORMAT, indexes);

	new strval[321], numval;
	numval = -1;

	switch (info) {
		case A2S_INFO_SOURCE_IDX_SERVERNAME:
			copyc(strval, charsmax(strval), data[indexes[info]], 0);
		case A2S_INFO_SOURCE_IDX_MAP:
			copyc(strval, charsmax(strval), data[indexes[info]], 0);
		case A2S_INFO_SOURCE_IDX_NUMPLAYERS:
			numval = index_get_byte(data, indexes[info]);
		case A2S_INFO_SOURCE_IDX_MAXPLAYERS:
			numval = index_get_byte(data, indexes[info]);
	}

	new func[64];
	format(func, charsmax(func), "%s", functions[args[FUNC]]);

	//server_print("Getting Function: %s Index: %i", func, args[FUNC]);

	if (numval != -1)
		set_task(0.1, func, numval, _, _, "a", 1);
	else if (!equali(strval, ""))
		set_task(0.1, func, _, strval, sizeof(strval), "a", 1);

	socket_close(socket);
}

stock index_get_byte (data[MAX_INFO_LEN], index)
{
	return data[index];
}

stock index_create(data[MAX_INFO_LEN], len, formatStr[100], indexes[MAX_INFO_FORMAT])
{
	new nFormatPos = 0 // current position within the format array
	new nIndexPos = 0 // current position within the data array
	new nDataIndex = 0 // current chracter index within the data stream
	new nFormatPosMax = strlen(formatStr)

	while ((nIndexPos < nFormatPosMax) && (nDataIndex <= len)) {
		switch (formatStr[nFormatPos]) {
			case '1': { // "byte"
				indexes[nIndexPos] = nDataIndex

				nDataIndex++
				nIndexPos++
			}
			case '2': { // "short"
				indexes[nIndexPos] = nDataIndex

				nDataIndex += 2
				nIndexPos++
			}
			case '4': { // "long"
				indexes[nIndexPos] = nDataIndex

				nDataIndex += 4
				nIndexPos++
			}
			case 's': { // string
				indexes[nIndexPos] = nDataIndex

				do { nDataIndex++; }
				while ((data[nDataIndex] != 0) && (nDataIndex < len)) // find the end of the string by searching a 0 character

				nDataIndex++
				nIndexPos++
			}
			case '[': { // byte switch and start of optional formats
				if (data[nDataIndex] != 1) { // skip options 
					do { nFormatPos++; }
					while ((formatStr[nFormatPos] != ']') && (nFormatPos < nFormatPosMax))
				} else {
					nDataIndex++
					nIndexPos++
				}
			}
			case ']': { // end of optional formats
				nDataIndex++
			}
			default: {
				nDataIndex++
			}
		}

		nFormatPos++
	}

	return nIndexPos;
}