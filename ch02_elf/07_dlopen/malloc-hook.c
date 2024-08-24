// GNU拡張であるRTLD_NEXTをつかうために_GNU_SOURCEを定義
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

// オリジナルのmalloc関数へのポインタを保存
// mallocの初回呼び出し時に初期化される
static void *(*real_malloc)(size_t) = NULL;

// mallocのラッパ関数
void *malloc(size_t size) {
  // 初回呼び出し時にreal_mallocを初期化
  if (real_malloc == NULL) {
    // RTLD_NEXTにより、これより後にロードされた共有ライブラリのmallocをみつける
    real_malloc = dlsym(RTLD_NEXT, "malloc");

    if (real_malloc == NULL) {
      fprintf(stderr, "dlsym error: %s\n", dlerror());
      exit(1);
    }
  }

  // ちなみに、（実装依存だが）ここの出力先をstdoutにすると
  // 内部でバッファリングのためにmallocが呼ばれて無限再帰になってしまう
  fprintf(stderr, "malloc: size=%zu\n", size);

  // オリジナルのmallocを呼び出す
  return real_malloc(size);
}
