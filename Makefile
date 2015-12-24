sish: sish.h sish.c loop.c loop.h utils.c utils.h builtin.c
	cc -Wall -g -o sish sish.c loop.c utils.c builtin.c

clean:
	rm -f sish
