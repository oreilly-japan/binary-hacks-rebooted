	.text
	.intel_syntax noprefix
	.file	"array.c"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
# %bb.0:
	push	rax
	mov	qword ptr [rip + array+96], 42
	lea	rdi, [rip + .L.str]
	mov	esi, 42
	xor	eax, eax
	call	printf@PLT
	xor	eax, eax
	pop	rcx
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.type	array,@object                   # @array
	.bss
	.globl	array
	.p2align	4, 0x0
array:
	.zero	80
	.size	array, 80

	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%lu\n"
	.size	.L.str, 5

	.ident	"Ubuntu clang version 17.0.6 (++20231209124227+6009708b4367-1~exp1~20231209124336.77)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
