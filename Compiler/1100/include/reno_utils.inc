/**
 * Logger levels enum
 */
enum LogLevel { Trace, Debug, Info, Warning, Error, Fatal }
/**
 * Logs a message with the specified log level.
 *
 * This function logs a message with the given log level along with a timestamp.
 *
 * @param level     The log level (e.g., Info, Warning, Error).
 * @param message   The message to log. You can use format specifiers as in printf.
 * @param ...       Additional parameters for message formatting.
 */
stock _RENO_Logger(const LogLevel: level = Info, const message[], any: ...) {
    static buffer[4096]
    vformat(buffer, sizeof buffer, message, 3)

    if (level == Fatal) {
        format(buffer, sizeof buffer, "%s", buffer)
        set_fail_state("%s", buffer)

        return
    }

    static const logLevelString[LogLevel][] = { "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" }
    format(buffer, sizeof buffer, "[%.2f][%s] %s", get_gametime(), logLevelString[level], buffer)

    server_print("%s", buffer)
}

/**
 * Logs information to a file with a timestamp.
 *
 * @param szFile The name of the file to log to.
 * @param szInfo The information to be logged.
 */
stock _RENO_LogInfoToFilee(const szFile[], const szInfo[])
{
    // Get the current date
    new y, m, d;
    date(y, m, d);

    // Log the information to the file with a timestamp
    log_to_file( fmt("%s__%d_%02d_%02d.log", szFile, y, m, d), szInfo);
}


// Extend ConVar flags.
enum _: ConVar_ExtendedFlags (<<= 1) {
    _FCVAR_BOOLEAN = (FCVAR_NOEXTRAWHITEPACE << 1), /**< Can contain boolean value */
    _FCVAR_STRING,                                  /**< Can contain string value */
    _FCVAR_FLOAT,                                   /**< Can contain float value */
    _FCVAR_INTEGER,                                 /**< Can contain integer value */
}

stock _RENO_GetCvarTypeStr(const /* ConVar_ExtendedFlags: */ flags) {
    new buffer[32]

    if (flags & _FCVAR_BOOLEAN)
        copy(buffer, charsmax(buffer), "boolean")

    if (flags & _FCVAR_STRING)
        copy(buffer, charsmax(buffer), "string")

    if (flags & _FCVAR_FLOAT)
        copy(buffer, charsmax(buffer), "float")
    
    if (flags & _FCVAR_INTEGER)
        copy(buffer, charsmax(buffer), "integer")
    
    return buffer
}

/**
 * Parses a color buffer into an array of RGBA values.
 *
 * This function parses a color buffer (e.g., "255 0 0 128") into an array of RGBA values.
 *
 * @param buffer    The color buffer to parse.
 * @return          Returns an array of RGBA values [R, G, B, A].
 */
stock _RENO_ParseColorsStrToArray(const buffer[]) {
    new stringParsed[4][4]
    new colors[4]

    if (!strlen(buffer))
        return colors
    
    new parsedArgs = parse(
        buffer,
        stringParsed[0], charsmax(stringParsed[]),  // R
        stringParsed[1], charsmax(stringParsed[]),  // G
        stringParsed[2], charsmax(stringParsed[]),  // B
        stringParsed[3], charsmax(stringParsed[])   // A
    )

    if (parsedArgs < 3)
        return colors

    for (new i; i < sizeof(stringParsed); i++)
        colors[i] = strtol(stringParsed[i])

    return colors
}

/**
 * Checks if the current map's name is compatible with a list of prefix blocklists.
 *
 * This function compares the current map's name with a list of prefixes specified in `list`.
 * It returns true if the map's name does not contain any of the specified prefixes, and false otherwise.
 *
 * @param list      An array of strings representing the prefixes that should not be present in the map's name.
 * @param size      The size of the `list` array.
 *
 * @return                  Returns true if the current map's name is compatible with the prefix blocklist,
 *                          meaning it does not contain any of the specified prefixes.
 *                          Returns false if the current map's name contains any of the specified prefixes.
 */
stock bool: _RENO_IsMapCompatible(const list[][], const size = sizeof(list)) {
    new mapName[32]
    get_mapname(mapName, charsmax(mapName))

    for (new i; i < size; i++) {
        if (containi(mapName, list[i]) == -1)
            continue
        
        return false
    }

    return true
}

/**
 * Checks if the current map's name is incompatible with a list item.
 *
 * This function compares the current map's name with a list of prefixes specified in `list`.
 * It returns true if the map's name contains any of the specified prefixes, and false otherwise.
 *
 * @param list      An array of strings representing the prefixes that should not be present in the map's name.
 * @param size      The size of the `list` array.
 *
 * @return          Returns true if the current map's name is incompatible with the prefix blocklist,
 *                  meaning it contains any of the specified prefixes.
 *                  Returns false if the current map's name does not contain any of the specified prefixes.
 */
stock bool: _RENO_IsMapIncompatible(const list[][], const size = sizeof(list)) {
    return !_RENO_IsMapCompatible(list, size);
}

/**
 * Verifies if a map name matches an input. Supports wildcard '*' for partial matches.
 *
 * @param input     Input string, '*' for partial matches.
 * @param mapName   Map name to check.
 * @return          true if matches, else false.
 */
stock bool:_RENO_MatchMapName(const input[], const mapName[])
{
	if (containi(input, "*") != -1)
	{
		new temp[98];
		copy(temp, sizeof(temp) - 1, input);
		replace(temp, sizeof(temp) - 1, "*", "");
		return bool:(containi(mapName, temp) != -1);
	}
	else
		return bool:equali(input, mapName);
}

/**
 * Checks if a player index is valid.
 *
 * This functions checks if a player index is valid (between range of 1 and maxClients)
 *
 * @param index    The player id to be checked.
 */
stock bool:_RENO_IsPlayerIndexValid(const index)
{
    return (index >= 1 && index <= MaxClients)
}

/**
 * Checks if a player is mocked (not a real player).
 *
 * This functions checks if a player is mocked by checking if the player is BOT, HLTV or FakeCLIENT.
 *
 * @param index    The player id to be checked.
 */
stock bool:_RENO_IsUserMocked(const index)
{
    if (is_user_bot(index) || is_user_hltv(index) || pev(index, pev_flags) & FL_FAKECLIENT)
    {
        return true;
    }

    return false;
}

/**
 * Plays a sound effect for a player with specified volume and pitch.
 *
 * This function plays a sound effect for a player with the specified volume and pitch.
 *
 * @param player    The player to play the sound effect to.
 * @param sound     The name of the sound file to play.
 * @param vol       The volume of the sound (default is 0.7).
 * @param pitch     The pitch of the sound (default is PITCH_NORM).
 */
stock _RENO_PlaySoundEffect(const player, const sound[], const Float: vol = 0.7, const pitch = PITCH_NORM, const Float: attn = ATTN_NORM, const flags = 0) {
    emit_sound(
        player,
        CHAN_VOICE,
        sound,
        .vol = vol,
        .pitch = pitch,
        .att = attn,
        .flags = flags
    )
}

stock _RENO_LieFlat(ent)
{
	// If the entity is not on the ground, don't bother continuing.
	if (pev(ent, pev_flags) & ~FL_ONGROUND) return
	
	// I decided to make all the variables static; suprisingly, the touch function can be called upwards of 5 times per drop.
	// I dunno why, but I suspect it's because the item "skips" on the ground.
	static Float:origin[3], Float:traceto[3], trace = 0, Float:fraction, Float:angles[3], Float:angles2[3]
	
	pev(ent, pev_origin, origin)
	pev(ent, pev_angles, angles)
	
	// We want to trace downwards 10 units.
	xs_vec_sub(origin, Float:{0.0, 0.0, 10.0}, traceto)
	
	engfunc(EngFunc_TraceLine, origin, traceto, IGNORE_MONSTERS, ent, trace)
	
	// Most likely if the entity has the FL_ONGROUND flag, flFraction will be less than 1.0, but we need to make sure.
	get_tr2(trace, TR_flFraction, fraction)
	if (fraction == 1.0) return
	
	// Normally, once an item is dropped, the X and Y-axis rotations (aka roll and pitch) are set to 0, making them lie "flat."
	// We find the forward vector: the direction the ent is facing before we mess with its angles.
	static Float:original_forward[3]
	angle_vector(angles, ANGLEVECTOR_FORWARD, original_forward)
	
	// If your head was an entity, no matter which direction you face, these vectors would be sticking out of your right ear,
	// up out the top of your head, and forward out from your nose.
	static Float:right[3], Float:up[3], Float:fwd[3]
	
	// The plane's normal line will be our new ANGLEVECTOR_UP.
	get_tr2(trace, TR_vecPlaneNormal, up)
	
	// This checks to see if the ground is flat. If it is, don't bother continuing.
	// Disable this if you use weaponphysics (Fix by Syturio)
	// if (up[2] == 1.0) return
	
	// The cross product (aka vector product) will give us a vector, which is in essence our ANGLEVECTOR_RIGHT.
	xs_vec_cross(original_forward, up, right)
	// And this cross product will give us our new ANGLEVECTOR_FORWARD.
	xs_vec_cross(up, right, fwd)
	
	// Converts from the forward vector to angles. Unfortunately, vectors don't provide enough info to determine X-axis rotation (roll),
	// so we have to find it by pretending our right anglevector is a forward, calculating the angles, and pulling the corresponding value
	// that would be the roll.
	vector_to_angle(fwd, angles)
	vector_to_angle(right, angles2)
	
	// Multiply by -1 because pitch increases as we look down.
	angles[2] = -1.0 * angles2[0]
	
	// Finally, we turn our entity to lie flat.
	set_pev(ent, pev_angles, angles)
}