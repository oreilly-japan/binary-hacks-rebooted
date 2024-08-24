#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

atomic_int length;
int data[4096];
int sum;
static const int UP_TO = 200;

void *consume(void *arg) {
  for (int i = 0; i < UP_TO; i++) {
    // produceがデータを生成するまで待機
    while (length == 0) {
      sched_yield();
    }
    // length未満のインデックスは常にデータがあるので消費する
    sum += data[length - 1];
    // 消費したデータ一つ分インデックスを減らす
    length--;
  }
  return NULL;
};

void *produce(void *arg) {
  for (int i = 1; i <= UP_TO; i++) {
    // lengthインデックスにはまだデータがないのでデータを置く
    data[length] = i;
    // lengthを伸ばしてconsumeが消費できるようにする
    length++;
  }
  return NULL;
}

int main(void) {
  pthread_t threads[2];
  pthread_create(&threads[0], NULL, consume, NULL);
  pthread_create(&threads[1], NULL, produce, NULL);

  for (int i = 0; i < sizeof(threads) / sizeof(threads[0]); i++) {
    pthread_join(threads[i], NULL);
  }
  printf("sum: %d\n", sum);
}
