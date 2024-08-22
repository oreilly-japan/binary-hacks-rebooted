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

def DW_OP_const8u(val):
    assert 0 <= val < 2**64
    return [0xE] + list(pack("<Q", val))

def DW_OP_deref():
    return [0x6]

def print_cfi_escape(payload):
    print(".cfi_escape " + ", ".join(str(i) for i in payload))

payload = []
payload += DW_OP_const8u(0xDEADBEEF)
payload += DW_OP_deref()
print_cfi_escape(DW_CFA_val_expression(15, payload))
