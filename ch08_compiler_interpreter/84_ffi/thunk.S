#if defined(__APPLE__)
#define SYMBOL(name) _ ## name
#else
#define SYMBOL(name) name
#endif
        .intel_syntax noprefix
        .globl SYMBOL(thunk)
SYMBOL(thunk):
        # rdi: func
        # rsi: regargs
        # rdx: number of bytes of stack args (multiple of 16)
        # rcx: stack args
        # r10, r11: temporaries
        push rbp
        mov rbp, rsp
        push rdi
        push rsi
        sub rsp, rdx
        mov rdi, rsp
        mov rsi, rcx
        call SYMBOL(memcpy)
        mov r11, qword ptr [rbp - 8]
        mov r10, qword ptr [rbp - 16]
        mov rdi, qword ptr [r10] # 1st arg
        mov rsi, qword ptr [r10 + 8] # 2nd arg
        mov rdx, qword ptr [r10 + 16] # 3rd arg
        mov rcx, qword ptr [r10 + 24] # 4th arg
        mov r8, qword ptr [r10 + 32] # 5th arg
        mov r9, qword ptr [r10 + 40] # 6th arg
        call r11
        leave
        ret
