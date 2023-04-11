#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include "miniC.h"

typedef F (*JF)(F, F);
typedef F (*VecOp)(F *, I);
typedef clock_t CT;

JF a(size_t s) { R(JF)mmap(0, s, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); }
V f(void *j, size_t s) { munmap(j, s); }
ZC G op[256] = { ['+'] = 0x58, ['-'] = 0x5C, ['*'] = 0x59, ['/'] = 0x5E };

JF g(char o) {
    JF j = a(4096); if (j == MAP_FAILED) R 0;
    G c[] = {0xf2, 0x0f, op[(G)o], 0xc1, 0xc3};
    memcpy(j, c, sizeof(c)); R j;
}

VecOp vec_op(JF op) {
    VecOp vop = (VecOp)a(4096); if (vop == MAP_FAILED) R 0;
    G code[] = {
        0x55,                               // push   %rbp
        0x48, 0x89, 0xe5,                   // mov    %rsp,%rbp
        0x48, 0x89, 0x7d, 0xf8,             // mov    %rdi,-0x8(%rbp)
        0x89, 0x75, 0xf4,                   // mov    %esi,-0xc(%rbp)
        0x48, 0x8b, 0x45, 0xf8,             // mov    -0x8(%rbp),%rax
        0x66, 0x0f, 0x10, 0x00,             // movups (%rax),%xmm0
        0x8b, 0x55, 0xf4,                   // mov    -0xc(%rbp),%edx
        0x83, 0xea, 0x04,                   // sub    $0x4,%edx
        0x89, 0xd0,                         // mov    %edx,%eax
        0x48, 0x98,                         // cdqe
        0x48, 0x8d, 0x14, 0x85, 0x00, 0x00, // lea    0x0(,%rax,4),%rdx
        0x00, 0x00,                         //
        0x48, 0x8d, 0x7c, 0x10, 0x10,       // lea    0x10(%rax,%rdx,1),%rdi
        0x48, 0x8d, 0x75, 0xf4,             // lea    -0xc(%rbp),%rsi
        0xb8, 0x00, 0x00, 0x00, 0x00,       // mov    $0x0,%eax
        0x66, 0x0f, 0x10, 0x1c, 0x10,       // movups (%rax,%rdx,1),%xmm3
        0xf2, 0x0f, 0x11, 0x45, 0xe0,       // movsd  %xmm0,-0x20(%rbp)
        0xf2, 0x0f, 0x11, 0x5d, 0xe8,       // movsd  %xmm3,-0x18(%rbp)
        0x48, 0x8b, 0x4d, 0x10,             // mov    0x10(%rbp),%rcx // Load the address of "op" function into %rcx
        0xff, 0xd1,                         // callq  *%rcx          // Call the "op" function
        0xf2, 0x0f, 0x10, 0x45, 0xe0, // movsd -0x20(%rbp),%xmm0
        0x83, 0x45, 0xf4, 0x04, // addl $0x4,-0xc(%rbp)
        0x83, 0x7d, 0xf4, 0x00, // cmpl $0x0,-0xc(%rbp)
        0x7f, 0xc2, // jg -0x3e
        0xf2, 0x0f, 0x11, 0x45, 0xe0, // movsd %xmm0,-0x20(%rbp)
        0xf2, 0x0f, 0x10, 0x45, 0xe0, // movsd -0x20(%rbp),%xmm0
        0x5d, // pop %rbp
        0xc3 // retq
    };
    memcpy(vop, code, sizeof(code));
    *(JF*)(vop + 0x30) = op; // Store the address of "op" function into the code
    return vop;
}

M {
    JF a = g('+'), s = g('-'), m = g('*'), d = g('/');
    F x = 5, y = 3;
    O("Add: %f\nSubtract: %f\nMultiply: %f\nDivide: %f\n", a(x, y), s(x, y), m(x, y), d(x, y));

    F arr[] = {1.0, 2.0, 3.0, 4.0};
    I len = sizeof(arr) / sizeof(F);
    VecOp add_op = vec_op(a);

    F sum = add_op(arr, len);
    O("Sum: %f\n", sum);

    F avg = sum / len;
    O("Avg: %f\n", avg);

    VecOp mult_op = vec_op(m);
    F reduce = mult_op(arr, len);
    O("Reduce: %f\n", reduce);

    f(a, 4096); f(s, 4096); f(m, 4096); f(d, 4096); f(add_op, 4096); f(mult_op, 4096);
    R 0;
}

