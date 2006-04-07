CC=gcc
CFLAGS=-Wall -g `pkg-config gtk+-2.0 --cflags`
LIBS=`pkg-config gtk+-2.0 --libs`

all: build
	$(CC) $(LIBS) oneline.o contingdrawing.o contingdrawingbus.o contingdrawingline.o -o main

build: oneline.o contingdrawing.o contingdrawingbus.o contingdrawingline.o


oneline.o: oneline.c
	$(CC) $(CFLAGS) -c oneline.c -o oneline.o

contingdrawing.o: contingdrawing.c
	$(CC) $(CFLAGS) -c contingdrawing.c -o contingdrawing.o

contingdrawingbus.o: contingdrawingbus.c
	$(CC) $(CFLAGS) -c contingdrawingbus.c -o contingdrawingbus.o

contingdrawingline.o: contingdrawingline.c
	$(CC) $(CFLAGS) -c contingdrawingline.c -o contingdrawingline.o

clean:
	rm -f oneline.o
	rm -f contingdrawing.o
	rm -f contingdrawingbus.o
	rm -f contingdrawingline.o
	rm -f main
