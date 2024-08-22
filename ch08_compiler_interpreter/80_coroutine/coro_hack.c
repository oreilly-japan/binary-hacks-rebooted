#include <stdio.h>

#define CO_BEGIN        \
  static int state = 0; \
  switch (state) {      \
    case 0:
#define CO_YIELD(state_id, retval) \
  do {                             \
    state = state_id;              \
    return retval;                 \
    case state_id:;                \
  } while (0)
#define CO_END \
  }            \
  return -1;

int iota(void) {
  static int i;
  CO_BEGIN;
  for (i = 0; i < 3; i++) {
    // for 文の中への再入が実現できる
    CO_YIELD(1, i);
  }
  // 終了前に最後に 42 を yield する
  CO_YIELD(2, 42);
  CO_END;
}

int main(void) {
  int result = 0;
  while (result >= 0) {
    result = iota();
    printf("%d\n", result);
  }
}
