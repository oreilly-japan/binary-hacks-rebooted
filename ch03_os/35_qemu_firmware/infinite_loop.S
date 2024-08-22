.code16
.text
.globl	_start
start0:
	mov	$'a', %al
	mov	$0xe9, %dx
	outb	%al, %dx
1:
	jmp	1b

_start:
	ljmp	$0xf000, $start0
	.string	"11/12/23"

.align	16
end:

