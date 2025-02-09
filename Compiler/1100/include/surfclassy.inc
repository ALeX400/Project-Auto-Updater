#if defined _surf_included
  #endinput
#endif
#define _surf_included


/**
 * Get user points
 *
 */
native surf_get_user_points(index);

/**
 * Set user points
 *
 */
native surf_set_user_points(index, amount);

/**
 * Add user points
 *
 */
native surf_add_user_points(index, amount);

/**
 * Delete user points
 *
 */
native surf_del_user_points(index, amount);