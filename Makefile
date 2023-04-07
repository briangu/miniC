# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Executable names
FIB_EXEC = fib
GEN_ADD_EXEC = gen_add
REPL_EXEC = repl

# Source files
FIB_SRC = fib.c
GEN_ADD_SRC = gen_add.c
REPL_SRC = repl.c

# Header files
MINIC_HEADER = miniC.h

# Build all targets
all: $(FIB_EXEC) $(GEN_ADD_EXEC) $(REPL_EXEC)

# Build fib
$(FIB_EXEC): $(FIB_SRC) $(MINIC_HEADER)
	$(CC) $(CFLAGS) -o $(FIB_EXEC) $(FIB_SRC)

# Build gen_add
$(GEN_ADD_EXEC): $(GEN_ADD_SRC) $(MINIC_HEADER)
	$(CC) $(CFLAGS) -o $(GEN_ADD_EXEC) $(GEN_ADD_SRC)

# Build repl
$(REPL_EXEC): $(REPL_SRC) $(MINIC_HEADER)
	$(CC) $(CFLAGS) -o $(REPL_EXEC) $(REPL_SRC)

# Clean up build artifacts
clean:
	rm -f $(FIB_EXEC) $(GEN_ADD_EXEC) $(REPL_EXEC)

.PHONY: all clean

