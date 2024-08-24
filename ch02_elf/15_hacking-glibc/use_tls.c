#include <stdio.h>
_Thread_local unsigned long tls_var_with_init = 0xaabbccddaabbccdd;
int main(void) { printf("tls_var_with_init = 0x%lx\n", tls_var_with_init); }
