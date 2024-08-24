int data = 99; /* 値あり read write 変数は .dataに置かれる */
const char message[] = "Hello, World!"; /* const変数は.rodataに置かれる */
char buffer[1024]; /* 未初期化変数(初期値0)は.bssに置かれる */

extern char __bss_start[]; /* リンカが定義する.bssの開始アドレス */
extern char _end[]; /* リンカが定義する.bssの終了アドレス */

void start(void) {
  char *p;
  for (p = __bss_start; p != _end; p++) {
    *p = 0; /* .bssを0クリア */
  }
}
