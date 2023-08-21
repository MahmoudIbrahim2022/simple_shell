#include "shell.h"

/**
 * get_history_file - gets the history file
 * @param: The parameters Struct
 *
 * Return: allocated string containing history file
 */
char *get_history_file(param_t *param)
{
	char *buf, *dir;

	dir = _getenv(param, "HOME=");

	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);

	return (buf);
}

/**
 * write_history - writes history from  cmd history list to history file.
 * @param: The parameters Struct
 *
 * Return: 1 on success, else -1
 */

int write_history(param_t *param)
{
	ssize_t fd;
	char *filename = get_history_file(param);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);

	if (fd == -1)
		return (-1);

	for (node = param->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads history from history file to cmd history list
 * @param: The parameters Struct
 *
 * Return: histcount on success, 0 otherwise.
 */
int read_history(param_t *param)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(param);

	if (!filename)
	return (0);

	fd = open(filename, O_RDONLY);
	free(filename);

	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_history_list(param, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(param, buf + last, linecount++);
	free(buf);
	param->histcount = linecount;
	while (param->histcount-- >= HIST_MAX)
		delete_node_at_index(&(param->history), 0);
	renumber_history(param);
	return (param->histcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * @param: The parameters Struct
 * @buf: buffer that contains the entry
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0.
 */
int build_history_list(param_t *param, char *buf, int linecount)
{
	list_t *node = NULL;

	if (param->history)
		node = param->history;
	add_node_end(&node, buf, linecount);

	if (!param->history)
		param->history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @param: The parameters Struct
 *
 * Return: the new histcount
 */

int renumber_history(param_t *param)
{
	list_t *node = param->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (param->histcount = i);
}
