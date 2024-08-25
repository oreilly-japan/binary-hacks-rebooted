# Hack #71 浮動小数点例外

## 例外の状態フラグ

```
$ gcc -frounding-math -Wall -o exception exception.c -lm
$ ./exception
```

発展：`-frounding-math` オプションを外した場合に実行結果は変わるでしょうか。`-frounding-math` を外して、さらに `-O2` オプションを追加した場合は実行結果は変わるでしょうか。

## デフォルト以外の処理方法

### トラップ

x86の場合：

```
$ cc -o trap-sse2 trap-sse2.c
$ ./trap-sse2
```

AArch64の場合：

```
$ cc -o trap-aarch64 trap-aarch64.c
$ ./trap-aarch64
```

使用しているCPU（またはエミュレーター）はトラップをサポートしているでしょうか？

glibcの独自機能：

```
$ cc -o trap-glibc trap-glibc.c -lm
$ ./trap-glibc
```

### フラグを立てない

（x86専用）

```
$ cc -mavx512f -o avx512-sae avx512-sae.c -lm
$ ./avx512-sae       # SAE不使用
$ ./avx512-sae sae 　# SAE使用
```

使用したCPUはAVX-512に対応しているでしょうか？対応していない場合は、Intel SDE等を使ってエミュレートすると良いでしょう。

### flush to zero

（x86またはAArch64に対応）

```
$ cc -o flushtozero flushtozero.c -lm
$ ./flushtozero     # flush to zero不使用
$ ./flushtozero FZ  # flush to zero使用
```

おまけ：32ビットArmの環境を持っている方向けに、「32ビットArmのNEONのSIMD（ベクトル）命令で常にflush to zeroが有効である」ことを確認するプログラムを用意しています。

```
$ cc -o flushtozero-neon flushtozero-neon.c -lm
$ ./flushtozero-neon       # スカラー演算
$ ./flushtozero-neon NEON  # ベクトル演算
```

発展：このプログラムはAArch64でもコンパイル・実行できます。AArch32の場合と実行結果を比較してみましょう。
