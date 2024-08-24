#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

// コルーチンで実行する関数の型。引数はユーザデータで、普通は汎用性のため、
// pthread_create のように void* にするでしょうが、ここでは簡単のため int
// を使います
typedef void (*coro_fn)(int);

typedef struct {
  // 実行する関数とその引数
  coro_fn fn;
  int arg;
  // coro_yield に渡されて、 coro_resume に戻すべき値を保持する
  int ret;
  // コルーチンと呼び出し元のコンテキスト
  ucontext_t coro_ctx;
  ucontext_t main_ctx;
  // コルーチンが使用するスタック領域
  char stack[4096];
} coro_type;

// 現在のスレッドで実行中のコルーチン。コルーチン実行中でない場合は NULL です。
// 今回の設計ではコルーチンの中で別のコルーチンが呼び出せない仕様としています。
_Thread_local coro_type* g_coro;

coro_type* coro_create(coro_fn fn, int arg) {
  coro_type* coro = (coro_type*)malloc(sizeof(coro_type));
  coro->fn = fn;
  coro->arg = arg;

  if (getcontext(&coro->coro_ctx) != 0) {
    perror("getcontext");
    abort();
  }
  coro->coro_ctx.uc_stack.ss_sp = coro->stack;
  coro->coro_ctx.uc_stack.ss_flags = 0;
  coro->coro_ctx.uc_stack.ss_size = sizeof(coro->stack);
  coro->coro_ctx.uc_link = &coro->main_ctx;
  makecontext(&coro->coro_ctx, (void (*)())coro->fn, 1, coro->arg);
  return coro;
}

int coro_resume(coro_type* coro) {
  // 簡易実装のため、コルーチンの中でコルーチンを呼ぶことは禁止
  assert(!g_coro);
  g_coro = coro;
  // swapcontext を使って、ここに戻ってくるためのコンテキストを main_ctx に
  // 保存しつつ、コルーチンの実行コンテキスト coro_ctx に切り替える
  if (swapcontext(&coro->main_ctx, &coro->coro_ctx) != 0) {
    perror("swapcontext");
    abort();
  }

  if (g_coro) {
    // コルーチンが終了した場合
    g_coro = NULL;
    return -1;
  } else {
    // coro_yield でコルーチンが中断された場合
    return coro->ret;
  }
}

void coro_yield(int ret) {
  coro_type* coro = g_coro;
  // g_coro をリセットして、中断されたことを `coro_resume` に伝える
  g_coro = NULL;
  coro->ret = ret;

  if (swapcontext(&coro->coro_ctx, &coro->main_ctx) != 0) {
    perror("swapcontext");
    abort();
  }
}

void iota(int end) {
  puts("[coro] iota start");
  for (int n = 0; n < end; ++n) {
    printf("[coro] resuming from iota %d\n", n);
    coro_yield(n);
    printf("[coro] returned to iota %d\n", n);
  }
  puts("[coro] iota end");
}

int main(void) {
  coro_type* coro = coro_create(iota, 3);
  for (int n = 0;; ++n) {
    printf("[main] entering iota %d\n", n);
    int ret = coro_resume(coro);
    printf("[main] returned from itoa %d (ret=%d)\n", n, ret);
    if (ret < 0) {
      return 0;
    }
  }
}
