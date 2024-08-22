	.file	"throw_exception.cpp"
# GNU C++17 (Debian 13.2.0-4) version 13.2.0 (x86_64-linux-gnu)
#	compiled by GNU C version 13.2.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

# warning: MPFR header version 4.2.1 differs from library version 4.2.0.
# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -std=gnu++17 -fasynchronous-unwind-tables
	.text
	.globl	buf
	.bss
	.align 32
	.type	buf, @object
	.size	buf, 100
buf:
	.zero	100
	.section	.rodata
	.align 8
.LC0:
	.string	"Enter expression to calculate: "
	.text
	.globl	_Z12ReadAndThrowv
	.type	_Z12ReadAndThrowv, @function
_Z12ReadAndThrowv:
.LFB2:
	.cfi_startproc
  .cfi_escape 22, 7, 5, 87, 10, 112, 8, 28
.cfi_escape 22, 15, 197, 5, 19, 96, 10, 252, 45, 34, 18, 148, 1, 18, 8, 0, 41, 40, 38, 0, 18, 8, 43, 46, 40, 7, 0, 19, 23, 34, 22, 47, 18, 0, 18, 8, 42, 46, 40, 7, 0, 19, 23, 30, 22, 47, 4, 0, 8, 48, 28, 22, 8, 1, 34, 47, 208, 255, 19, 19, 8, 0, 22, 8, 1, 22, 8, 2, 22, 8, 3, 22, 8, 4, 22, 8, 5, 22, 8, 6, 22, 8, 7, 22, 8, 8, 22, 8, 9, 22, 8, 10, 22, 8, 11, 22, 8, 12, 22, 8, 13, 22, 8, 14, 22, 8, 15, 22, 8, 16, 22, 8, 17, 22, 8, 18, 22, 8, 19, 22, 8, 20, 22, 8, 21, 22, 8, 22, 22, 8, 23, 22, 8, 24, 22, 8, 25, 22, 8, 26, 22, 8, 27, 22, 96, 8, 161, 28, 22, 87, 10, 64, 8, 28, 22, 96, 8, 67, 34, 22, 87, 10, 48, 8, 28, 22, 8, 0, 22, 8, 1, 22, 8, 2, 22, 96, 10, 116, 1, 28, 22, 8, 0, 20, 14, 0, 0, 100, 167, 179, 182, 224, 13, 27, 8, 48, 34, 8, 0, 36, 34, 20, 14, 0, 0, 100, 167, 179, 182, 224, 13, 29, 14, 0, 0, 138, 93, 120, 69, 99, 1, 27, 8, 48, 34, 8, 8, 36, 34, 20, 14, 0, 0, 138, 93, 120, 69, 99, 1, 29, 14, 0, 0, 193, 111, 242, 134, 35, 0, 27, 8, 48, 34, 8, 16, 36, 34, 20, 14, 0, 0, 193, 111, 242, 134, 35, 0, 29, 14, 0, 128, 198, 164, 126, 141, 3, 0, 27, 8, 48, 34, 8, 24, 36, 34, 20, 14, 0, 128, 198, 164, 126, 141, 3, 0, 29, 14, 0, 64, 122, 16, 243, 90, 0, 0, 27, 8, 48, 34, 8, 32, 36, 34, 20, 14, 0, 64, 122, 16, 243, 90, 0, 0, 29, 14, 0, 160, 114, 78, 24, 9, 0, 0, 27, 8, 48, 34, 8, 40, 36, 34, 20, 14, 0, 160, 114, 78, 24, 9, 0, 0, 29, 14, 0, 16, 165, 212, 232, 0, 0, 0, 27, 8, 48, 34, 8, 48, 36, 34, 20, 14, 0, 16, 165, 212, 232, 0, 0, 0, 29, 14, 0, 232, 118, 72, 23, 0, 0, 0, 27, 8, 48, 34, 8, 56, 36, 34, 22, 8, 0, 20, 14, 0, 232, 118, 72, 23, 0, 0, 0, 29, 14, 0, 228, 11, 84, 2, 0, 0, 0, 27, 8, 48, 34, 8, 0, 36, 34, 20, 14, 0, 228, 11, 84, 2, 0, 0, 0, 29, 14, 0, 202, 154, 59, 0, 0, 0, 0, 27, 8, 48, 34, 8, 8, 36, 34, 20, 14, 0, 202, 154, 59, 0, 0, 0, 0, 29, 14, 0, 225, 245, 5, 0, 0, 0, 0, 27, 8, 48, 34, 8, 16, 36, 34, 20, 14, 0, 225, 245, 5, 0, 0, 0, 0, 29, 14, 128, 150, 152, 0, 0, 0, 0, 0, 27, 8, 48, 34, 8, 24, 36, 34, 20, 14, 128, 150, 152, 0, 0, 0, 0, 0, 29, 14, 64, 66, 15, 0, 0, 0, 0, 0, 27, 8, 48, 34, 8, 32, 36, 34, 20, 14, 64, 66, 15, 0, 0, 0, 0, 0, 29, 14, 160, 134, 1, 0, 0, 0, 0, 0, 27, 8, 48, 34, 8, 40, 36, 34, 20, 14, 160, 134, 1, 0, 0, 0, 0, 0, 29, 14, 16, 39, 0, 0, 0, 0, 0, 0, 27, 8, 48, 34, 8, 48, 36, 34, 20, 14, 16, 39, 0, 0, 0, 0, 0, 0, 29, 14, 232, 3, 0, 0, 0, 0, 0, 0, 27, 8, 48, 34, 8, 56, 36, 34, 22, 8, 0, 20, 14, 232, 3, 0, 0, 0, 0, 0, 0, 29, 14, 100, 0, 0, 0, 0, 0, 0, 0, 27, 8, 48, 34, 8, 0, 36, 34, 20, 14, 100, 0, 0, 0, 0, 0, 0, 0, 29, 14, 10, 0, 0, 0, 0, 0, 0, 0, 27, 8, 48, 34, 8, 8, 36, 34, 20, 14, 10, 0, 0, 0, 0, 0, 0, 0, 29, 14, 1, 0, 0, 0, 0, 0, 0, 0, 27, 8, 48, 34, 8, 16, 36, 34, 22, 19
# BLOCK 2 seq:0
# PRED: ENTRY (FALLTHRU)
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$16, %rsp	#,
# throw_exception.cpp:7:   printf("Enter expression to calculate: ");
	leaq	.LC0(%rip), %rax	#, tmp84
	movq	%rax, %rdi	# tmp84,
	movl	$0, %eax	#,
	call	printf@PLT	#
