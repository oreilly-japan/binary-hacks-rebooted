# Hack #75 浮動小数点数のアーキテクチャごとの差異に触れる

## アンダーフローの判定のタイミング

```
$ cc -o tininess tininess.c -lm
$ ./tininess
```

使用した環境ではアンダーフローの判定のタイミングはどちらでしょうか？

## FMAとinvalid operation例外

x86の場合：

```
$ cc -mfma -O -o fma-exception fma-exception.c -lm
$ ./fma-exception
```

それ以外の場合：

```
$ cc -O -o fma-exception fma-exception.c -lm
$ ./fma-exception
```

使用した環境ではFMA(0,∞,qNaN)で例外は発生したでしょうか？x86の場合、`-mfma` オプションを外すと実行結果は変わるでしょうか？あるいは、最適化オプション `-O` を外すと実行結果は変わるでしょうか？

## 浮動小数点数から整数への変換

本文には載っていないおまけとして、浮動小数点数から整数への変換を行う各種やり方を試せるプログラムを用意しています。

標準Cのやり方：

```
$ cc -o fptoint fptoint.c -lm
$ ./fptoint
```

### x86の提供する命令

```
$ cc -o fptoint-sse fptoint-sse.c -lm
$ ./fptoint-sse
```

AVX-512が使える場合：

```
$ cc -mavx512f -o fptoint-sse fptoint-sse.c -lm
$ ./fptoint-sse
```

### AArch64の提供する命令

```
$ cc -march=armv8.5-a -o fptoint-arm fptoint-arm.c -lm
$ ./fptoint-arm tonearest
$ ./fptoint-arm tonearestfromzero
$ ./fptoint-arm upward
$ ./fptoint-arm downward
$ ./fptoint-arm towardzero
$ ./fptoint-arm jcvt     # FEAT_JSCVTが使える場合
$ ./fptoint-arm rint32z  # FEAT_FRINTTSが使える場合
$ ./fptoint-arm rint32x  # FEAT_FRINTTSが使える場合
```

## Armの代替動作(FEAT_AFP)

本文には載っていないおまけとして、Armの代替動作を試せるプログラムを用意しています。といっても、筆者はFEAT_AFPに対応したCPUを持っていないので動作確認はできていませんが……。

（AArch64専用）

```
$ cc -o arm-altfp arm-altfp.c -lm
$ ./arm-altfp
```
