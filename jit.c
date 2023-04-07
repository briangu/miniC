#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "miniC.h"

typedef F (*JitFunc)(F, F);

// Utility function to allocate executable memory
JitFunc allocate_executable_memory(size_t size) {
    return (JitFunc)mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

// Utility function to free executable memory
void free_executable_memory(JitFunc jit_func, size_t size) {
    munmap(jit_func, size);
}

// Generate machine code for arithmetic operations
JitFunc generate_arithmetic_function(char operation) {
    JitFunc jit_func = allocate_executable_memory(4096);
    NP(jit_func != MAP_FAILED);

    // Generate x86-64 instructions for arithmetic operations
    G fn[] = {
        0xf2, 0x0f, 0x00, 0xc1, // <op>ss xmm0, xmm1
        0xc3                    // ret
    };

    G operation_code;
    switch (operation) {
        case '+':
            operation_code = 0x58;  // addss xmm0, xmm1
            break;
        case '-':
            operation_code = 0x5c;  // subss xmm0, xmm1
            break;
        case '*':
            operation_code = 0x59;  // mulss xmm0, xmm1
            break;
        case '/':
            operation_code = 0x5e;  // divss xmm0, xmm1
            break;
        default:
            free_executable_memory(jit_func, 4096);
            R NULL;
    }

    memcpy(jit_func, fn, sizeof(fn));
    ((G*)jit_func)[2] = operation_code;

    return jit_func;
}


M {
    JitFunc add_func = generate_arithmetic_function('+');
    JitFunc sub_func = generate_arithmetic_function('-');
    JitFunc mul_func = generate_arithmetic_function('*');
    JitFunc div_func = generate_arithmetic_function('/');
    
    F a = 5, b = 3;
    O("Add: %f\n", add_func(a, b));
    O("Subtract: %f\n", sub_func(a, b));
    O("Multiply: %f\n", mul_func(a, b));
    O("Divide: %f\n", div_func(a, b));
    
    free_executable_memory(add_func, 4096);
    free_executable_memory(sub_func, 4096);
    free_executable_memory(mul_func, 4096);
    free_executable_memory(div_func, 4096);

    R 0;
}

