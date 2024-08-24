#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

typedef uint64_t Value;

#define TAG_MASK UINT64_C(0x7FFF000000000000)
#define PAYLOAD_MASK UINT64_C(0x0000FFFFFFFFFFFF)
#define STRING_TAG UINT64_C(0x7FF9000000000000)
#define INT32_TAG UINT64_C(0x7FFA000000000000)

double u64_to_f64(uint64_t u) {
  double d;
  memcpy(&d, &u, 8);
  return d;
}
uint64_t f64_to_u64(double d) {
  uint64_t u;
  memcpy(&u, &d, 8);
  return u;
}

Value box_i32(int32_t i) { return INT32_TAG | (uint32_t)i; }

Value box_string(const char *s) {
  uint64_t p = (uint64_t)(uintptr_t)s;
  assert(p <= UINT64_C(0x0000FFFFFFFFFFFF));
  return STRING_TAG | p;
}

Value box_f64(double d) {
  uint64_t u = f64_to_u64(d);
  if ((u & TAG_MASK) > UINT64_C(0x7FF8000000000000)) {
    return UINT64_C(0x7FF8000000000000);
  }
  return u;
}

void print_value(Value x) {
  switch (x & TAG_MASK) {
    case INT32_TAG: {
      int32_t i32 = (int32_t)(uint32_t)(x & PAYLOAD_MASK);
      printf("int32(%" PRId32 ")\n", i32);
      break;
    }
    case STRING_TAG: {
      const char *s = (const char *)(x & PAYLOAD_MASK);
      printf("string(%s)\n", s);
      break;
    }
    default: {
      double d = u64_to_f64(x);
      printf("double(%g)\n", d);
    }
  }
}

int main(void) {
  print_value(box_i32(INT32_MIN));
  print_value(box_string("Binary Hacks"));
  print_value(box_f64(-1.5));
  print_value(box_f64(0.0 / 0.0));
  double crafted_nan = u64_to_f64(UINT64_C(0x7FFA000000000000));
  print_value(box_f64(crafted_nan));
}
