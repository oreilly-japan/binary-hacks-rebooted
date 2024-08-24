#include <dlfcn.h>
#include <stdio.h>

int main(void) {
  // libhello.soをロードする
  void *lib = dlopen("./libhello.so", RTLD_LAZY);
  if (lib == NULL) {
    printf("dlopen failed: %s\n", dlerror());
    return 1;
  }

  int (*hello)(void);
  // dlsymでhello関数のアドレスを取得する
  hello = dlsym(lib, "hello");
  if (hello == NULL) {
    printf("dlsym failed: %s\n", dlerror());
    return 1;
  }

  // libhello.so内に定義されたhello関数を呼び出す
  hello();

  // libhello.soをアンロードする
  int ret = dlclose(lib);
  if (ret != 0) {
    printf("dlclose failed: %s\n", dlerror());
    return 1;
  }
  return 0;
}
