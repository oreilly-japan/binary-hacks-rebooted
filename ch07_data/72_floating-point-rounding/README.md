# Hack #72 浮動小数点数の丸め方を変える

## 動的な指定

```
$ cc -o setround1 setround1.c -lm
$ ./setround1
$ cc -o setround2 setround2.c -lm
$ ./setround2
```

## 静的な指定

x86 AVX-512の場合：

```
$ cc -mavx512f -o rounding-avx512 rounding-avx512.c
$ ./rounding-avx512
```

使用したCPUはAVX-512に対応しているでしょうか？対応していない場合は、Intel SDE等を使ってエミュレートすると良いでしょう。

RISC-Vの場合（QEMU使用）：

```
$ sudo apt-get install qemu-user gcc-riscv64-linux-gnu
$ riscv64-linux-gnu-gcc -static -o rounding-riscv rounding-riscv.c
$ qemu-riscv64 ./rounding-riscv
```

CUDAの場合：

```
$ nvcc -o rounding-cuda rounding-cuda.cu
$ ./rounding-cuda
```

C23の場合：

> [!CAUTION]
> 執筆時点（2024年8月）ではC23の `#pragma STDC FENV_ROUND` に対応したコンパイラは存在しません。

```
$ cc -Wall -std=c23 -o rounding-c23 rounding-c23.c
$ ./rounding-c23
```
