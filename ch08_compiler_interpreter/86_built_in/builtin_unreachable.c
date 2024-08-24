#include <assert.h>
int foo0(void);
int foo1(void);
int foo2(void);
int foo3(void);
int foo(int x) {
  assert(0 <= x && x < 4);
  switch (x) {
    case 0:
      return foo0();
    case 1:
      return foo1();
    case 2:
      return foo2();
    case 3:
      return foo3();
    default:
      return -1;  // ここには来ないことがわかっているとする
  }
}
