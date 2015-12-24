/* 
 * builtin.c	- functions for builtin commands
 *
 * sish		- a simple shell
 *
 * Members:
 *		- Gong Cheng,	gcheng2@stevens.edu
 *		- Maisi Li,	mli27@stevens.edu
 *
 * Usage:	- ./sish [-x][-c command]
 */
#include <sys/types.h>

#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sish.h"

/* return 0 on success, -1 on error */
int
exit_syntax_check(char *arg[MAX_ARGC])
{
	if (arg[1] != NULL)
		return -1;
	else
		return 0;
}

/* return 0 on success, -1 on error */
int
cd_syntax_check(char *arg[MAX_ARGC])
{
	if (arg[1] == NULL || arg[2] == NULL)
		return 0;
	else
		return -1;
}

/* return EXIT_FAILURE on error, EXIT_SUCCESS on success */
int
cd_exec(char *arg[MAX_ARGC])
{
	uid_t uid;
	struct passwd *usr;
	char path[PATH_MAX];
	if (arg[1] == NULL) {	/* home directory */
		uid = getuid();
		if ((usr = getpwuid(uid)) != NULL) {
			snprintf(path, PATH_MAX, "/home/%s", usr->pw_name);
		} else {
			if (errno != 0)
				fprintf(stderr, "cd: getpwuid() error: "
					"%s\n", strerror(errno));
			else
				fprintf(stderr, "cd: getpwuid() error: "
					"username not found\n");
			return EXIT_FAILURE;
		}
	} else	/* dir */
		strncpy(path, arg[1], PATH_MAX);

	path[PATH_MAX - 1] = '\0';

	if (chdir(path) == -1) {
		fprintf(stderr, "cd: %s: chdir() error: %s\n",
			path, strerror(errno));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}

/* we assume echo always success */
void
echo_exec(char *arg[MAX_ARGC])
{
	char **tmp;
	char *str;
	int i;
	
	for (tmp = arg + 1; *tmp != NULL; tmp++) {
		str = *tmp;
		for (i = 0; i < strlen(str); i++) {
			if ((i < strlen(str) - 1) && *(str + i) == '$' &&
				*(str + i + 1) == '$') { /* $$ */
				printf("%d", processid);
				i++;
			} else if ((i < strlen(str) - 1) && *(str + i) == '$'
					&& *(str + i + 1) == '?') { /* $? */
				printf("%d", ret_val);
				i++;
			} else {
				printf("%c", *(str + i));
			}
		}
		printf(" ");
	}
	printf("\n");
	fflush(stdout);
}

void
exit_usage()
{
	fprintf(stderr, "exit: usage: exit\n");
}

void
cd_usage()
{
	fprintf(stderr, "cd: usage: cd [dir]\n");
}
