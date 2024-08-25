# Hack #70 浮動小数点数のビット列表現を理解する

## 浮動小数点数のビット列表現（２進）

```
$ cc -o f32rep f32rep.c
$ ./f32rep
```

## 浮動小数点数のビット列表現（10進）

> [!CAUTION]
> 2024年8月現在、C言語で10進浮動小数点型を試すにはGCCが必要です。macOSでのデフォルトの `gcc` の実体はClangなので、macOSユーザーの方は注意してください。

```
$ gcc -o d32rep d32rep.c
$ ./d32rep
```

PowerPC Linuxでの実行結果を確認する場合（QEMU使用）：

```
$ sudo apt-get install qemu-user gcc-powerpc-linux-gnu
$ powerpc-linux-gnu-gcc -static -o d32rep.ppc d32rep.c
$ qemu-ppc ./d32rep.ppc
```
