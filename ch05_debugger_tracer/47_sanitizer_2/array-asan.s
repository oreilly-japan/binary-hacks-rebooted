	.text
	.intel_syntax noprefix
	.file	"array.c"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	push	rax
	lea	rax, [rip + array+96]
	shr	rax, 3
	cmp	byte ptr [rax + 2147450880], 0
	jne	.LBB0_2
# %bb.1:
	mov	qword ptr [rip + array+96], 42
	lea	rdi, [rip + .str]
	mov	esi, 42
	xor	eax, eax
	call	printf@PLT
	xor	eax, eax
	pop	rcx
	ret
.LBB0_2:
	lea	rdi, [rip + array+96]
	call	__asan_report_store8@PLT
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.section	.text.asan.module_ctor,"axGR",@progbits,asan.module_ctor,comdat
	.p2align	4, 0x90                         # -- Begin function asan.module_ctor
	.type	asan.module_ctor,@function
asan.module_ctor:                       # @asan.module_ctor
# %bb.0:
	push	rax
	call	__asan_init@PLT
	call	__asan_version_mismatch_check_v8@PLT
	lea	rdi, [rip + ___asan_globals_registered]
	mov	rsi, qword ptr [rip + __start_asan_globals@GOTPCREL]
	mov	rdx, qword ptr [rip + __stop_asan_globals@GOTPCREL]
	call	__asan_register_elf_globals@PLT
	pop	rax
	ret
.Lfunc_end1:
	.size	asan.module_ctor, .Lfunc_end1-asan.module_ctor
                                        # -- End function
	.section	.text.asan.module_dtor,"axGR",@progbits,asan.module_dtor,comdat
	.p2align	4, 0x90                         # -- Begin function asan.module_dtor
	.type	asan.module_dtor,@function
asan.module_dtor:                       # @asan.module_dtor
# %bb.0:
	push	rax
	lea	rdi, [rip + ___asan_globals_registered]
	mov	rsi, qword ptr [rip + __start_asan_globals@GOTPCREL]
	mov	rdx, qword ptr [rip + __stop_asan_globals@GOTPCREL]
	call	__asan_unregister_elf_globals@PLT
	pop	rax
	ret
.Lfunc_end2:
	.size	asan.module_dtor, .Lfunc_end2-asan.module_dtor
                                        # -- End function
	.type	array,@object                   # @array
	.section	.bss.array,"aGw",@nobits,array,comdat
	.globl	array
	.p2align	5, 0x0
array:
	.zero	128
	.size	array, 128

	.type	.str,@object                    # @.str
	.section	.rodata..str,"aG",@progbits,.str.8d6e8d5943003158eba64c747107e2d0,comdat
	.p2align	5, 0x0
.str:
	.asciz	"%lu\n"
	.zero	27
	.size	.str, 32

	.type	.L___asan_gen_,@object          # @___asan_gen_
	.section	.rodata,"a",@progbits
.L___asan_gen_:
	.asciz	"array.c"
	.size	.L___asan_gen_, 8

	.type	.L___asan_gen_.1,@object        # @___asan_gen_.1
	.section	.rodata.str1.1,"aMS",@progbits,1
.L___asan_gen_.1:
	.asciz	"array"
	.size	.L___asan_gen_.1, 6

	.type	__odr_asan_gen_array,@object    # @__odr_asan_gen_array
	.bss
	.globl	__odr_asan_gen_array
__odr_asan_gen_array:
	.byte	0                               # 0x0
	.size	__odr_asan_gen_array, 1

	.type	.L___asan_gen_.2,@object        # @___asan_gen_.2
	.section	.rodata.str1.1,"aMS",@progbits,1
.L___asan_gen_.2:
	.asciz	".str"
	.size	.L___asan_gen_.2, 5

	.type	.L__asan_global_array,@object   # @__asan_global_array
	.section	asan_globals,"aGwo",@progbits,array,comdat,array,unique,1
	.p2align	4, 0x90
.L__asan_global_array:
	.quad	.L__unnamed_1
	.quad	80                              # 0x50
	.quad	128                             # 0x80
	.quad	.L___asan_gen_.1
	.quad	.L___asan_gen_
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	__odr_asan_gen_array
	.size	.L__asan_global_array, 64

	.type	.L__asan_global_.str,@object    # @__asan_global_.str
	.section	asan_globals,"aGwo",@progbits,.str.8d6e8d5943003158eba64c747107e2d0,comdat,.str,unique,2
	.p2align	4, 0x90
.L__asan_global_.str:
	.quad	.L__unnamed_2
	.quad	5                               # 0x5
	.quad	32                              # 0x20
	.quad	.L___asan_gen_.2
	.quad	.L___asan_gen_
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	-1                              # 0xffffffffffffffff
	.size	.L__asan_global_.str, 64

	.hidden	___asan_globals_registered      # @___asan_globals_registered
	.type	___asan_globals_registered,@object
	.comm	___asan_globals_registered,8,8
	.hidden	__start_asan_globals
	.hidden	__stop_asan_globals
	.section	.init_array.1,"aGw",@init_array,asan.module_ctor,comdat
	.p2align	3, 0x90
	.quad	asan.module_ctor
	.section	.fini_array.1,"aGw",@fini_array,asan.module_dtor,comdat
	.p2align	3, 0x90
	.quad	asan.module_dtor
	.weak	__start_asan_globals
	.weak	__stop_asan_globals
.set .L__unnamed_1, array
.set .L__unnamed_2, .str
	.ident	"Ubuntu clang version 17.0.6 (++20231209124227+6009708b4367-1~exp1~20231209124336.77)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __asan_report_store8
	.addrsig_sym __asan_register_elf_globals
	.addrsig_sym __asan_unregister_elf_globals
	.addrsig_sym __asan_init
	.addrsig_sym asan.module_ctor
	.addrsig_sym __asan_version_mismatch_check_v8
	.addrsig_sym asan.module_dtor
	.addrsig_sym array
	.addrsig_sym .str
	.addrsig_sym .L___asan_gen_
	.addrsig_sym __odr_asan_gen_array
	.addrsig_sym .L__asan_global_array
	.addrsig_sym .L__asan_global_.str
	.addrsig_sym ___asan_globals_registered
	.addrsig_sym __start_asan_globals
	.addrsig_sym __stop_asan_globals
	.addrsig_sym .L__unnamed_1
	.addrsig_sym .L__unnamed_2
