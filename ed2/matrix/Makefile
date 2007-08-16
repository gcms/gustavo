CC=gcc
CFLAGS=-Wall -g -DQUIET
LIBS=

ECHO=echo
TESTRUNNER=test_runner

all: matrix.o test

matrix.o: matrix.c
	@$(ECHO) "Building modules..."
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o

test_runner: matrix.o matrix_test.c test_runner.c
	@$(ECHO) "Building tests..."
	$(CC) $(CFLAGS) -c matrix_test.c -o matrix_test.o
	$(CC) $(CFLAGS) -c test_runner.c -o test.o
	$(CC) $(LIBS) test.o matrix.o matrix_test.o -o test_runner

test: test_runner
	@$(ECHO) "Running tests..."
	@./test_runner

clean:
	rm -f matrix.o
	rm -f matrix_test.o
	rm -f test_runner.o
	rm -f test_runner
