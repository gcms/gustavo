CC=gcc
CFLAGS=-Wall -g `pkg-config gtk+-2.0 libart-2.0 --cflags`
LIBS=`pkg-config gtk+-2.0 libart-2.0 --libs` -lm

all: build
	$(CC) $(LIBS) contingoneline.o contingdrawing.o contingcomponent.o contingline.o contingutil.o continggroup.o contingtrans2.o contingbus.o -o main

build: contingoneline.o contingdrawing.o contingcomponent.o contingline.o contingutil.o continggroup.o contingtrans2.o contingbus.o

contingutil.o: contingutil.c
	$(CC) $(CFLAGS) -c contingutil.c -o contingutil.o

contingoneline.o: contingoneline.c
	$(CC) $(CFLAGS) -c contingoneline.c -o contingoneline.o

contingdrawing.o: contingdrawing.c
	$(CC) $(CFLAGS) -c contingdrawing.c -o contingdrawing.o

contingcomponent.o: contingcomponent.c
	$(CC) $(CFLAGS) -c contingcomponent.c -o contingcomponent.o

contingline.o: contingline.c
	$(CC) $(CFLAGS) -c contingline.c -o contingline.o

continggroup.o: continggroup.c
	$(CC) $(CFLAGS) -c continggroup.c -o continggroup.o

contingtrans2.o: contingtrans2.c
	$(CC) $(CFLAGS) -c contingtrans2.c -o contingtrans2.o

contingbus.o: contingbus.c
	$(CC) $(CFLAGS) -c contingbus.c -o contingbus.o

clean:
	rm -f contingoneline.o
	rm -f contingdrawing.o
	rm -f contingcomponent.o
	rm -f contingline.o
	rm -f contingutil.o
	rm -f continggroup.o
	rm -f contingtrans2.o
	rm -f contingbus.o
	rm -f main
