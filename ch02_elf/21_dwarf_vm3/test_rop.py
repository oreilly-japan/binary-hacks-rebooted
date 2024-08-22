# dwarf3のスクリプトだけは、Dockerのubuntu 20.04上で動くように
# 本誌のものから改変してある。
# ただし、aptのパッケージ更新などにより、
# libgccなどに更新が入った場合、このスクリプトでも動く保証はない

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

def DW_OP_reg(reg_idx):
    assert 0 <= reg_idx <= 48
    if reg_idx < 32:
        return [0x50 + reg_idx]
    return [0x90] + uleb(reg_idx)

def DW_OP_const1u(val):
    assert 0 <= val < 2**8
    return [0x8] + list(pack("<B", val))

def DW_OP_const2u(val):
    assert 0 <= val < 2**16
    return [0xA] + list(pack("<H", val))

def DW_OP_const8u(val):
    assert 0 <= val < 2**64
    return [0xE] + list(pack("<Q", val))

def DW_OP_drop():
    return [0x13]

def DW_OP_minus():
    return [0x1C]

def print_cfi_escape(payload):
    print(".cfi_escape " + ", ".join(str(i) for i in payload))

payload = []
payload += DW_OP_drop()

for i in range(28):
    payload += DW_OP_const1u(0xCC)  # unshown

payload += DW_OP_const8u(0xDEADBEEF)
payload += DW_OP_const8u(0xFEEDFACE)

payload += DW_OP_reg(7)
payload += DW_OP_const2u(0x870)
payload += DW_OP_minus()

print_cfi_escape(DW_CFA_val_expression(7, payload))
