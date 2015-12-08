/* 
 * sish.c - main program for sish
 */

#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sish.h"

int cflag;
int xflag;
char *ccmd;

void usage();

int
main(int argc, char **args)
{
	int opt;

	while ((opt = getopt(argc, args, "c:x")) != -1) {
		switch (opt) {
		case 'c':
			cflag = 1;
			ccmd = optarg;
			break;
		case 'x':
			xflag = 1;
			break;
		default:
			usage();
		}
	}

	execute();

	return 0;
}

void
usage()
{
	fprintf(stderr, "Usage: ./sish [-x][-c command]\n");
	exit(EXIT_FAILURE);
}
