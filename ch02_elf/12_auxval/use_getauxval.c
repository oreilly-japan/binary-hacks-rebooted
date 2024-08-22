#include <stdio.h>
#include <sys/auxv.h>
int main(void) { printf("AT_PAGESZ=%lu\n", getauxval(AT_PAGESZ)); }
