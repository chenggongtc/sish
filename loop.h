/* 
 * loop.h	- header file for loop.c
 *
 * sish		- a simple shell
 *
 * Members:
 *		- Gong Cheng,	gcheng2@stevens.edu
 *		- Maisi Li,	mli27@stevens.edu
 *
 * Usage:	- ./sish [-x][-c command]
 */

#ifndef _LOOP_H_
#define _LOOP_H_

#include "sish.h"

static int check_status(int);
static void execute_command(char [MAX_CMD][MAX_CMD_LEN], pid_t [MAX_CMD], int);
static void trace(char [MAX_CMD][MAX_CMD_LEN], int);
static void print();
static void intHandler(int);
static void chldHandler(int);

#endif
