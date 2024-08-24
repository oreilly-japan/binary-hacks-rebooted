void add16(const float *a, const float *b, float *restrict c) {
  #pragma omp simd simdlen(8)
  for (int i = 0; i < 16; i++) {
    c[i] = a[i] + b[i];
  }
}
