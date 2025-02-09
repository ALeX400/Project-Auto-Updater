/*
 * An Custom Utils library - a small library of custom utility functions
 * Author: @LeX
 */

#if defined _utils_included
	#endinput
#endif
#define _utils_included

#if !defined _amxmodx_included
	#include <amxmodx>
#endif

#if !defined _string_stocks_included
	#include <string_stocks>
#endif

stock remove_string(const buffer[], const left[], const right[])
{
	new left_len = strlen(left);
	new right_len = strlen(right);
	new buffer_len = strlen(buffer);
	
	new start = 0;
	if (strncmp(buffer, left, left_len) == 0)
		start = left_len;
	
	new end = buffer_len;
	if (buffer_len >= right_len && strncmp(buffer[buffer_len - right_len], right, right_len) == 0)
		end -= right_len;
	
	new result[128];
	new j = 0;
	for (new i = start; i < end && j < sizeof(result) - 1; i++, j++)
		result[j] = buffer[i];

	result[j] = '^0';
	
	return result;
}

/**
 * Replace the buffer with the source buffer
 *
 * @param dest      The destination buffer
 * @param source    The source buffer
 *
 * @noreturn
 */ 
stock replace_buffer(dest[], const source[])
{
	new i = 0
	
	while(source[i] != '^0')
	{
		dest[i] = source[i]
		i++
	}
	
	dest[i] = '^0'

	// optional: clear the rest of the buffer
	while(dest[++i] != '^0')
		dest[i] = '^0'
}
