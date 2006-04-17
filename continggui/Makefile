CC=gcc
CFLAGS=-Wall -g `pkg-config gtk+-2.0 --cflags`
LIBS=`pkg-config gtk+-2.0 --libs`

all: build
	$(CC) $(LIBS) oneline.o contingdrawing.o contingcomponent.o contingconnection.o contingbus.o contingtrans2.o -o main

build: oneline.o contingdrawing.o contingcomponent.o contingconnection.o contingbus.o contingtrans2.o


oneline.o: oneline.c
	$(CC) $(CFLAGS) -c oneline.c -o oneline.o

contingdrawing.o: contingdrawing.c
	$(CC) $(CFLAGS) -c contingdrawing.c -o contingdrawing.o

contingcomponent.o: contingcomponent.c
	$(CC) $(CFLAGS) -c contingcomponent.c -o contingcomponent.o

contingconnection.o: contingconnection.c
	$(CC) $(CFLAGS) -c contingconnection.c -o contingconnection.o

contingbus.o: contingbus.c
	$(CC) $(CFLAGS) -c contingbus.c -o contingbus.o

contingtrans2.o: contingtrans2.c
	$(CC) $(CFLAGS) -c contingtrans2.c -o contingtrans2.o

clean:
	rm -f oneline.o
	rm -f contingdrawing.o
	rm -f contingcomponent.o
	rm -f contingconnection.o
	rm -f contingbus.o
	rm -f contingtrans2.o
	rm -f main
