CC = gcc
CFLAGS = -Wall -O2
TARGETS = fib gen_add jit repl

all: $(TARGETS)

fib: fib.c miniC.h
	$(CC) $(CFLAGS) -o fib fib.c

gen_add: gen_add.c miniC.h
	$(CC) $(CFLAGS) -o gen_add gen_add.c

jit: jit.c miniC.h
	$(CC) $(CFLAGS) -o jit jit.c

repl: repl.c miniC.h
	$(CC) $(CFLAGS) -o repl repl.c

clean:
	rm -f $(TARGETS)

