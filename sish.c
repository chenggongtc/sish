/* 
 * sish.c	- main program for sish
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
#include <sys/wait.h>

#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sish.h"

int cflag;
int xflag;
char *ccmd;
char *progname;
int ret_val;
pid_t processid;

static void usage();

int
main(int argc, char **argv)
{
	int opt;
	char path[PATH_MAX];

	progname = argv[0];
	setlocale(LC_ALL, "");

	while ((opt = getopt(argc, argv, "c:x")) != -1) {
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

	argc -= optind;

	if (argc != 0)
		usage();

	if (getcwd(path, PATH_MAX - strlen(progname)) == NULL) {
		fprintf(stderr, "%s: unable to set env SHELL: getcwd() error"
				": %s\n", progname, strerror(errno));
		return EXIT_FAILURE;
	}

	if (path[strlen(path) - 1] == '/')
		path[strlen(path) - 1] = '\0';

	snprintf(path, PATH_MAX - 1, "%s/%s", path, basename(progname));
	path[PATH_MAX - 1] = '\0';

	if (setenv("SHELL", path, 1) == -1) {
		fprintf(stderr, "%s: unable to set env SHELL: setenv() error"
				": %s\n", progname, strerror(errno));
		return EXIT_FAILURE;
	}

	ret_val = EXIT_SUCCESS;
	processid = getpid();

	return execute();
}

static void
usage()
{
	fprintf(stderr, "Usage: %s [-x][-c command]\n", progname);
	exit(EXIT_FAILURE);
}
