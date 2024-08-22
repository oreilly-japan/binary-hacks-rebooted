#include <dlfcn.h>
#include <ffi.h>
#include <stdio.h>

#if defined(__APPLE__)
#define LIBC_SO "libSystem.dylib"
#else
#include <gnu/lib-names.h>
#endif

#define MAX_ARGS_NUM 10

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <libc function> [args..]\n", argv[0]);
    return 1;
  }

  int n_args = argc - 2;
  if (n_args > MAX_ARGS_NUM) {
    printf("Too many arguments\n");
    return 1;
  }

  void *lib = dlopen(LIBC_SO, RTLD_LAZY);
  if (lib == NULL) {
    printf("dlopen error: %s\n", dlerror());
    return 1;
  }

  void *libc_func = dlsym(lib, argv[1]);
  if (libc_func == NULL) {
    printf("dlsym error: %s\n", dlerror());
    return 1;
  }

  ffi_type *arg_types[MAX_ARGS_NUM];
  void *arg_values[MAX_ARGS_NUM];
  int int_args[MAX_ARGS_NUM];

  // 引数の型と値をそれぞれ arg_types と arg_values に格納する
  for (int i = 0; i < n_args; i++) {
    // 整数としてパースできるならint、そうでなければ文字列ポインタ
    // ちなみに、この実装だと"123"のような文字列を整数として扱ってしまうので
    // 正しく実装する場合は型情報も受け取る必要がある
    if (sscanf(argv[i + 2], "%d", &int_args[i]) == 1) {
      arg_types[i] = &ffi_type_sint;
      arg_values[i] = &int_args[i];
    } else {
      arg_types[i] = &ffi_type_pointer;
      arg_values[i] = &argv[i + 2];
    }
  }

  // ffi_cif構造体を初期化する
  ffi_cif cif;
  // 簡単のため、戻り値の型はintに固定
  ffi_type *return_type = &ffi_type_sint;
  ffi_status status = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, n_args,
                                   return_type, arg_types);
  if (status != FFI_OK) {
    printf("ffi_prep_cif error: %d\n", status);
    return 1;
  }

  // 関数を呼び出す
  ffi_arg result;
  ffi_call(&cif, FFI_FN(libc_func), &result, arg_values);

  int ret = dlclose(lib);
  if (ret != 0) {
    printf("dlclose error: %s\n", dlerror());
    return 1;
  }
}
