CC = gcc -Wall -Wextra -Werror -ansi -pedantic -O3

All:rgrep

rgrep:rgrep.o sub_dir
	$(CC) rgrep.o lib/core.o lib/args.o lib/youtube.o -o rgrep

rgrep.o:rgrep.c rgrep.h
	$(CC) -c rgrep.c

sub_dir:
	$(MAKE) -C lib

clean:
	rm -f rgrep rgrep.o
	$(MAKE) -C lib clean

.PHONY:sub_dir

