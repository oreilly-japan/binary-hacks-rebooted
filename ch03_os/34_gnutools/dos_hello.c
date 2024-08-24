const char message[] = "Hello, World!\r\n$";

void start(void) {
  const char *p;

  /* ah=9に設定してint 0x21を実行するとdxで示す先が出力される */
  asm volatile("int 0x21" ::"a"(9 << 8), "d"(message));
  /* int 0x20 でプログラム終了(exit) */
  asm volatile("int 0x20");
}
