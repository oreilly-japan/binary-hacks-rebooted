#include <stdio.h>
#include <stdlib.h>

__global__ void add(const float *a, const float *b,
                    float *result_ne, float *result_up,
                    float *result_down, float *result_zero) {
  int i = threadIdx.x;
  result_ne[i] = __fadd_rn(a[i], b[i]);
  result_up[i] = __fadd_ru(a[i], b[i]);
  result_down[i] = __fadd_rd(a[i], b[i]);
  result_zero[i] = __fadd_rz(a[i], b[i]);
}

void checkError(cudaError_t err, const char *name) {
  if (err != cudaSuccess) {
    fprintf(stderr, "%s: %s\n", name, cudaGetErrorString(err));
    exit(1);
  }
}

int main() {
  size_t size = sizeof(float);
  float a = 0x1p0f;
  float b = 0x1p-100f;
  float result_ne, result_up, result_down, result_zero;

  cudaError_t err;
  float *d_a, *d_b, *d_result_ne, *d_result_up, *d_result_down, *d_result_zero;
  err = cudaMalloc(&d_a, size);
  checkError(err, "cudaMalloc");
  err = cudaMalloc(&d_b, size);
  checkError(err, "cudaMalloc");
  err = cudaMalloc(&d_result_ne, size);
  checkError(err, "cudaMalloc");
  err = cudaMalloc(&d_result_up, size);
  checkError(err, "cudaMalloc");
  err = cudaMalloc(&d_result_down, size);
  checkError(err, "cudaMalloc");
  err = cudaMalloc(&d_result_zero, size);
  checkError(err, "cudaMalloc");

  err = cudaMemcpy(d_a, &a, size, cudaMemcpyHostToDevice);
  checkError(err, "cudaMemcpy");
  err = cudaMemcpy(d_b, &b, size, cudaMemcpyHostToDevice);
  checkError(err, "cudaMemcpy");

  add<<<1, 1>>>(d_a, d_b, d_result_ne, d_result_up, d_result_down,
                d_result_zero);

  err = cudaMemcpy(&result_ne, d_result_ne, size, cudaMemcpyDeviceToHost);
  checkError(err, "cudaMemcpy");
  err = cudaMemcpy(&result_up, d_result_up, size, cudaMemcpyDeviceToHost);
  checkError(err, "cudaMemcpy");
  err = cudaMemcpy(&result_down, d_result_down, size, cudaMemcpyDeviceToHost);
  checkError(err, "cudaMemcpy");
  err = cudaMemcpy(&result_zero, d_result_zero, size, cudaMemcpyDeviceToHost);
  checkError(err, "cudaMemcpy");

  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_result_ne);
  cudaFree(d_result_up);
  cudaFree(d_result_down);
  cudaFree(d_result_zero);

  printf("nearesteven: %a\n", result_ne);
  printf("up: %a\n", result_up);
  printf("down: %a\n", result_down);
  printf("zero: %a\n", result_zero);
}
