/*
 * utils.c	- utility functions
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
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sish.h"
#include "utils.h"

/* check syntax for the input */
int
syntax_check(char *origline, int *is_bg)
{
	int i = 0, j = 0;
	int len = 0;
	
	int flag_redirect = 0;
	int flag_notEmptyAfterBack = 0;
	int flag_notEmptyBeforePipe = 0;
	int flag_notEmptyAfterPipe = 0;	
	int flag_hasBackground = 0;
	int flag_pipe = 0;

	char line[MAX_LINE_LEN];
	char *buf_cmd;
	char *buf_arg;
	char *token_pipe = "|";
	char *token_arg = " \t><&";
	char *save_cmd;
	char *save_arg;
		
	// check lens
	len = strlen(origline);
	
	if(len >= MAX_LINE_LEN) {
		fprintf(stderr, "%s: syntax error: commands too long\n",
			progname);
		return -1;
	}

	strcpy(line, origline);
	buf_cmd = strtok_r(line, token_pipe, &save_cmd);
	
	while(buf_cmd != NULL) {
		if(i >= MAX_CMD) {
			fprintf(stderr, "%s: syntax error: too many commands\n",
				progname);
			return -1;
		}
		i++;
		j = 0;
		len = strlen(buf_cmd);
		if(len >= MAX_CMD_LEN) {	
			fprintf(stderr, "%s: syntax error: one or more commands"
					" exceed max command length\n",
					progname);
			return -1;	
		}
		
		buf_arg = strtok_r(buf_cmd,token_arg, &save_arg);	
		while(buf_arg != NULL) {
			if(j >= MAX_ARGC - 1) {	
				fprintf(stderr, "%s: syntax error: too many"
						" arguments\n", progname);
				return -1;
			}
			j++;
			len = strlen(buf_arg);
			if(len > MAX_ARG_LEN) {
				fprintf(stderr, "%s: syntax error: one or more"
						" arguments exceed max argument"
						" length\n", progname);
				return -1;
			}
			buf_arg = strtok_r(NULL, token_arg, &save_arg);
		}
		buf_cmd = strtok_r(NULL,token_pipe, &save_cmd);
	}

	//
	strcpy(line, origline);	
	i = -1; j = 0;
	len = strlen(line);
	
	while(i < len) {
		i++;
		if(whitespace(line[i])) {
			continue;
	
		} else if (line[i] == '>') {
			flag_notEmptyAfterBack = 1;
			flag_notEmptyAfterPipe = 1;
			if(flag_redirect) {
				fprintf(stderr, "%s: syntax error: near >\n",
					progname);
				return -1;
			}
			 if(i + 1 < len && line[i + 1] == '>') {
				i++;
			}
				flag_redirect = 1;

		} else if (line[i] == '<') {
			flag_notEmptyAfterBack = 1;
			flag_notEmptyAfterPipe = 1;
			if(flag_redirect) {
				fprintf(stderr, "%s: syntax error: near <\n",
					progname);
				return -1;
			}

			flag_redirect = 1;

		} else if (line[i] == '&') {
			if(flag_redirect) {
				fprintf(stderr, "%s: syntax error: near &\n",
					progname);
				return -1;
			}
			flag_hasBackground = 1;
			flag_notEmptyAfterBack = 0;
	
		} else if (line[i] == '|') {
			
			flag_notEmptyAfterBack = 1;
			if(flag_redirect) {
				fprintf(stderr, "%s: syntax error: near |\n",
					progname);
				return -1;
			}
			
			if(flag_notEmptyAfterPipe) {
				flag_notEmptyBeforePipe = 1;
			} else
				flag_notEmptyBeforePipe	= 0;	
		
			if(!flag_notEmptyBeforePipe) {
				fprintf(stderr, "%s: syntax error: near |\n",
					progname);
				return -1;
			}
	
			flag_pipe = 1;
			flag_notEmptyAfterPipe = 0;
			
		} else if (line[i] == '\0') {
			break;
		} else {
			
			if(flag_hasBackground) {
				fprintf(stderr, "%s: syntax error: near &\n",
					progname);
				return -1;
			}
			flag_redirect = 0;
			flag_notEmptyAfterBack = 1;
			flag_notEmptyAfterPipe = 1;
		}
	}

	if(flag_hasBackground && flag_notEmptyAfterBack) {
		fprintf(stderr, "%s: syntax error: near &\n",
			progname);
		return -1;
	}
	
	if(flag_redirect) {
		fprintf(stderr, "%s: syntax error: near <newline>\n",
			progname);
		return -1;
	}
		
	if(flag_pipe && !flag_notEmptyAfterPipe) {
		return 0;
	}
	
	if(flag_hasBackground) {
		char *pos = strstr(origline, "&");
		*is_bg = 1;
		if(pos != NULL) {
			*pos = ' ';
		}	
	} else
		*is_bg = 0;

	return 1;	
}

