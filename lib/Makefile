CC = gcc -Wall -Wextra -Werror -ansi -pedantic -O3

All:core args youtube

core:core.c core.h
	$(CC) -c core.c

args:args.c args.h
	$(CC) -c args.c

youtube:youtube.c youtube.h
	$(CC) -c youtube.c

clean:
	rm -f core.o args.o youtube.o