# throw_exception.cpp:8:   fgets(buf, sizeof(buf), stdin);
	movq	stdin(%rip), %rax	# stdin, stdin.0_1
	movq	%rax, %rdx	# stdin.0_1,
	movl	$100, %esi	#,
	leaq	buf(%rip), %rax	#, tmp85
	movq	%rax, %rdi	# tmp85,
	call	fgets@PLT	#
# throw_exception.cpp:10:   char *nlp = strchr(buf, '\n');
	movl	$10, %esi	#,
	leaq	buf(%rip), %rax	#, tmp86
	movq	%rax, %rdi	# tmp86,
	call	strchr@PLT	#
	movq	%rax, -8(%rbp)	# tmp87, nlp
# throw_exception.cpp:11:   if (nlp) *nlp = '\0';
	cmpq	$0, -8(%rbp)	#, nlp
# SUCC: 3 (FALLTHRU) 4
	je	.L2	#,
# BLOCK 3 seq:1
# PRED: 2 (FALLTHRU)
# throw_exception.cpp:11:   if (nlp) *nlp = '\0';
	movq	-8(%rbp), %rax	# nlp, tmp88
# SUCC: 4 (FALLTHRU)
	movb	$0, (%rax)	#, *nlp_6
# BLOCK 4 seq:2
# PRED: 2 3 (FALLTHRU)
.L2:
# throw_exception.cpp:13:   throw (const char*)buf;
	movl	$8, %edi	#,
	call	__cxa_allocate_exception@PLT	#
# throw_exception.cpp:13:   throw (const char*)buf;
	leaq	buf(%rip), %rdx	#, tmp90
	movq	%rdx, (%rax)	# tmp90, MEM[(const char * *)_9]
	movl	$0, %edx	#,
	leaq	_ZTIPKc(%rip), %rcx	#, tmp91
	movq	%rcx, %rsi	# tmp91,
	movq	%rax, %rdi	# _9,
# SUCC:
	call	__cxa_throw@PLT	#
	.cfi_endproc
.LFE2:
	.size	_Z12ReadAndThrowv, .-_Z12ReadAndThrowv
	.section	.rodata
.LC1:
	.string	"Exception occurred!: "
	.text
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA3
# BLOCK 2 seq:0
# PRED: ENTRY (FALLTHRU)
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%rbx	#
	subq	$24, %rsp	#,
	.cfi_offset 3, -24
.LEHB0:
# SUCC: 4 (ABNORMAL,ABNORMAL_CALL,EH) 3 (FALLTHRU)
# throw_exception.cpp:18:     ReadAndThrow();
	call	_Z12ReadAndThrowv	#
.LEHE0:
# BLOCK 3 seq:1
# PRED: 2 (FALLTHRU) 8 [always]
.L7:
# throw_exception.cpp:23: }
	movl	$0, %eax	#, _13
# SUCC: 10 [always]  throw_exception.cpp:23:1
	jmp	.L11	#
