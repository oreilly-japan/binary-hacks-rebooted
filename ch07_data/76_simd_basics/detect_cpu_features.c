#include <cpuid.h>
#include <stdio.h>

int main(void) {
  unsigned int eax, ebx, ecx, edx;

  // SSE4.1の情報はBasic Information（eax = 1）にある
  __cpuid(1, eax, ebx, ecx, edx);
  int has_sse41 = (ecx & bit_SSE4_1) == bit_SSE4_1;

  // AVX2とAVX-512の情報は
  // Extended Feature Flags（leaf = 7, sub-leaf = 0）にある
  __cpuid_count(7, 0, eax, ebx, ecx, edx);
  int has_avx2 = (ebx & bit_AVX2) == bit_AVX2;
  int has_avx512f = (ebx & bit_AVX512F) == bit_AVX512F;

  printf("SSE4.1: %d, AVX2: %d, AVX-512F: %d\n", has_sse41, has_avx2, has_avx512f);
  return 0;
}
