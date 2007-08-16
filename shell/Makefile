CC=gcc
CFLAGS=-Wall -g -I.
LIBS=
MAKE=make

NEWDS_HOME=../newds

all: newshell


newshell: newshell.o jobs.o builtin.o history.o alias.o string_command.o
	$(MAKE) -C $(NEWDS_HOME)
	$(CC) $(LIBS) newshell.o jobs.o builtin.o history.o alias.o string_command.o $(NEWDS_HOME)/*.o -o newshell

string_command.o: string_command.c
	$(CC) $(CFLAGS) -c string_command.c -o string_command.o

newshell.o: newshell.c
	$(CC) $(CFLAGS) -c newshell.c -o newshell.o

jobs.o: jobs.c
	$(CC) $(CFLAGS) -c jobs.c -o jobs.o

history.o: history.c
	$(CC) $(CFLAGS) -c history.c -o history.o

builtin.o: builtin.c
	$(CC) $(CFLAGS) -c builtin.c -o builtin.o

alias.o: alias.c
	$(CC) $(CFLAGS) -c alias.c -o alias.o

clean:
	rm -f string_command.o
	rm -f newshell.o
	rm -f jobs.o
	rm -f history.o
	rm -f builtin.o
	rm -f alias.o
	rm -f newshell
	$(MAKE) -C $(NEWDS_HOME) clean
