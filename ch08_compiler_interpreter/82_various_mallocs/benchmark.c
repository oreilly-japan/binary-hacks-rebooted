#include <stdlib.h>

#define N 10000
void *mems[N];

int main(void) {
  for (size_t i = 0; i < N; i++) {
    mems[i] = malloc(rand() % (1LL << 32));
  }
  for (size_t i = 0; i < N; i++) {
    size_t j = rand() % (N - 1);
    void *t = mems[j];
    mems[j] = mems[j + 1];
    mems[j + 1] = t;
  }
  for (size_t i = 0; i < N; i++) {
    free(mems[i]);
  }
}
