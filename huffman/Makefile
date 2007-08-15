CC=cc
CFLAGS=-Wall -g
LIBS=-lm

all: huffman

huffman: huffman.o codecalculator.o bitbuffer.o fileutil.o strutil.o hashtable.o myassert.o huffman_test.c
	$(CC) $(LIBS) huffman.o codecalculator.o bitbuffer.o fileutil.o strutil.o hashtable.o myassert.o huffman_test.c -o huffman

huffman.o: huffman.c

codecalculator.o: codecalculator.c

bitbuffer.o: bitbuffer.c

fileutil.o: fileutil.c

strutil.o: strutil.c

hashtable.o: hashtable.c

myassert.o: myassert.c


clean:
	rm -f huffman.o
	rm -f codecalculator.o
	rm -f bitbuffer.o
	rm -f fileutil.o
	rm -f strutil.o
	rm -f hashtable.o
	rm -f myassert.o
	rm -f huffman
