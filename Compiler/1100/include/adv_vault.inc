#if defined _adv_vault_included
  #endinput
#endif
#define _adv_vault_included


/*
Comment to remove the features you do not use.
(You must be the same as this in the plugin of the API)
*/
#define COMPILE_FIND
#define COMPILE_SORT
#define COMPILE_SIMPLE


/**
[Advanced Vault System 1.5 by Destro]
 */

/*Type of data*/
enum {
    DATATYPE_INT=0,
    DATATYPE_STRING,
    DATATYPE_ARRAY
}

enum (<<= 1) {
    CLEAR_ALL=1,
    CLEAR_DATA,
    CLEAR_INDEX,
    #if defined COMPILE_SIMPLE
    CLEAR_SIMPLEDATA,
    #endif
}

#if defined COMPILE_FIND
enum (<<= 1) {
    FINDFLAGS_EQUAL=1,
    FINDFLAGS_CONTAIN,
    FINDFLAGS_CASE_SENSITIVE,
    FINDFLAGS_LESS,
    FINDFLAGS_GREATER,
    FINDFLAGS_NOT,
    FINDFLAGS_AND,
    FINDFLAGS_OR
}
#endif

#if defined COMPILE_SORT
/*Descending/ascending order*/
enum {
    ORDER_DESC=0,
    ORDER_ASC
}
#endif

enum {
    SIZE_DATA=0,
    SIZE_INDEX,
    #if defined COMPILE_SIMPLE
    SIZE_SIMPLEDATA,
    #endif
}

/**
 * Check if the Vault is open
 * 
 * @param Vaultname         Vault name
 *
 * @return                  Returns the Vault Index
 */
native adv_vault_is_open(const vaultname[]);


/**
 * Open/Create a Vault
 *
 * @param Vaultname         Vault name
 * @param Cache_index       Key Key's in Memory
 *                          (recommended in mass use)
 *
 * @return                  Returns the Vault Index
 */
native adv_vault_open(const vaultname[], cache_index=false);


/**
 * Initialize the Vault
 *
 * @param Vault             Vault Index
 *
 * @noreturn
 */
native adv_vault_init(vault);


/**
 * Close a Vault
 *
 * @param Vault             Vault Index
 *
 * @noreturn
 */
native adv_vault_closed(vault);


/**
 * Clean the vault
 * 
 * @param Vault             Vault Index
 *
 * @noreturn
 */
native adv_vault_clear(vault, flags);


/**
 * Returns the total amount of records
 *
 * @param Vault             Vault Index
 * @param Type              Size_data, size_index or size_simpledata
 *
 * @noreturn
 */
native adv_vault_size(vault, type=SIZE_DATA);

/**
 * Add a field to Vault
 *
 * @param Vault             Vault Index
 * @param Fieldname         Field Name
 * @param Type              type of data that will be kept
 * @param Length            Reserved size to save
 *                          (only for datatype_string and datatype_array)
 *
 * @return                  Returns the Index of the field (Fieldindex)
 */
native adv_vault_register_field(vault, const fieldname[], type=DATATYPE_INT, length=0);


/**
 * Look for the index associated with Keyname
 *
 * @param Vault             Vault Index
 *
 * @return                  returns 0 if the keyindex was not found,
 *                          otherwise returns Keyindex
 */
native adv_vault_get_keyindex(vault, const keyname[]);


/**
 * Look for Keyname associated with Keyindex
 *
 * @param Vault             Vault Index
 * @param Output            Output variable
 * @param Len               Output size
 *
 * @return                  Returns 1 if the keyname was found, otherwise 0
 */
native adv_vault_get_keyname(vault, keyindex, output[], len);


/**
 * Prepares data obtaining
 *
 * @param Vault             Vault Index
 * @param Keyindex          The index of a key (recommended)
 * @param Keyname           Key (internally looking for the index of Key)
 *
 * @return                  Returns 1 if there is data, otherwise 0
 */
native adv_vault_get_prepare(vault, keyindex=0, const keyname[]="");


/**
 * Obtain the data from a field
 *
 * @param Vault             Vault Index
 * @param Fieldindex        Index of the field
 * @param Output            Output variable
 * @param Len               Output size
 *
 * @return                  If it's datatype_int returns the value saved
 */
native adv_vault_get_field(vault, fieldindex, output[]="", len=0);
    
    
/**
 * Prepare everything to modify the fields
 *
 * @param Vault             Vault Index
 *
 * @noreturn
 */
native adv_vault_set_start(vault);


