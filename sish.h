#ifndef _SISH_H_
#define _SISH_H_

#include <limits.h>

extern int cflag;
extern int xflag;
extern char *ccmd;

#define MAX_ARGC 64
#define MAX_ARG_LEN PATH_MAX
#define MAX_CMD 20
#define MAX_CMD_LEN 2048
#define DEFAULT_RET 127

typedef struct {
	char input[PATH_MAX];
	char output[PATH_MAX];
	char *arg[MAX_ARGC];
	char buf[MAX_ARGC][MAX_ARG_LEN];
} COMMAND;

int execute();
#endif
