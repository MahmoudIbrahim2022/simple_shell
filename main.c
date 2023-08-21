#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg values
 *
 * Return: 0 on success, 1 on error
 */

int main(int ac, char **av)
{
	param_t param[] = { PARAM_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r"  (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENONET)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open");
				_eputs(av[1]);
				_eputchar(BUF_FLUSH);
					exit(127);
			}
			return (EXIT_FAILURE);
		}
		param->readfd = fd;
	}

	populate_env_list(param);
	read_history(param);
	shell_loop(param, av);

	return (EXIT_SUCCESS);
}
