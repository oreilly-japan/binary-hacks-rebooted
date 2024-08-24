#include "xbyak/xbyak.h"

class Add16 : public Xbyak::CodeGenerator {
public:
  Add16() {
    vmovups(zmm0, ptr [rdi]);
    vaddps(zmm0, zmm0, ptr [rsi]);
    vmovups(ptr [rdx], zmm0);
    vzeroupper();
    ret();
  }
};
