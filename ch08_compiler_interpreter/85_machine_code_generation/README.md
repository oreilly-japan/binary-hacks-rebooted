# Hack #85 実行時に機械語を生成する

## 例：関数を部分適用する

x86-64 Unix向け：

```
$ cc -o bind_x86_64_sysv bind_x86_64_sysv.c
$ ./bind_x86_64_sysv
```

x86-64 Windows向け（MSYS2等を使う場合）：

```
$ cc -o bind_x86_64_win.exe bind_x86_64_win.c
$ ./bind_x86_64_win.exe
```

x86-64 Windows向け（MSVCを使う場合）：

```
> cl /nologo bind_x86_64_win.c
> bind_x86_64_win.exe
```

AArch64 Unix向け：

```
$ cc -o bind_aarch64_unix bind_aarch64_unix.c
$ ./bind_aarch64_unix
```

Apple Silicon Macでは「AArch64 Unix向け」のプログラムは動作するでしょうか？AArch64 Linuxの場合はどうでしょうか？

## メモリの実行許可

x86-64 Unix向け：

```
$ cc -o bind_x86_64_sysv_nowx bind_x86_64_sysv_nowx.c
$ ./bind_x86_64_sysv_nowx
```

AArch64 Unix向け：

```
$ cc -o bind_aarch64_unix_nowx bind_aarch64_unix_nowx.c
$ ./bind_aarch64_unix_nowx
```

Apple Silicon Macで `MAP_JIT` を使う例：

```
$ cc -o bind_aarch64_apple_map_jit bind_aarch64_apple_map_jit.c
$ ./bind_aarch64_apple_map_jit
```

## Apple Silicon Mac上での `mprotect` と `pthread_jit_write_protect_np` の速度の比較

本文に

> JITコンパイラが頻繁にコード書き換えを行う場合、システムコールである `mprotect` を毎回呼び出すと遅いです。

と書きましたが、それを検証できるプログラムを用意しました。といっても、時間の計測に使っているのが `clock` 関数なので、測定は簡易的なものです。

実行例：

```
$ cc -O2 -o macos_map_jit_benchmark macos_map_jit_benchmark.c 
$ ./macos_map_jit_benchmark
```

筆者の環境（Apple M1, macOS 13）での実行結果は以下の通りでした：

```
pthread_jit_write_protect_supported_np() = 1
mprotect: 269.325 ms
pthread_jit_write_protect_np: 3.762 ms
```

筆者の環境での `mprotect` と `pthread_jit_write_protect_np` の速度差は70倍程度となりました。皆さんの環境ではどうでしょうか？
