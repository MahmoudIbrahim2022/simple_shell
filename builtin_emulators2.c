#include "shell.h"

/**
 * _myhistory - displays the history list
 * @param: The parameters Struct
 *
 * Return: Always 0
 */

int _myhistory(param_t *param)
{
	print_list(param->history);
	return (0);
}

/**
 * unset_alias - remove alias
 * @param: The parameters Struct
 * @str: The string alias
 *
 * Return: Always 0 on success, 1 on error
 */

int unset_alias(param_t *param, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(param->alias),
		get_node_index(param->alias, node_starts_with(param->alias, str, -1)));
	*p = c;
	return (ret);
}


/**
 * set_alias - sets alias to string
 * @param: The parameters Struct
 * @str: The string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(param_t *param, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(param, str));

	unset_alias(param, str);
	return (add_node_end(&(param->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: The alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}


/**
 * _myalias - mimics the alias builtin (man alias)
 * @param: The parameters Struct
 *
 * Return: Always 0.
 */

int _myalias(param_t *param)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (param->argc == 1)
	{
		node = param->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; param->argv[i]; i++)
	{
		p = _strchr(param->argv[i], '=');
		if (p)
			set_alias(param, param->argv[i]);

		else
			print_alias(node_starts_with(param->alias, param->argv[i], '='));
	}

	return (0);
}
