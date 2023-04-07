#include <stdio.h>
#include "miniC.h"

I p(I n) { Q(n<=1)R 0; L(I i=2;i*i<=n;++i)Q(n%i==0)R 0; R 1; }
V P(I N) { I c=0,n=2; W(c<N){Q(p(n)){O("%d ",n);++c;}++n;} O("\n"); }
M { I N=10; P(N); R 0; }

