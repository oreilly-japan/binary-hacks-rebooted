; youkoso.asm
section .text
    global _start
section .text
_start:
    jmp caller
callee:
    mov rdx,15
    pop rsi
    mov rdi,1
    mov rax,1
    syscall
    pop rdx
    pop rcx
    pop rbx
    pop rax
    jmp [rel entry_addr]
caller:
    push rax
    push rbx
    push rcx
    push rdx
    call callee
    db  'Youkoso Sekai!',0x0a
entry_addr:
    dq 0x401650
