CC = gcc
CXX = g++
CFLAGS = -Wall -O3
CXXFLAGS = -std=c++14 $(shell llvm-config --cxxflags)
LLVMCFLAGS = $(shell llvm-config --cflags)
LLVMLIBS = $(shell llvm-config --ldflags --system-libs --libs core executionengine mcjit native orcjit)
TARGETS = fib jit repl vec llvm_jit_example llvm_c_example
BUILD_DIR = build

all: $(TARGETS)

fib: fib.c miniC.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ fib.c

jit: jit.c miniC.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ jit.c

vec: vec.c miniC.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ vec.c

repl: repl.c miniC.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ repl.c

llvm_jit_example: llvm_jit_example.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ llvm_jit_example.cpp $(LLVMLIBS)

llvm_c_example: llvm_c_example.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LLVMCFLAGS) -o $(BUILD_DIR)/$@ llvm_c_example.c $(LLVMLIBS)

clean:
	rm -rf $(BUILD_DIR)

