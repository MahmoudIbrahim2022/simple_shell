#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @param: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */

int is_chain(param_t *param, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		param->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		param->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		param->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @param: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: void
 */

void check_chain(param_t *param, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (param->cmd_buf_type == CMD_AND)
	{
		if (param->status) /* is non-zero indicating an error */
		{
			buf[i] = 0;
			j = len;
		}
	}

	if (param->cmd_buf_type == CMD_OR)
	{
		if (!param->status)
		{
			buf[i] = 0;
			j = len;
		}
	}


	*p = j;

}

/**
 * replace_alias - replaces an aliases in the tokenized string
 * @param: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */

int replace_alias(param_t *param) /* show=ls -l */
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(param->alias, param->argv[0], '=');
		if (!node)
			return (0);
		free(param->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		param->argv[0] = p;
	}

	return (1);
}


/**
 * replace_vars - replaces an vars in the tokenized string
 * @param: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */

int replace_vars(param_t *param) /* name=sadat echo "first : $name" */
{
	int i;
	list_t *node;

	for (i = 0; param->argv[i] ; i++)
	{
		if (param->argv[i][0] != '$' || !param->argv[i][1])
		continue;

		if (!_strcmp(param->argv[i], "$?")) /* Exit Status of Last Command */
		{
			replace_string(&(param->argv[i]),
				_strdup(convert_number(param->status, 10, 0)));
			continue;
		}

		if (!_strcmp(param->argv[i], "$$")) /* Process ID */
		{
			replace_string(&(param->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}

		node = node_starts_with(param->env, &param->argv[i][1], '=');
		if (node)
		{
			replace_string(&(param->argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}

		replace_string(&param->argv[i], _strdup(""));
	}
	return (0);
}

/**
 * replace_string - replaces a string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */

int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
