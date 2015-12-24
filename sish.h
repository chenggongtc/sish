/* 
 * sish.h	- header file for global variable and function forward 
 *		  declarations
 *
 * sish		- a simple shell
 *
 * Members:
 *		- Gong Cheng,	gcheng2@stevens.edu
 *		- Maisi Li,	mli27@stevens.edu
 *
 * Usage:	- ./sish [-x][-c command]
 */
#ifndef _SISH_H_
#define _SISH_H_

#include <sys/types.h>

#include <limits.h>

extern int cflag;
extern int xflag;
extern char *ccmd;	/* command for -c option */
extern int ret_val;
extern char *progname;
extern pid_t processid;

#define MAX_ARGC 64		/* max number of arguments for one command */
#define MAX_ARG_LEN PATH_MAX	/* max length of each argument */
#define MAX_CMD 20		/* max number of commands */
#define MAX_CMD_LEN 2048	/* max length of each command */
#define MAX_LINE_LEN 4096	/* max length of input commands */
#define DEFAULT_STATUS 127	/* default return value */

/* struct that stores the information of a command */
typedef struct {
	char input[PATH_MAX];	/* < */
	char output[PATH_MAX];	/* > */
	int append;		/* >> */
	char *arg[MAX_ARGC];	/* arguments, including command */
} COMMAND;

/* loop.c */
int execute();

/* utils.c */
int syntax_check(char *, int *);
int command_parser(char *, char [MAX_CMD][MAX_CMD_LEN]);
void command_init(COMMAND *);
int argument_parser(char *, char [MAX_ARGC][MAX_ARG_LEN], COMMAND *);

/* builtin.c */
int exit_syntax_check(char *[MAX_ARGC]);
int cd_syntax_check(char *[MAX_ARGC]);
int cd_exec(char *[MAX_ARGC]);
void echo_exec(char *[MAX_ARGC]);
void exit_usage();
void cd_usage();

#endif
