/* symbol_name のハッシュ値を計算する */
uint32_t hash = elf_hash(symbol_name);

for (uint32_t symidx = bucket[hash % nbuckets]; symidx != STN_UNDEF;
     symidx = chain[symidx]) {
  /* symidx が指すシンボルがsymbol_nameと一致するか調べる。
   * 一致すれば NULLでないポインタを返す。 */
  sym = check_match(symidx, symbol_name);
  if (sym != NULL) {
    /* 見つかった */
    goto found_it;
  }
}
