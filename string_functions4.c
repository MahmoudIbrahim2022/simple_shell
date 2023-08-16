#include "shell.h"

/**
 * **strtow - Divides a string into words, ignoring repeated delimiters.
 * @str: Input string.
 * @d: Delimiter string.
 * Return: Returns a pointer to an array of strings, or
 * NULL if unsuccessful.
 */
char **strtow(char *str, char *d)
{
	int i, j, k, m, nbrwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!d)
		d = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!is_delim(str[i], d) && (is_delim(str[i + 1], d) || !str[i + 1]))
			nbrwords++;
	if (nbrwords == 0)
		return (NULL);
	s = malloc((1 + nbrwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < nbrwords; j++)
	{
		while (is_delim(str[i], d))
			i++;
		k = 0;
		while (!is_delim(str[i + k], d) && str[i + k])
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = str[i++];
		s[j][m] = 0;
	}
	s[j] = NULL;

	return (s);
}

/**
 * **strtow2 - Divides a string into words.
 * @str: Input string.
 * @d: Delimiter.
 * Return: Returns a pointer to an array of strings, or NULL if unsuccessful.
 */
char **strtow2(char *str, char d)
{
	int i, j, k, m, nbrwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != d && str[i + 1] == d) ||
				    (str[i] != d && !str[i + 1]) || str[i + 1] == d)
			nbrwords++;
	if (nbrwords == 0)
		return (NULL);
	s = malloc((1 + nbrwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < nbrwords; j++)
	{
		while (str[i] == d && str[i] != d)
			i++;
		k = 0;
		while (str[i + k] != d && str[i + k] && str[i + k] != d)
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = str[i++];
		s[j][m] = 0;
	}
	s[j] = NULL;

	return (s);
}
