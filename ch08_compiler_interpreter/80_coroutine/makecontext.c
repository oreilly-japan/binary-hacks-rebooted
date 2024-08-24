#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

void func(void) { printf("func %p\n", __builtin_frame_address(0)); }

int main(void) {
  printf("main %p\n", __builtin_frame_address(0));
  ucontext_t main_ctx;
  ucontext_t func_ctx;
  if (getcontext(&func_ctx) != 0) {
    perror("getcontext");
    abort();
  }

  // `func` を呼び出した時に使われるスタックを準備する
  func_ctx.uc_stack.ss_sp = malloc(4096);
  func_ctx.uc_stack.ss_flags = 0;
  func_ctx.uc_stack.ss_size = 4096;
  // `func` が終了した後に戻ってくるコンテキストを指定する
  // この段階では未初期化だが、後で説明する
  func_ctx.uc_link = &main_ctx;
  // `func` を無引数で呼び出すと指定
  makecontext(&func_ctx, func, 0 /* argc */);

  // 前のコード片で未初期化だった `main_ctx` をここでセットすることで
  // `func` が終わった後に戻ってくる先をここにする
  if (swapcontext(&main_ctx, &func_ctx) != 0) {
    perror("swapcontext");
    abort();
  }
}