/* split commands by '|', return the number of commands */
int
command_parser(char *line, char cmd[MAX_CMD][MAX_CMD_LEN])
{
	int count;
	int i, j;
	int len;
	int is_ws;

	is_ws = 0;
	len = strlen(line);
	count = 0;
	i = 0;

	while (i < len) {
		if (count == MAX_CMD)
			break;
		while (i < len && whitespace(line[i]))
			i++;
		if (i == len)
			break;
		j = 0;
		while (i < len) {
			if (line[i] == '|') {
				i++;
				break;
			} else if (whitespace(line[i])) {
				if (!is_ws) {
					if (j == MAX_CMD_LEN - 1) {
						while (i < len && line[i] !=
								'|')
							i++;
						i++;
						break;
					}
					cmd[count][j] = ' ';
					j++;
				}
				i++;
				is_ws = 1;
			} else if (line[i] == '>'){
				if (!is_ws) {
					if (j == MAX_CMD_LEN - 1) {
						while (i < len && line[i] !=
								'|')
							i++;
						i++;
						break;
					}
					cmd[count][j] = ' ';
					j++;
				}
				if (j == MAX_CMD_LEN - 1) {
					while (i < len && line[i] != '|')
						i++;
					i++;
					break;
				}
				cmd[count][j] = '>';
				j++;
				i++;
				if (i < len && line[i] == '>') {
					if (j == MAX_CMD_LEN - 1) {
						while (i < len && line[i] !=
								'|')
							i++;
						i++;
						j--;
						break;
					}
					cmd[count][j] = '>';
					j++;
					i++;
				}
				if (j == MAX_CMD_LEN - 1) {
					while (i < len && line[i] != '|')
						i++;
					i++;
					break;
				}
				cmd[count][j] = ' ';
				j++;
				is_ws = 1;
			} else if (line[i] == '<') {
				if (!is_ws) {
					if (j == MAX_CMD_LEN - 1) {
						while (i < len && line[i] !=
								'|')
							i++;
						i++;
						break;
					}
					cmd[count][j] = ' ';
					j++;
				}
				if (j == MAX_CMD_LEN - 1) {
					while (i < len && line[i] != '|')
						i++;
					i++;
					break;
				}
				cmd[count][j] = '<';
				j++;
				if (j == MAX_CMD_LEN - 1) {
					while (i < len && line[i] != '|')
						i++;
					i++;
					break;
				}
				cmd[count][j] = ' ';
				j++;
				i++;
				is_ws = 1;
			} else {
				if (j == MAX_CMD_LEN - 1) {
					while (i < len && line[i] != '|')
						i++;
					i++;
					break;
				}
				cmd[count][j] = line[i];
				j++;
				i++;
				is_ws = 0;
			}
		}
		cmd[count][j] = '\0';
		count++;
	}
	
	return count;
}

/* initial COMMAND struct */
void
command_init(COMMAND *command)
{
	strcpy(command->input, "");
	strcpy(command->output, "");
	command->append = 0;
	command->arg[0] = NULL;
}

/* 
 * Parse command string to COMMAND struct.
 * Return -1 if i/o redirection file open error, return 0 on success.
 */
int
argument_parser(char *cmd, char buf[MAX_ARGC][MAX_ARG_LEN], COMMAND *command)
{
	int input_flag, output_flag, append_flag;
	int count;
	int fd;
	char *token = " \t";
	char tmp[MAX_CMD_LEN];
	char *str;

	strncpy(tmp, cmd, MAX_CMD_LEN);
	tmp[MAX_CMD_LEN - 1] = '\0';

	input_flag = output_flag = append_flag = 0;
	count = 0;
	str = strtok(tmp, token);

	while (str != NULL) {
		if (count == MAX_ARGC - 1)
			break;
		if (strcmp(str, ">") == 0) {
			output_flag = 1;
			append_flag = 0;
		} else if (strcmp(str, "<") == 0) {
			input_flag = 1;
		} else if (strcmp(str, ">>") == 0) {
			output_flag = 1;
			append_flag = 1;
		} else {
			if (input_flag) {
				input_flag = 0;
				if ((fd = open(str, O_RDONLY)) == -1) {
					fprintf(stderr, "%s: %s: %s\n", 
						progname, str, 
						strerror(errno));
					return -1;
				}
				close(fd);
				strncpy(command->input, str, PATH_MAX);
				command->input[PATH_MAX - 1] = '\0';
			} else if (output_flag) {
				output_flag = 0;
				if (append_flag) {
					if ((fd = open(str, O_WRONLY | O_APPEND
							| O_CREAT, S_IWUSR |
							S_IRUSR)) == -1) {
						fprintf(stderr, "%s: %s: %s\n", 
							progname, str, 
							strerror(errno));
						return -1;
					}
					command->append = 1;
				} else {
					if ((fd = open(str, O_WRONLY | O_TRUNC
							| O_CREAT, S_IWUSR |
							S_IRUSR)) == -1) {
						fprintf(stderr, "%s: %s: %s\n", 
							progname, str, 
							strerror(errno));
						return -1;
					}
					command->append = 0;
				}
				close(fd);

				strncpy(command->output, str, PATH_MAX);
				command->output[PATH_MAX - 1] = '\0';
			} else {
				strncpy(buf[count], str, MAX_ARG_LEN);
				buf[count][MAX_ARG_LEN - 1] = '\0';
				command->arg[count] = buf[count];
				count++;
			}
		}
		str = strtok(NULL, token);
	}
	command->arg[count] = NULL;
	return 0;
}

/* check space or tab */
static int
whitespace(char c)
{
	if (c == ' ' || c == '\t')
		return 1;
	else
		return 0;
}
 