# BLOCK 4 seq:2
# PRED: 2 (ABNORMAL,ABNORMAL_CALL,EH)
.L9:
# throw_exception.cpp:19:   } catch (const char* msg) {
	cmpq	$1, %rdx	#, D.3811
# SUCC: 5 (FALLTHRU) 6
	je	.L6	#,
# BLOCK 5 seq:3
# PRED: 4 (FALLTHRU)
	movq	%rax, %rdi	# D.3812,
.LEHB1:
# SUCC:
	call	_Unwind_Resume@PLT	#
.LEHE1:
# BLOCK 6 seq:4
# PRED: 4
.L6:
# throw_exception.cpp:19:   } catch (const char* msg) {
	movq	%rax, %rdi	# _1,
	call	__cxa_begin_catch@PLT	#
# throw_exception.cpp:19:   } catch (const char* msg) {
	movq	%rax, -24(%rbp)	# _7, msg
# throw_exception.cpp:20:     printf("Exception occurred!: ");
	leaq	.LC1(%rip), %rax	#, tmp92
	movq	%rax, %rdi	# tmp92,
	movl	$0, %eax	#,
.LEHB2:
# SUCC: 9 (ABNORMAL,ABNORMAL_CALL,EH) 7 (FALLTHRU)
	call	printf@PLT	#
# BLOCK 7 seq:5
# PRED: 6 (FALLTHRU)
# throw_exception.cpp:21:     puts(msg);
	movq	-24(%rbp), %rax	# msg, tmp93
	movq	%rax, %rdi	# tmp93,
# SUCC: 9 (ABNORMAL,ABNORMAL_CALL,EH) 8 (FALLTHRU)
	call	puts@PLT	#
.LEHE2:
# BLOCK 8 seq:6
# PRED: 7 (FALLTHRU)
# throw_exception.cpp:22:   }
	call	__cxa_end_catch@PLT	#
# SUCC: 3 [always]
	jmp	.L7	#
# BLOCK 9 seq:7
# PRED: 6 (ABNORMAL,ABNORMAL_CALL,EH) 7 (ABNORMAL,ABNORMAL_CALL,EH)
.L10:
	movq	%rax, %rbx	#, tmp94
	call	__cxa_end_catch@PLT	#
	movq	%rbx, %rax	# tmp94, D.3813
	movq	%rax, %rdi	# D.3813,
.LEHB3:
# SUCC:
	call	_Unwind_Resume@PLT	#
.LEHE3:
# BLOCK 10 seq:8
# PRED: 3 [always]  throw_exception.cpp:23:1
.L11:
# throw_exception.cpp:23: }
	movq	-8(%rbp), %rbx	#,
	leave
	.cfi_def_cfa 7, 8
# SUCC: EXIT [always]
.dummy_landpad:
	ret
	.cfi_endproc
.LFE3:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
	.align 4
.LLSDA3:
	.byte	0xff	# @LPStart format (omit)
	.byte	0x9b	# @TType format (indirect pcrel sdata4)
	.uleb128 .LLSDATT3-.LLSDATTD3	# @TType base offset
.LLSDATTD3:
	.byte	0x1	# call-site format (uleb128)
	.uleb128 .LLSDACSE3-.LLSDACSB3	# Call-site table length
.LLSDACSB3:
	.uleb128 .LEHB0-.LFB3	# region 0 start
	.uleb128 .LEHE0-.LEHB0	# length
	.uleb128 .dummy_landpad-.LFB3	# landing pad
	.uleb128 0x1	# action
	.uleb128 .LEHB1-.LFB3	# region 1 start
	.uleb128 .LEHE1-.LEHB1	# length
	.uleb128 0	# landing pad
	.uleb128 0	# action
	.uleb128 .LEHB2-.LFB3	# region 2 start
	.uleb128 .LEHE2-.LEHB2	# length
	.uleb128 .L10-.LFB3	# landing pad
	.uleb128 0	# action
	.uleb128 .LEHB3-.LFB3	# region 3 start
	.uleb128 .LEHE3-.LEHB3	# length
	.uleb128 0	# landing pad
	.uleb128 0	# action
.LLSDACSE3:
	.byte	0x1	# Action record table
	.byte	0
	.align 4
	.long	DW.ref._ZTIPKc-.
.LLSDATT3:
	.text
	.size	main, .-main
	.hidden	DW.ref._ZTIPKc
	.weak	DW.ref._ZTIPKc
	.section	.data.rel.local.DW.ref._ZTIPKc,"awG",@progbits,DW.ref._ZTIPKc,comdat
	.align 8
	.type	DW.ref._ZTIPKc, @object
	.size	DW.ref._ZTIPKc, 8
DW.ref._ZTIPKc:
	.quad	_ZTIPKc
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (Debian 13.2.0-4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
