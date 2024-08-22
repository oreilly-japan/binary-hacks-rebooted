# dwarf3のスクリプトだけは、Dockerのubuntu 20.04上で動くように
# 本誌のものから改変してある。
# ただし、aptのパッケージ更新などにより、
# libgccなどに更新が入った場合、このスクリプトでも動く保証はない

from struct import pack

class LabelResolver:
    def __init__(self, payload):
        self.payload = payload
        self.name2addr = {}
        self.wait_list = {}

    def add_inst(self, inst):
        self.payload += inst

    def cur_addr(self):
        return len(self.payload)

    def label(self, name):
        addr = self.cur_addr()
        self.name2addr[name] = addr

        if name in self.wait_list:
            for pos, callback, len_tmp_inst in self.wait_list[name]:
                real_inst = callback(addr, pos + len_tmp_inst)
                assert len(real_inst) == len_tmp_inst

                for i in range(len_tmp_inst):
                    self.payload[pos + i] = real_inst[i]

            del self.wait_list[name]

    def label_addr(self, name):
        return self.name2addr[name]

    def resolve_label(self, name, callback):
        if name in self.name2addr:
            addr = self.label_addr(name)
            tmp_inst = callback(0, 0)
            nxt_ip = self.cur_addr() + len(tmp_inst)
            real_inst = callback(addr, nxt_ip)
            assert len(tmp_inst) == len(real_inst)
            self.add_inst(real_inst)
        else:
            if not name in self.wait_list:
                self.wait_list[name] = []

            tmp_inst = callback(0, 0)
            self.wait_list[name].append((self.cur_addr(), callback, len(tmp_inst)))
            self.add_inst(tmp_inst)

    def get_payload(self):
        assert not self.wait_list
        return self.payload

def _pad_in_7(bs):
    ret = []
    n = len(bs)
    reved_bs = bs[::-1]
    for i in range(0, n, 7):
        does_continue = i + 7 < n
        ret.append(int("01"[does_continue] + reved_bs[i : i + 7][::-1], 2))
    return ret

def uleb(val):
    assert 0 <= val < 2**128
    bs = bin(val)[2:]
    return _pad_in_7(bs)

def DW_CFA_val_expression(reg_idx, block):
    assert 0 <= reg_idx <= 48
    return [0x16, reg_idx] + uleb(len(block)) + block

def DW_OP_const1u(val):
    assert 0 <= val < 2**8
    return [0x8] + list(pack("<B", val))

def DW_OP_const2u(val):
    assert 0 <= val < 2**16
    return [0xA] + list(pack("<H", val))

def DW_OP_const8u(val):
    assert 0 <= val < 2**64
    return [0xE] + list(pack("<Q", val))

def DW_OP_deref():
    return [0x6]

def DW_OP_deref_size(size):
    assert 1 <= size <= 8
    return [0x94] + [size]

def DW_OP_dup():
    return [0x12]

def DW_OP_drop():
    return [0x13]

def DW_OP_over():
    return [0x14]

def DW_OP_swap():
    return [0x16]

def DW_OP_rot():
    return [0x17]

def DW_OP_minus():
    return [0x1C]

def DW_OP_div():
    return [0x1B]

def DW_OP_mod():
    return [0x1D]

def DW_OP_mul():
    return [0x1E]

def DW_OP_shl():
    return [0x24]

# DW_OP_reg0, ..., DW_OP_reg31 と
# DW_OP_regx を使い分ける汎用関数
def DW_OP_reg(reg_idx):
    assert 0 <= reg_idx <= 48
    if reg_idx < 32:
        return [0x50 + reg_idx]
    return [0x90] + uleb(reg_idx)

def DW_OP_plus():
    return [0x22]

def DW_OP_eq():
    return [0x29]

def DW_OP_ne():
    return [0x2E]

def DW_OP_bra(offset):
    return [0x28] + list(pack("<h", offset))

def DW_OP_skip(offset):
    return [0x2F] + list(pack("<h", offset))

def print_cfi_escape(payload):
    print(".cfi_escape " + ", ".join(str(i) for i in payload))

# rsp を VM のスタックのアドレスに設定する
payload = []
payload += DW_OP_reg(7)
payload += DW_OP_const2u(0x870)
payload += DW_OP_minus()
print_cfi_escape(DW_CFA_val_expression(7, payload))

resolver = LabelResolver([])

# スタックに入っている初期値を捨てる
resolver.add_inst(DW_OP_drop())


# (gdb) p/x (char*)&buf - (char*)main
# $1 = 0x2dfc

resolver.add_inst(DW_OP_reg(16))
resolver.add_inst(DW_OP_const2u(0x2dfc))
resolver.add_inst(DW_OP_plus())  # ptr = buf

# loop_start:
resolver.label("LOOP_START")
resolver.add_inst(DW_OP_dup())
resolver.add_inst(DW_OP_deref_size(1))  # push *ptr

# cmp *ptr, '\0'; jnz out_of_loop
resolver.add_inst(DW_OP_dup())
resolver.add_inst(DW_OP_const1u(ord("\0")))
resolver.add_inst(DW_OP_eq())
resolver.resolve_label("OUT_OF_LOOP", lambda addr, nxt_ip: DW_OP_bra(addr - nxt_ip))

