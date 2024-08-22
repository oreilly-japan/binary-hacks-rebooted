uint8_t* to_shadow_addr(uint8_t* ptr) {
  return (uint8_t*)(((uint64_t)ptr >> 3) + SHADOW_MEM_OFFSET);
}

int main(void) {
  uint64_t index = 12;
  if (*to_shadow_addr((uint8_t*)(array + index))) {
    report_and_exit();
  }
  array[index] = 42;
  printf("%lu\n", array[index]);
  return 0;
}
