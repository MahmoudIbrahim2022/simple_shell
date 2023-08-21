#include "shell.h"

/**
 * clear_param - clear param_t struct
 * @param: The parameters Struct
 *
 * Return: void
 */

void clear_param(param_t *param)
{
	param->arg = NULL;
	param->argv = NULL;
	param->path = NULL;
	param->argc = 0;

}


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
 * set_param - initializes param_t struct
 * @param: The parameters Struct
 * @av: argument vector
 *
 * Return: void
 */
void set_param(param_t *param, char **av)
{
	int i = 0;

	param->fname = av[0];
	if (param->arg)
	{
		param->argv = strtow(param->arg, " \t");
		if (!param->argv)
		{
			param->argv = malloc(sizeof(char *) * 2);
			if (param->argv)
			{
				param->argv[0] = _strdup(param->arg);
				param->argv[1] = NULL;
			}
		}
		for (i = 0; param->argv && param->argv[i]; i++)
			;
		param->argc = i;

		replace_alias(param);
		replace_vars(param);
	}
}


/**
 * free_param - frees param_t struct fields
 * @param: The parameters Struct
 * @all: true if freeing all fields
 *
 * Return: void
 */

void free_param(param_t *param, int all)
{
	ffree(param->argv);
	param->argv = NULL;
	param->path = NULL;

	if (all)
	{
		if (!param->cmd_buf)
			free(param->arg);
		if (param->env)
			free_list(&(param->env));
		if (param->history)
			free_list(&(param->history));
		if (param->alias)
			free_list(&(param->alias));

		ffree(param->environ);
			param->environ = NULL;
		bfree((void **)param->cmd_buf);

		if (param->readfd > 2)
			close(param->readfd);
		_putchar(BUF_FLUSH);

	}

}
