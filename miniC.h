#ifndef MINIC_H
#define MINIC_H

typedef char *S;
typedef char C;
typedef unsigned char G;
typedef short H;
typedef int I;
typedef long long J;
typedef float E;
typedef double F;
typedef void V;
typedef unsigned long long UJ;

// Additional macros for compact code
#define V void
#define M int main()
#define Q if
#define L for
#define W while
#define O printf
#define R return
#define Z static
#define ZC static const
#define N(x) if (!(x)) return;
#define NP(x) if (!(x)) return NULL;

#define SW switch
#define CS(n, x) \
    case n:      \
        x;       \
        break;
#define CD default:

#endif // MINIC_H

