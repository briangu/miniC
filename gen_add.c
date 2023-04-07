#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "miniC.h" // Assuming the MiniC header is named "miniC.h"

typedef I (*JitFunc)(I, I);

V generate_add_function() {
    // Allocate executable memory
    JitFunc jit_func = (JitFunc)mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    N(jit_func != MAP_FAILED);

    // Generate x86 instructions for adding two integers (function prologue, addition, function epilogue)
    G code[] = {
        0x55,                   // push rbp
        0x48, 0x89, 0xe5,       // mov rbp, rsp
        0x89, 0x7d, 0xfc,       // mov [rbp-4], edi
        0x89, 0x75, 0xf8,       // mov [rbp-8], esi
        0x8b, 0x45, 0xfc,       // mov eax, [rbp-4]
        0x03, 0x45, 0xf8,       // add eax, [rbp-8]
        0x5d,                   // pop rbp
        0xc3                    // ret
    };
    memcpy(jit_func, code, sizeof(code));

    // Call the generated function
    I result = jit_func(3, 5);
    O("Result: %d\n", result);

    // Free the executable memory
    munmap(jit_func, 4096);
}

M {
    generate_add_function();
    R 0;
}

