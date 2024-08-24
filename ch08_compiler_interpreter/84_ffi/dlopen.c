#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
  int ret;
  void *lib = dlopen("./mylib.so", RTLD_LAZY);
  if (lib == NULL) {
    printf("dlopen failed: %s\n", dlerror());
    return 1;
  }

  // dlsymの戻り値を、my_addの持つシグネチャを表す関数ポインタにキャスト
  int32_t (*my_add)(int32_t, int32_t) = dlsym(lib, "my_add");
  if (my_add == NULL) {
    printf("dlsym failed: %s\n", dlerror());
    return 1;
  }
  printf("1 + 2 = %d\n", my_add(1, 2));

  ret = dlclose(lib);
  if (ret != 0) {
    printf("dlclose failed: %s\n", dlerror());
    return 1;
  }
  return 0;
}
