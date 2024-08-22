#include <stdio.h>

void foo_1(void) { printf("foo_1\n"); }
void foo_2(void) { printf("foo_2\n"); }

void foo(void) __attribute__((ifunc("resolve_foo")));

static void *resolve_foo(void) {
  printf("resolve_foo\n");
  if (0)
    return foo_1;
  else
    return foo_2;
}

int main(void) {
  printf("beginning of main()\n");
  foo();
  return 0;
}
