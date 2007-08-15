CC=gcc
CFLAGS=-Wall
LIBS=-lm

all: hashtable.o fileutil.o list.o myassert.o main.o
	$(CC) $(LIBS) hashtable.o fileutil.o main.o myassert.o list.o

hashtable.o: hashtable.c

fileutil.o: fileutil.c

main.o: main.c

myassert.o: myassert.c

list.o: list.c

clean:
	rm -f hashtable.o
	rm -f fileutil.o
	rm -f list.o
	rm -f myassert.o
	rm -f main.o
	rm -f a.out