# cmp *ptr, '+'; jnz plus_end
resolver.add_inst(DW_OP_dup())
resolver.add_inst(DW_OP_const1u(ord("+")))
resolver.add_inst(DW_OP_ne())
resolver.resolve_label("PLUS_END", lambda addr, nxt_ip: DW_OP_bra(addr - nxt_ip))

# plus_start:
resolver.add_inst(DW_OP_drop())
resolver.add_inst(DW_OP_rot())
resolver.add_inst(DW_OP_plus())
resolver.add_inst(DW_OP_swap())
resolver.resolve_label("LOOP_FINALLY", lambda addr, nxt_ip: DW_OP_skip(addr - nxt_ip))

# plus_end:
resolver.label("PLUS_END")
# cmp *ptr, '*'; jnz mul_end
resolver.add_inst(DW_OP_dup())
resolver.add_inst(DW_OP_const1u(ord("*")))
resolver.add_inst(DW_OP_ne())
resolver.resolve_label("MUL_END", lambda addr, nxt_ip: DW_OP_bra(addr - nxt_ip))

# mul_start:
resolver.add_inst(DW_OP_drop())
resolver.add_inst(DW_OP_rot())
resolver.add_inst(DW_OP_mul())
resolver.add_inst(DW_OP_swap())
resolver.resolve_label("LOOP_FINALLY", lambda addr, nxt_ip: DW_OP_skip(addr - nxt_ip))

# mul_end:
resolver.label("MUL_END")
# otherwise, *ptr = '0', ..., '9'
resolver.add_inst(DW_OP_const1u(ord("0")))
resolver.add_inst(DW_OP_minus())
resolver.add_inst(DW_OP_swap())

# loop_finally:
resolver.label("LOOP_FINALLY")
# ++ptr; jmp loop_start
resolver.add_inst(DW_OP_const1u(1))
resolver.add_inst(DW_OP_plus())
resolver.resolve_label("LOOP_START", lambda addr, nxt_ip: DW_OP_skip(addr - nxt_ip))

# out_of_loop:
resolver.label("OUT_OF_LOOP")
# pop; pop
resolver.add_inst(DW_OP_drop())
resolver.add_inst(DW_OP_drop())

for i in range(28):
    resolver.add_inst(DW_OP_const1u(i))
    resolver.add_inst(DW_OP_swap())

# rsp-0x870 （に入る値をプッシュ）
#  pop rbp ; ret
# (gdb) p/x (int)main - 0x11a3
# $1 = 0xa1
resolver.add_inst(DW_OP_reg(16))
resolver.add_inst(DW_OP_const1u(0xA1))
resolver.add_inst(DW_OP_minus())
# 数式の評価結果が入ってるので入れ替える
resolver.add_inst(DW_OP_swap())

# rsp-0x868
#  rsp-0x858 + 0x18
resolver.add_inst(DW_OP_reg(7))
resolver.add_inst(DW_OP_const2u(0x858 - 0x18))
resolver.add_inst(DW_OP_minus())
resolver.add_inst(DW_OP_swap())

# rsp-0x860:
#  mov -0x18(%rbp),%rax ; ... ; puts
# (gdb) p/x 0x1287 - (int)main
# $2 = 0x43
resolver.add_inst(DW_OP_reg(16))
resolver.add_inst(DW_OP_const1u(0x43))
resolver.add_inst(DW_OP_plus())
resolver.add_inst(DW_OP_swap())

# rsp-0x858:
#  pointer to result string
resolver.add_inst(DW_OP_reg(7))
resolver.add_inst(DW_OP_const2u(0x830))
resolver.add_inst(DW_OP_minus())
resolver.add_inst(DW_OP_swap())

# rsp-0x850 ~ rsp-0x840
for i in range(3):
    resolver.add_inst(DW_OP_const1u(i))
    resolver.add_inst(DW_OP_swap())

# rsp-0x838 (old_rbp+8)
#  _start
# (gdb) p/x main - _start
# $3 = 0x174
resolver.add_inst(DW_OP_reg(16))
resolver.add_inst(DW_OP_const2u(0x174))
resolver.add_inst(DW_OP_minus())
resolver.add_inst(DW_OP_swap())

# rsp-0x830
#  result string

resolver.add_inst(DW_OP_const1u(0))

for i in range(18, -1, -1):
    digit_idx = (18 - i) % 8

    resolver.add_inst(DW_OP_over())

    # 下から i+1 桁目を取り出して文字にする
    if i != 18:
        resolver.add_inst(DW_OP_const8u(10 ** (i + 1)))
        resolver.add_inst(DW_OP_mod())

    resolver.add_inst(DW_OP_const8u(10**i))
    resolver.add_inst(DW_OP_div())
    resolver.add_inst(DW_OP_const1u(ord("0")))
    resolver.add_inst(DW_OP_plus())
    resolver.add_inst(DW_OP_const1u(digit_idx * 8))
    resolver.add_inst(DW_OP_shl())
    resolver.add_inst(DW_OP_plus())

    # 8 バイト分の文字が溜まったらプッシュ
    if digit_idx == 7:
        resolver.add_inst(DW_OP_swap())
        resolver.add_inst(DW_OP_const1u(0))

resolver.add_inst(DW_OP_swap())
resolver.add_inst(DW_OP_drop())

print_cfi_escape(DW_CFA_val_expression(15, resolver.get_payload()))
