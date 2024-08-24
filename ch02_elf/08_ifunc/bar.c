#include <stdio.h>

extern void bar(void);
void bar_1(void) { printf("bar_1\n"); }
void bar_2(void) { printf("bar_2\n"); }

void bar(void) __attribute__((ifunc("resolve_bar")));

static void *resolve_bar(void) {
  printf("resolve_bar\n");
  if (0)
    return bar_1;
  else
    return bar_2;
}
