# Hack #73 浮動小数点環境を触るコードに対するコンパイラの最適化と戦う

## コンパイラの最適化によって意図通りに動かなくなる例

```
$ cc -O0 -o fpopt0-O0 fpopt0.c -lm
$ ./fpopt0-O0
$ cc -O2 -o fpopt0-O2 fpopt0.c -lm
$ ./fpopt0-O2
```

## C標準のプラグマ

```
$ cc -Wall -O2 -o fpopt1 fpopt1.c -lm
$ ./fpopt1
```

使用したコンパイラは `#pragma STDC FENV_ACCESS` に対応しているでしょうか？

## volatileの利用

```
$ cc -Wall -O2 -o fpopt2 fpopt2.c -lm
$ ./fpopt2
```

発展：引数の `volatile` 指定を外して同様にコンパイル・実行すると実行結果はどうなるでしょうか？

## インラインアセンブリの利用

```
$ cc -Wall -O2 -o fpopt3 fpopt3.c -lm
$ ./fpopt3
```
