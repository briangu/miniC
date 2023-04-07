CC = gcc
CFLAGS = -Wall -O3
TARGETS = fib jit repl

all: $(TARGETS)

fib: fib.c miniC.h
	$(CC) $(CFLAGS) -o fib fib.c

jit: jit.c miniC.h
	$(CC) $(CFLAGS) -o jit jit.c

repl: repl.c miniC.h
	$(CC) $(CFLAGS) -o repl repl.c

clean:
	rm -f $(TARGETS)

