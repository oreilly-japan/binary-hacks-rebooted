#!/usr/bin/env python3
import ctypes

# 自前のmylib.soをロードし、add関数を呼ぶ
mylib = ctypes.cdll.LoadLibrary("./mylib.so")
answer = mylib.my_add(1, 2)
print(f"1 + 2 = {answer}")

# libcをロードし、printfを呼ぶ
try:
    libc = ctypes.cdll.LoadLibrary("libc.so.6")
    libc.printf(b"Binary Hacks R%xted!\n", 60160)
except OSError:
    print("libc.so.6 not available")
