#include <stdio.h>

extern void foo(void);
void foo_default(void) { printf("foo_default\n"); }
void foo_1(void) { printf("foo_1\n"); }
void foo_2(void) { printf("foo_2\n"); }

void foo(void) __attribute__((ifunc("resolve_foo")));

static void *resolve_foo(void) {
  printf("resolve_foo\n");
  if (0)
    return foo_1;
  else if (42 == 41 + 1)
    return foo_2;
  else
    return foo_default;
}
