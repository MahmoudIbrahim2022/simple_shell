#ifndef SHELL_HEADER
#define SHELL_HEADER


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM   0
#define CMD_OR     1
#define CMD_AND    2
#define CMD_CHAIN  3


/* for convert_number() */
#define CONVERT_LOWERCASE  1
#define CONVERT_UNSIGNED   2


/* 1 if using system getline() */
#define USE_GETLINE 0

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX   4096

extern char **environ;


/**
 *struct liststr - contains a builtin string and related function
 *@num: the number field
 *@str: a string
 *@next: points to the next node
 */

typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;


/**
 *struct passparam - contains pseudo-arguments to pass into a function,
 * allowing uniform prototype for function pointer struct
 *
 *@arg: A string generated from getline containing arguments
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program file name
 *@env: Linked List local copy of environ
 *@environ: custom modified copy of environ from ll env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on iif environ was changed
 *@status: the return status of the last executed command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: The history line number count
 */

typedef struct passparam
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;


	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory management */
	int cmd_buf_type; /*CMD_type ||, &&, ; */
	int readfd;
	int histcount;

} param_t;


#define PARAM_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *
 *@type: the builtin command flag
 *@func: the function
 */

typedef struct builtin
{
	char *cmd;
	int (*func)(param_t *);

} builtin_cmd_table;



/* hsh.c */
int shell_loop(param_t *, char **);
int find_builtin(param_t *);
void find_external(param_t *);
void fork_external(param_t *);

/* path.c */
int is_cmd(param_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(param_t *, char *, char *);

/* error_functions.c */
void _eputs(char *c);
int _eputchar(char c);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* string_functions1.c */
int _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *starts_with(const char *haystack, const char *needle);
char *_strcat(char *dest, char *src);

/* string_functions2.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* string_functions3.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* memory_functions.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);
int bfree(void **);


/* more_functions1.c */
int interactive(param_t *);
int is_delim(char, char *);
int _isalpha(int);

/* more_functions2.c */
int _erratoi(char *);
void print_error(param_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* builtin_emulators1.c */
int _myexit(param_t *);
int _mycd(param_t *);

/* builtin_emulators2.c */
int _myhistory(param_t *);
int _myalias(param_t *);


/* getline.c module */
ssize_t get_input(param_t *);
int _getline(param_t *, char **, size_t *);
void sigintHandler(int);

/* param.c module */
void clear_param(param_t *);
void set_param(param_t *, char **);
void free_param(param_t *, int);

/* env1.c module */
char *_getenv(param_t *, const char *);
int _myenv(param_t *);
int _mysetenv(param_t *);
int _myunsetenv(param_t *);
int populate_env_list(param_t *);

/* env2.c module */
char **get_environ(param_t *);
int _unsetenv(param_t *, char *);
int _setenv(param_t *, char *, char *);

/* history_file.c */
char *get_history_file(param_t *param);
int write_history(param_t *param);
int read_history(param_t *param);
int build_history_list(param_t *param, char *buf, int linecount);
int renumber_history(param_t *param);

/* liststr1.c module */
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* liststr2.c module */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* chain.c */
int is_chain(param_t *, char *, size_t *);
void check_chain(param_t *, char *, size_t *, size_t, size_t);
int replace_alias(param_t *);
int replace_vars(param_t *);
int replace_string(char **, char *);


#endif
