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

F vec_op(JF op, F *arr, I len) {
    F result = arr[0];
    for (int i = 1; i < len; ++i) {
        result = op(result, arr[i]);
    }
    return result;
}

M {
    JF a = g('+'), s = g('-'), m = g('*'), d = g('/');
    F x = 5, y = 3;
    O("Add: %f\nSubtract: %f\nMultiply: %f\nDivide: %f\n", a(x, y), s(x, y), m(x, y), d(x, y));

/*
    F arr[] = {1.0, 2.0, 3.0, 4.0};
    I len = sizeof(arr) / sizeof(F);

    F sum = vec_op(a, arr, len);
    O("Sum: %f\n", sum);

    F avg = sum / len;
    O("Avg: %f\n", avg);

    F reduce = vec_op(m, arr, len);
    O("Reduce: %f\n", reduce);
*/
    f(a, 4096); f(s, 4096); f(m, 4096); f(d, 4096);
    R 0;
}

