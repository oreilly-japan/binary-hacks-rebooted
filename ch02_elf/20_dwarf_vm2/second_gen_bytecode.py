from struct import pack

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

def DW_OP_const2u(val):
    assert 0 <= val < 2**16
    return [0xA] + list(pack("<H", val))

def DW_OP_const8u(val):
    assert 0 <= val < 2**64
    return [0xE] + list(pack("<Q", val))

def DW_OP_deref():
    return [0x6]

# DW_OP_reg0, ..., DW_OP_reg31 と
# DW_OP_regx を使い分ける汎用関数
def DW_OP_reg(reg_idx):
    assert 0 <= reg_idx <= 48
    if reg_idx < 32:
        return [0x50 + reg_idx]
    return [0x90] + uleb(reg_idx)

def DW_OP_plus():
    return [0x22]

def print_cfi_escape(payload):
    print(".cfi_escape " + ", ".join(str(i) for i in payload))

# 筆者の環境では以下だが、環境に応じて数値を変える必要あり
# (gdb) p/x (char*)&buf - (char*)main
# $1 = 0x2e5c

payload = []
payload += DW_OP_reg(16)
payload += DW_OP_const2u(0x2E5C)
payload += DW_OP_plus()
print_cfi_escape(DW_CFA_val_expression(15, payload))
