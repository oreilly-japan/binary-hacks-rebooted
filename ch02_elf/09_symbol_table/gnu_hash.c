static uint_fast32_t dl_new_hash(const char *s) {
  uint_fast32_t h = 5381;
  for (unsigned char c = *s; c != '\0'; c = *++s) {
    h = h * 33 + c;
  }
  return h & 0xffffffff;
}
