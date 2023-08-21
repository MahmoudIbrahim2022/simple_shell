#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @param: The parameters Struct
 *
 * Return: Always 0
 */

char **get_environ(param_t *param)
{
	if (!param->environ || param->env_changed)
	{
		param->environ = list_to_strings(param->env);
		param->env_changed = 0;
	}

	return (param->environ);
}

/**
 * _unsetenv - returns the string array copy of our environ
 * @param: The parameters Struct
 * @var: the string env var property
 *
 * Return: 1 on delete, 0 otherwise
 */

int _unsetenv(param_t *param, char *var)
{
	list_t *node = param->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			param->env_changed = delete_node_at_index(&(param->env), i);
			i = 0;
			node = param->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (param->env_changed);
}

/**
 * _setenv - Initializes a new environment variable
 *		or modify an existing one
 * @param: The parameters Struct
 * @var: the string env var property
 * @value: the string env var value
 *
 * Return: Always 0
 */

int _setenv(param_t *param, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = param->env;

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			param->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(param->env), buf, 0);
	free(buf);
	param->env_changed = 1;
	return (0);
}

