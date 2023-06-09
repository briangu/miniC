#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include "miniC.h"

typedef F (*JF)(F, F);
typedef clock_t CT;

JF a(size_t s) { R(JF)mmap(0, s, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); }
V f(JF j, size_t s) { munmap(j, s); }
ZC G op[256] = { ['+'] = 0x58, ['-'] = 0x5C, ['*'] = 0x59, ['/'] = 0x5E };
JF g(char o) {
    JF j = a(4096); if (j == MAP_FAILED) R 0;
    G c[] = {0xf2, 0x0f, op[(G)o], 0xc1, 0xc3};
    memcpy(j, c, sizeof(c)); R j;
}
M {
    JF a = g('+'), s = g('-'), m = g('*'), d = g('/');
    F x = 5, y = 3;
    O("Add: %f\nSubtract: %f\nMultiply: %f\nDivide: %f\n", a(x, y), s(x, y), m(x, y), d(x, y));
    
    CT st = clock(); L (I i=0; i < 1000000000; i++) a(x,y); CT et = clock();
    F tt = (F)(et - st) / CLOCKS_PER_SEC;
    O("Time taken for 10**9 iterations: %f seconds, %.15fs / iter\n", tt, tt/1000000000.0);

    f(a, 4096); f(s, 4096); f(m, 4096); f(d, 4096);
    R 0;
}

