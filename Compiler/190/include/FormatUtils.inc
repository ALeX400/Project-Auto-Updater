#if defined _formatutils_included
	#endinput
#endif
#define _formatutils_included

#include <amxmodx>
#include <amxmisc>

// Global max lengths for categories and values.
new global_maxlen_category;
new global_maxlen;

/**
 * Updates a value in a specified array based on a given category.
 * Searches for the category in the array and updates the corresponding value.
 *
 * @param categories  Array of categories.
 * @param values      Array of values linked to categories.
 * @param size        Size of the arrays.
 * @param key         The category key to search for.
 * @param format      String format for the new value.
 * @param ...         Additional parameters for string formatting.
 */
stock UpdateValue(const categories[][], const values[][], size, const key[], const format[], any:...)
{
	new Buffer[192];
	vformat(Buffer, sizeof(Buffer) - 1, format, 5);

	for (new i = 0; i < size; i++)
	{
		if (equal(categories[i], key))
		{
			format(values[i], sizeof(values[]) - 1, "%s", Buffer);
			break;
		}
	}
}

/**
 * Calculates the maximum lengths for categories and values from given arrays.
 * Updates global variables for max lengths.
 *
 * @param Array1      First array (usually categories).
 * @param Array2      Second array (usually values).
 * @param numEntries  Number of entries in the arrays.
 */
stock CalculateMaxLens(const Array1[][], const Array2[][], numEntries)
{
	global_maxlen_category = 0;
	global_maxlen = 0;

	for (new i = 0; i < numEntries; i++)
	{
		global_maxlen_category = max(global_maxlen_category, strlen(Array1[i]));
		global_maxlen = max(global_maxlen, strlen(Array2[i]));
	}

	global_maxlen_category += 2;
	global_maxlen += 2;
}

/**
 * Resets the global max lengths for categories and values.
 */
stock ResetMaxLens()
{
	global_maxlen_category = 0;
	global_maxlen = 0;
}

/**
 * Formats a string by combining text from two different categories and values.
 *
 * @param text1   First text (category).
 * @param text2   Second text (value).
 * @param output  Output buffer for the formatted string.
 * @param len     Length of the output buffer.
 */
stock FormatString(text1[], text2[], output[], len)
{
	new part1[128];
	new part2[128];

	FormatString1(text1, part1, sizeof(part1) - 1);
	FormatString2(text2, part2, sizeof(part2) - 1);

	format(output, len, "%s%s", part1, part2);
}

/**
 * Formats the first part of the string (usually the category).
 *
 * @param text    Text to format.
 * @param output  Output buffer for the formatted text.
 * @param len     Length of the output buffer.
 */
stock FormatString1(text[], output[], len)
{
	new space = global_maxlen_category - strlen(text);
	new formatted[128] = "";

	for (new i = 0; i < space; i++)
		strcat(formatted, " ", sizeof(formatted) - 1);

	format(output, len, "[%s%s: ", formatted, text);
}

/**
 * Formats the second part of the string (usually the value).
 *
 * @param text    Text to format.
 * @param output  Output buffer for the formatted text.
 * @param len     Length of the output buffer.
 */
stock FormatString2(text[], output[], len)
{
	new space = global_maxlen - strlen(text);
	new formatted[128] = "";

	for (new i = 0; i < space; i++)
		strcat(formatted, " ", sizeof(formatted) - 1);

	format(output, len, "%s%s]", text, formatted);
}

/**
 * Resets a specified array. Sets each entry to an empty string.
 *
 * @param array   The array to reset.
 */
stock ResetArray(array[][])
{
	for (new i = 0; i < sizeof array; i++)
	{
		array[i][0] = '^0';
	}
}

/**
 * Adds a new category to an array.
 * Searches for a free position in the array and adds the category.
 *
 * @param array       The array to add the category to.
 * @param category    The category to add.
 */
stock AddCategory(array[][], const category[])
{
	for (new i = 0; i < sizeof array; i++)
	{
		if (array[i][0] == '^0')
		{
			format(array[i], sizeof(array[]) - 1, category);
			break;
		}
	}
}
