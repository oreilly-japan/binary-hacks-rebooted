#include <gmp.h>

int main(void) {
  mpz_t a, b, c;
  mpz_init(a);  // mpz_t型の変数は使用前に初期化が必要
  mpz_init(b);
  mpz_init(c);

  mpz_set_si(a, 5);         // 5を代入
  mpz_pow_ui(b, a, 100);    // 100乗
  mpz_add_ui(c, b, 1);      // +1

  gmp_printf("%Zi\n", c);

  mpz_clear(a); // 使用後はメモリを解放する
  mpz_clear(b);
  mpz_clear(c);
}
