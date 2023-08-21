#include "shell.h"

/**
 * _myenv - prints the current environment
 * @param: The parameters Struct
 *
 * Return: Always 0
 */
int _myenv(param_t *param)
{
	print_list_str(param->env);
	return (0);
}


/**
 * _getenv - gets the value of an environment variable
 * @param: The parameters Struct
 * @name: env var name
 *
 * Return: the value of an environment variable
 */

char *_getenv(param_t *param, const char *name)
{
	list_t *node = param->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - initialize a new environment variable,
 *		or modify an existing one
 * @param: The parameters Struct
 *
 * Return: Always 0
 */

int _mysetenv(param_t *param)
{
	if (param->argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenv(param, param->argv[1], param->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Remove an environment variable,
 * @param: The parameters Struct
 *
 * Return: Always 0
 */

int _myunsetenv(param_t *param)
{
	int i;

	if (param->argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i <= param->argc; i++)
		_unsetenv(param, param->argv[i]);

	return (0);

}

/**
 * populate_env_list - populates env linked list
 * @param: The parameters Struct
 *
 * Return: Always 0
 */

int populate_env_list(param_t *param)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);

	param->env = node;
	return (0);
}
