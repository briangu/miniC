#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "miniC.h"

typedef F (*JF)(F, F);
JF a(size_t s) { return (JF)mmap(0, s, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); }
V f(JF j, size_t s) { munmap(j, s); }
JF g(char o) {
    JF j = a(4096); if (j == MAP_FAILED) return 0;
    G c[] = {0xf2, 0x0f, 0x00, 0xc1, 0xc3};
    switch (o) { case '+': c[2] = 0x58; break; case '-': c[2] = 0x5c; break; case '*': c[2] = 0x59; break; case '/': c[2] = 0x5e; break; default: f(j, 4096); return 0; }
    memcpy(j, c, sizeof(c)); return j;
}
M {
    JF a = g('+'), s = g('-'), m = g('*'), d = g('/');
    F x = 5, y = 3;
    O("Add: %f\nSubtract: %f\nMultiply: %f\nDivide: %f\n", a(x, y), s(x, y), m(x, y), d(x, y));
    f(a, 4096); f(s, 4096); f(m, 4096); f(d, 4096);
    R 0;
}

