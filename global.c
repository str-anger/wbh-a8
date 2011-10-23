#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

struct cortage { int M, N, seed0, a, b, m; };
struct frame { int mini, minj, maxi, maxj, sum; };

int** A;
int TEST_COUNT;
struct cortage *inputs;