/**
 * Modifies the field
 * 
 * @param Vault             Vault Index
 * @param Fieldindex        Index of the field
 * @param Value             Datatype_int: a number value
 *                          Datatype_string: a normal string or to format ( %s %d ...)
 *                          Datatype_array: an array
 *
 * @noreturn
 */
native adv_vault_set_field(vault, fieldindex, any:...);


/**
 * Update/insert modified fields
 *
 * @param Vault             Vault Index
 * @param Keyindex          The index of a key (recommended)
 * @param Keyname           Key (internally looking for the index of Key)
 * @note                    Use Keyindex 0 and Keyname "" to insert a new data
 *                          With an auto-incredible index
 *
 * @return                  Returns the Keyindex
*/
native adv_vault_set_end(vault, keyindex=0, const keyname[]="");


/**
 * Remove a key
 *
 * @param Vault             Vault Index
 * @param Keyindex          The index of a key (recommended)
 * @param Keyname           Key (internally looking for the index of Key)
 * @note                    If Keyname is used, the key to the index and data is removed
 *                          If Keyindex is used, only the data is removed
 *
 * @noreturn
 */
native adv_vault_removekey(vault, keyindex=0, const keyname[]="");


/**
 * The Forward is called when a Vault is closed
 *
 * @note                    New for: Save the data when closing the Vault
 */
forward fw_adv_vault_closed(vault);


/**
 * The Forward is called by initializing the Vault
 *
 * @note                     New for: register new fields in other plugins
 */
forward fw_adv_vault_init(vault, vaultname[]);


#if defined COMPILE_FIND
/**               [Search]               */

/**
 * Start a search
 *
 * @param Vault             Vault Index
 * @param Any               (Field, Flags, Value)
 *                          *Field: The Index of the Field to Compare
 *                          *Value: The value with which to compare
 *                          *Frags: Comparison sentences
 */
native adv_vault_find_start(vault, any:...);

/**
 * Look for the following result
 *
 * @param Vault             Vault Index
 * @noreturn
 */
native adv_vault_find_next(vault);

/**
 * The search end
 *
 * @param Vault             Vault Index
 * @noreturn
 */
native adv_vault_find_closed(vault);
#endif

#if defined COMPILE_SORT
/**             [ Ordering ]            */

/**
 * Create an order
 *
 * @param Vault             Vault Index
 * @param order             THE ORDER (ORDER_DESC/ORDER_ASC)
 * @param Refresh           time to update the order (0 to deactivate)
 * @param Maximit           Maximum Limit of Results (Unlimited 0)
 * @param any               The int int that will be used to order (maximum 4)
 *
 * @note                    Use Keyindex 0 and Keyname "" to insert a new data
 *                          Returns the Sort Index
 *
 * @return                  Returns the Keyindex
 */
native adv_vault_sort_create(vault, order, refresh, maxlimit, any:...);


/**
 * Update an order
 *
 * @param Vault             Vault Index
 * @param Sort              Index of ordering
 *
 * @noreturn
 */
native adv_vault_sort_update(vault, sort);


/**
 * Destroy an order
 *
 * @param Vault             Vault Index
 * @param Sort              Index of ordering
 *
 * @noreturn
 */
native adv_vault_sort_destroy(vault, sort);


/**
 * Returns the position of a key
 *
 * @param Vault             Vault Index
 * @param Sort              Index of ordering
 * @param Keyindex          The index of a key (recommended)
 * @param Keyname           Key (internally looking for the index of Key)
 *
 * @return                  Returns the position of the key in the order, 
 *                          if the key is not found returns 0
 */
native adv_vault_sort_key(vault, sort, keyindex, const keyname[]="");


/**
 * Returns the indexkey of a position
 *
 * @param Vault             Vault Index
 * @param Sort              Index of ordering
 * @param Position          the potion (from 1 to results)
 * 
 * @return                  Returns the indexkey of a position
 */
native adv_vault_sort_position(vault, sort, position);


/**
 * Returns the number of results
 *
 * @param Vault             Vault Index
 * @param Sort              Index of ordering
 *
 * @return                  Returns the number of results
 */
native adv_vault_sort_numresult(vault, sort);


/**
 * The Forward is called when the Sort is updated
 */
forward fw_adv_vault_sort_update(vault, sort);
#endif

#if defined COMPILE_SIMPLE
/**                 [ Simple Vault ]          */

// Get the data from a key
native adv_vault_simple_get(vault, const key[], output[], len);

// Save the data in a key
native adv_vault_simple_set(vault, const key[], const data[], any:...);

// Remove a key
native adv_vault_simple_removekey(vault, const key[]);
#endif 