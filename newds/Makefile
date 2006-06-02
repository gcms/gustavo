CC=gcc
CFLAGS=-Wall -g -I.
LIBS=

all: build

build: stack.o bst.o red_black.o

buildtest: stack_test.o bst_test.o base_test.o red_black_test.o

runtest: buildtest stack_test_run bst_test_run base_test_run red_black_test_run



bst.o: bst.c bst.h
	$(CC) $(CFLAGS) -c bst.c -o bst.o

bst_test.o: bst_test.c bst.h
	$(CC) $(CFLAGS) -c bst_test.c -o bst_test.o

bst_test: bst_test.o bst.o base.o
	$(CC) $(LIBS) bst_test.o bst.o base.o -o bst_test

bst_test_run: bst_test
	./bst_test

red_black.o: red_black.c red_black.h
	$(CC) $(CFLAGS) -c red_black.c -o red_black.o

red_black_test.o: red_black_test.c red_black.h
	$(CC) $(CFLAGS) -c red_black_test.c -o red_black_test.o

red_black_test: red_black_test.o red_black.o base.o
	$(CC) $(LIBS) red_black_test.o red_black.o base.o -o red_black_test

red_black_test_run: red_black_test
	./red_black_test

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c -o stack.o

stack_test.o: stack_test.c stack.h
	$(CC) $(CFLAGS) -c stack_test.c -o stack_test.o

stack_test: stack_test.o stack.o
	$(CC) $(LIBS) stack_test.o stack.o -o stack_test

stack_test_run: stack_test
	./stack_test

base.o: base.c base.h
	$(CC) $(CFLAGS) -c base.c -o base.o

base_test.o: base_test.c base.h
	$(CC) $(CFLAGS) -c base_test.c -o base_test.o

base_test: base_test.o base.o
	$(CC) $(LIBS) base_test.o base.o -o base_test

base_test_run: base_test
	./base_test


clean:
	rm -f stack.o
	rm -f stack_test.o
	rm -f stack_test
	rm -f bst.o
	rm -f bst_test.o
	rm -f bst_test
	rm -f base.o
	rm -f base_test.o
	rm -f base_test
	rm -f red_black.o
	rm -f red_black_test.o
	rm -f red_black_test
