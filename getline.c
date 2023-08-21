#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @param: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */

ssize_t input_buf(param_t *param, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(param, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			param->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(param, *buf, param->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain ? */
			{
				*len = r;
				param->cmd_buf = buf;
			}
		}
	}
	return (r);
}


/**
 * get_input - gets a line minus the newline
 * @param: parameter struct
 *
 * Return: bytes Read
 */

ssize_t get_input(param_t *param)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(param->arg), *p;

	_putchar(BUF_FLUSH);
	r = input_buf(param, &buf, &len);

	if (r == -1) /* EOF */
	return (-1);

	if (len)
	{
		j = i;
		p = buf + i;

		check_chain(param, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(param, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';' ' */
		if (i >= len) /* reached end of buffer ? */
		{
			i = len = 0; /* reset position and length */
			param->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
		}

		*buf_p = buf; /* else not a chain, pass back buffer from _getline() */
		return (r); /* return length of buffer from _getline() */
}

/**
 * read_buf - reads a buffer
 * @param: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: bytes Read
 */

ssize_t read_buf(param_t *param, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(param->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - gets the next line of input from STDN
 * @param: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: bytes Read
 */


int _getline(param_t *param, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(param, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int) (c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);

	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void.
 */

void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);

}
