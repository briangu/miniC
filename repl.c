#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "miniC.h"

C *e;

F x(F *r), t(F *r), f(F *r), n(F *r);

F x(F *r) { F l, h; t(&l); W(*e == '+' || *e == '-') { C o = *e++; t(&h); l = (o == '+') ? l + h : l - h; } *r = l; R 1; }
F t(F *r) { F l, h; f(&l); W(*e == '*' || *e == '/') { C o = *e++; f(&h); l = (o == '*') ? l * h : l / h; } *r = l; R 1; }
F f(F *r) { F v; I neg = (*e == '-'); e += neg; R *e == '(' ? (++e, x(&v), *e == ')' ? (++e, *r = neg ? -v : v, 1) : 0) : n(&v) ? (*r = neg ? -v : v, 1) : 0; }
F n(F *r) { F v = 0; if (isdigit(*e)) { W(isdigit(*e)) v = v * 10 + (*e - '0'), ++e; *r = v; R 1; } R 0; }

V r() { O("Arithmetic REPL\nEnter an arithmetic expression or type 'exit' to quit.\n"); S l = NULL; W(getline(&l, (size_t[]){0}, stdin) != -1) { Q(strcmp(l, "exit\n") == 0) R; e = l; F v; x(&v) ? O("Result: %f\n", v) : O("Invalid expression.\n"); } free(l); }
M { r(); R 0; }

