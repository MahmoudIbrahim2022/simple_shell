#include "shell.h"

/**
 * _myexit - exits the shell
 * @param: The parameters Struct
 *
 * Return: exits with a given exit status
 *		(0) if param.argv[0] != "exit
 */

int _myexit(param_t *param)
{
	int exitcheck;

	if (param->argv[1]) /* if there is an exit argument */
	{
		exitcheck = _erratoi(param->argv[1]);
		if (exitcheck == -1) /* convert error */
		{
			param->status = 2;
			print_error(param, "Illegal number: ");
			_eputs(param->argv[1]);
			_eputchar('\n');
			return (1);
		}
		param->err_num = _erratoi(param->argv[1]);
		return (-2);
	}
	param->err_num = -1;
	return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @param: The parameters Struct
 *
 * Return: Always 0
 */

int _mycd(param_t *param)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!param->argv[1])
	{
		dir = _getenv(param, "Home=");
		if (!dir)
			chdir_ret = chdir((dir = _getenv(param, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(param->argv[1], "-") == 0)
	{
		if (!_getenv(param, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(param, "OLDPWD=")), _putchar('\n');
		chdir_ret = chdir((dir = _getenv(param, "OLDPWD=")) ? dir : "/");
	}

	else
		chdir_ret = chdir(param->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(param, "can't cd to ");
		_eputs(param->argv[1]), _putchar('\n');
	}
	else
	{
		_setenv(param, "OLDPWD", _getenv(param, "PWD="));
		_setenv(param, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @param: The parameters Struct
 *
 * Return: Always 0
 */
int _myhelp(param_t *param)
{
	char **arg_array;

	arg_array = param->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
