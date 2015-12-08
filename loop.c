#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sish.h"

int
execute()
{
	int ret_val;
	char *buf[MAX_ARGC];
	char arg[MAX_ARGC][MAX_ARG_LEN];
	char line[MAX_CMD_LEN];
	char cmd[MAX_CMD][MAX_ARG_LEN]
	char input[PATH_MAX];
	char output[PATH_MAX];
	int status;
	int is_bg;
	pid_t pid;
	int i;
	COMMAND command;

	ret_val = DEFAULT_RET;
	is_bg = 0;
	i = 0;

	if (cflag) {
		strncpy(line, ccmd, MAX_CMD_LEN - 1);
		line[MAX_CMD_LEN - 1] = '\0';

		if (syntax_check(line, is_bg) == -1) {
			fprintf(stderr, "Syntax error.\n");
			return DEFAULT_RET;
		}

		parse_cmd(line, cmd);

		if (xflag) {
			tracing(cmd);
		}

		while (strcmp(cmd[i], "") != 0) {
			ini_command(&command);
			parse_command(&command, cmd[i]);

			if ((pid = fork()) == -1) {
				fprintf(stderr, "%s: %s\n", command.arg[0],
					strerror(errno));
				return DEFAULT_RET;
			} else if (pid == 0) {
				execvp(arg[0], arg);
				fprintf(stderr, "%s: %s\n", command,arg[0],
					strerror(errno));
				return DEFAULT_RET;
			} else {
				if (waitpid(pid, &status, 0) == -1) {

				}
			}

			i++;
		}
//		run

		return ret_val;
	}

	for (;;) {
//		ini_arg(arg);
		
		printf("sish$ ");

		fgets(line, MAX_CMD_LEN - 1, stdin);
		cmd[strlen(line) - 1] = '\0';
		fflush(stdin);

//		if(syntax_check() == -1)


	}

	return ret_val;
}

void
print()
{
	printf("sish$ ");
}
