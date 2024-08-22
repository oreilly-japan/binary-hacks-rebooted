section .text
_start:
    mov rdx, 0xe                 ; 書き込むデータの長さ
    mov rsi, msg                 ; 書き込むデータの先頭アドレス
    mov rdi, 0x1                 ; 書き込み先のファイル記述子
    mov rax, 0x1                 ; writeシステムコールの番号
    syscall                      ; システムコールの呼び出し
    mov rdi, 0x0                 ; 終了ステータス
    mov rax, 0x3c                ; exitシステムコールの番号
    syscall                      ; システムコールの呼び出し
section .rodata
    msg: db "Hello, World!", 0xa ; 0xaは改行コード
