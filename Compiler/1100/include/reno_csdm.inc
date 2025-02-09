
#if defined _reno_csdm_included
	#endinput
#endif
#define _reno_csdm_included

// AMXX 1.9.0 compatibility
#if !defined MAX_MAPNAME_LENGTH
    #define MAX_MAPNAME_LENGTH 64
#endif

/**
 * Returns whether CSDM is active.
 * 
 * @return      True if CSDM is active, false otherwise.
 */
native bool: csdm_is_active()

/**
 * Sets CSDM to active (do not set the cvar!).
 * 
 * @param active    True to set CSDM active, false to deactivate it.
 * @return          True if CSDM is successfully set to active or inactive, false otherwise.
 */
native bool: csdm_set_active(const bool: active)

/**
 * Retrieves the main plugin's configuration as a JSON handle.
 * 
 * @return      Config JSON handle. Returns Invalid_JSON if an error occurs.
 */
native JSON: csdm_get_config()

/**
 * Retrieves the equipment configuration from the main plugin as a JSON handle.
 * 
 * @return      Equip config JSON handle. Returns Invalid_JSON if an error occurs.
 */
native JSON: csdm_get_equip_config()

/**
 * Gets the number of registered spawner styles.
 * 
 * @return      The number of registered spawner styles.
 */
native csdm_spawnstyles()

/**
 * Retrieves spawn style information by index.
 * 
 * @param style_index   Style index.
 * @param name          Buffer to copy the name.
 * @param maxlength     Maximum size of the buffer.
 * 
 * @error               Throws an error if an invalid handle is provided.
 * 
 * @return              The number of characters copied into the 'name' buffer.
 */
native csdm_styleinfo(const style_index, name[], const length)

/**
 * Adds a spawn style handler with the specified name and function.
 */
native csdm_addstyle(const name[], const function[])

/**
 * Sets the current spawn style handler by name.
 * The handler registered to this name will be called after every spawn.
 */
native csdm_setstyle(const name[])

/**
 * Returns the current style ID.
 * 
 * @return      The ID of the current spawn style.
 */
native csdm_curstyle()

/**
 * Called before initialization starts.
 */
forward CSDM_ChangeState(const bool: active)

/**
 * Called before initialization starts.
 */
forward CSDM_InitStart()

/**
 * Called after full initialization.
 */
forward CSDM_Initialized()

/**
 * Forward for retrieving the config name.
 * 
 * @param config    Buffer to copy the config name.
 * @param len       Maximum length of the buffer.
 */
forward CSDM_GetConfigName(config[], const len)

/**
 * Forward for retrieving the config prefix name.
 * 
 * @param config    Buffer to copy the config prefix name.
 * @param len       Maximum length of the buffer.
 * @param mapPrefix Buffer containing the map prefix.
 */
forward CSDM_GetConfigPrefixName(config[], const len, const mapPrefix[])
