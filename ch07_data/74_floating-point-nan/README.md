# Hack #74 NaNを深掘りする

## signaling NaN

通常の例：

```
$ cc -o snan1 snan1.c -lm
$ ./snan1
```

悪い例：

```
$ cc -o snan2 snan2.c -lm
$ ./snan2
```

悪い例（32ビットx86の場合）：

```
$ i686-linux-gnu-gcc -o snan2 snan2.c -lm
$ ./snan2
```

使用した環境で構築されたのはsignaling NaNでしょうか？32ビットx86ではどうでしょうか？

C23の例：

```
$ cc -std=c2x -o snan3 snan3.c -lm
$ ./snan3
```

## NaNのペイロードと伝播

```
$ cc -o nan-propagation -Wall nan-propagation.c -lm
$ ./nan-propagation
```

環境によってNaNのペイロードの伝播の方法が違うということを本文で説明しました。使用した環境ではどのように伝播したでしょうか？

また、最適化を有効にするとどうなるでしょうか？コンパイラを変える（GCC↔︎Clang）とどうでしょうか？

```
$ cc -O2 -o nan-propagation-O2 -Wall nan-propagation.c -lm
$ ./nan-propagation-O2
```

## RISC-Vの浮動小数点レジスタとNaN

QEMU使用：

```
$ riscv64-linux-gnu-gcc -Wall -static -o riscv-fpreg riscv-fpreg.c riscv-fpreg-sub.s
$ qemu-riscv64 ./riscv-fpreg
```
