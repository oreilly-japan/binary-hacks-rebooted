#include <stdio.h>

extern long double calc(long double x);

int main(void) { printf("calc(1.0L) = %Lg\n", calc(1.0L)); }
