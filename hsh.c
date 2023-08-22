#include "shell.h"

/**
 * interactive - returns true if shell is interactive mode
 * @param: struct address
 * Return: 1 if interactive mode, 0 otherwise
 */
int interactive(param_t *param)
{
	return (isatty(STDIN_FILENO) && param->readfd <= 2);
}

/**
 * shell_loop - main shell loop
 * @param: The parameters Struct
 * @av: argument values
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shell_loop(param_t *param, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_param(param);
		if (interactive(param))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_input(param);
		if (r != -1)
		{
			set_param(param, av);
			builtin_ret = find_builtin(param);
			if (builtin_ret == -1)
				find_external(param);
		}
		else if (interactive(param))
			_putchar('\n');
		free_param(param, 0);
	}
	write_history(param);
	free_param(param, 1);

	if (!interactive(param) && param->status)
		exit(param->status);

	if (builtin_ret == -2)
	{
		if (param->err_num == -1)
			exit(param->status);
		exit(param->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @param: The parameters Struct
 *
 * Return: -1 if builtin not found
 *	    0 if builtin executed successfully,
 *	    0 if builtin found but not successful
 *         -2 if builtin signals exit()
 */
int find_builtin(param_t *param)
{
	int i, built_in_ret = -1;
	builtin_cmd_table cmd_table[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; cmd_table[i].cmd; i++)
	{
		if (_strcmp(param->argv[0], cmd_table[i].cmd) == 0)
		{
			param->line_count++;
			built_in_ret = cmd_table[i].func(param);
			break;
		}
	}

		return (built_in_ret);
}

/**
 * find_external - finds an external command (command in path)
 * @param: The parameters Struct
 *
 * Return: void
 */

void find_external(param_t *param)
{
	char *path = NULL;
	int i, k;

	param->path = param->argv[0];
	if (param->linecount_flag == 1)
	{
		param->line_count++;
		param->linecount_flag = 0;
	}
	for (i = 0, k = 0; param->arg[i]; i++)
		if (!is_delim(param->arg[i], "\t\n"))
		k++;
	if (!k)
		return;
	path = find_path(param, _getenv(param, "PATH="), param->argv[0]);

	if (path)
	{
		param->path = path;
		fork_external(param);
	}
	else
	{
		if ((interactive(param) || _getenv(param, "PATH=")
		|| param->argv[0][0] == '/') && is_cmd(param, param->argv[0]))
		fork_external(param);

		else if (*(param->arg) != '\n')
		{
			param->status = 127;
			print_error(param, "notfound\n");
		}
	}
}

/**
 * fork_external - creates a child process to run external command
 * @param: The parameters Struct
 *
 * Return: void
 */

void fork_external(param_t *param)
{
	pid_t child_pid;

	child_pid = fork();

	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error");
		return;
	}

	if (child_pid == 0)
	{
		if (execve(param->path, param->argv, get_environ(param)) == -1)
		{
			free_param(param, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}

	else
	{
		wait(&(param->status));

		if (WIFEXITED(param->status))
		{
			param->status = WEXITSTATUS(param->status);
			if (param->status == 126)
				print_error(param, "Permission denied\n");
		}
	}
}
