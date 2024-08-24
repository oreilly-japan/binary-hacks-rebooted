#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// パスワード: "Th15_15_s3cr3t"
#define SIZE_PASSWORD 14
const unsigned int garbled_pass[SIZE_PASSWORD] = {
    0xbf5cbdf4, 0x82bafbdc, 0x05f6b7a0, 0x0e6e2f5a, 0x245d9e9b,
    0x493bfd2e, 0x94f8ba46, 0x3172b4a3, 0x6467297f, 0xca4f12e7,
    0x961fa5ff, 0x2d410bc7, 0x5c02d797, 0xba076f14,
};

// 適当な値の変換を行う関数
unsigned int garble(char c) {
  static unsigned int prev_val = 0xdeadbeef;

  unsigned char uc = c;
  prev_val = ((prev_val << 1) ^ (uc >> 1)) + ((uc >> 4) << 14) +
             ((uc & 0xf) << 23);
  return prev_val;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Pass argument :(");
    return 0;
  }

  srand(time(NULL));

  if (rand() != 777) {
    puts("Bad luck :(");
    return 0;
  }

  char input[SIZE_PASSWORD + 1] = "";
  strncpy(input, argv[1], SIZE_PASSWORD);

  for (int i = 0; i < SIZE_PASSWORD; i++) {
    if (garble(input[i]) != garbled_pass[i]) {
      puts("Fail :(");
      return 0;
    }
  }
  puts("You did it!");
}
