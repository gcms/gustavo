CC=gcc
CFLAGS=-Wall -g `pkg-config gtk+-2.0 libart-2.0 --cflags`
LIBS=`pkg-config gtk+-2.0 libart-2.0 --libs`

all: build
	$(CC) $(LIBS) contingoneline.o contingdrawing.o contingcomponent.o contingline.o -o main

build: contingoneline.o contingdrawing.o contingcomponent.o contingline.o


contingoneline.o: contingoneline.c
	$(CC) $(CFLAGS) -c contingoneline.c -o contingoneline.o

contingdrawing.o: contingdrawing.c
	$(CC) $(CFLAGS) -c contingdrawing.c -o contingdrawing.o

contingcomponent.o: contingcomponent.c
	$(CC) $(CFLAGS) -c contingcomponent.c -o contingcomponent.o

contingline.o: contingline.c
	$(CC) $(CFLAGS) -c contingline.c -o contingline.o

clean:
	rm -f contingoneline.o
	rm -f contingdrawing.o
	rm -f contingcomponent.o
	rm -f contingline.o
	rm -f main
