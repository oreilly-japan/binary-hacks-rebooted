# Hack #83 ABIと呼び出し規約を理解する

## ABIがそろっていない場合に起こる現象の例

（x86-64用）

正常な実行例：

```
$ cc -Wall -o mismatch mismatch-main.c mismatch-sub.c
$ ./mismatch
```

ABIが一致しない実行例：

```
$ cc -Wall -c mismatch-main.c
$ cc -Wall -c -mlong-double-64 mismatch-sub.c
$ cc -o mismatch mismatch-main.o mismatch-sub.o
$ ./mismatch
```

## C言語のABI

### コンパイル結果をアセンブリソースとして見る

x86の場合：

```
$ cc -S -masm=intel test.c
$ cc -S -masm=intel vararg.c
```

x86以外の場合：

```
$ cc -S test.c
$ cc -S vararg.c
```

### 可変長引数関数をプロトタイプなしの関数として呼び出すとどうなるか

```
$ cc -Wall -std=c17 -o mismatch-vararg mismatch-vararg.c
$ ./mismatch-vararg
```

引数は正常に受け渡されたでしょうか？Apple Silicon Macの場合はどうでしょうか？

本文に載っていないおまけ：`__m256` 系の型を可変長引数で受け取る関数をプロトタイプなしの関数として呼び出すとどうなるか（x86専用）：

```
$ cc -Wall -std=c17 -mavx -o m256 m256.c
$ ./m256
```

### `va_list` 型を自分で構築する

（x86-64でSystem V ABIを使用している環境専用）

```
$ gcc -Wall -o custom-vararg custom-vararg.c
$ ./custom-vararg
```
