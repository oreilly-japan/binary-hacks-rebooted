#include <stdio.h>

__attribute__((noinline)) void first_0(void) {
  puts("The 1st argument is 0.");
}
__attribute__((noinline)) void first_1(void) {
  puts("The 1st argument is 1.");
}
__attribute__((noinline)) void all_00(void) {
  puts("All arguments are 00.");
}
__attribute__((noinline)) void all_01(void) {
  puts("All arguments are 01.");
}
__attribute__((noinline)) void all_10(void) {
  puts("All arguments are 10.");
}
__attribute__((noinline)) void all_11(void) {
  puts("All arguments are 11.");
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < 100000000; i++) {
    if ((*argv[1]) == '0') {
      first_0();
      if ((*argv[2]) == '0') {
        all_00();
      } else {
        all_01();
      }
    } else {
      first_1();
      if ((*argv[2]) == '0') {
        all_10();
      } else {
        all_11();
      }
    }
  }
  return 0;
}
