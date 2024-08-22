#include <stdint.h>
#include <stdio.h>
#include <threads.h>

thread_local uint64_t tls_deadbeef = 0xdeadbeefdeadbeef;

int main(void) { printf("tls = %lx\n", tls_deadbeef); }
