section .text
    global _start
section .text
_start:
    call callee
    db  'Hello, World!',0x0a     ; 0xaは改行コード
callee:
    mov rdx, 0xe                 ; 書き込むデータの長さ
    pop rsi                      ; 書き込むデータの先頭アドレス
                                 ; call calleeの次の命令のアドレスがpopされ、
                                 ; rsiに格納される。
    mov rdi, 0x1                 ; 書き込み先のファイル記述子
    mov rax, 0x1                 ; writeシステムコールの番号
    syscall                      ; システムコールの呼び出し
    mov rdi, 0x0                 ; 終了ステータス
    mov rax, 0x3c                ; exitシステムコールの番号
    syscall                      ; システムコールの呼び出し
