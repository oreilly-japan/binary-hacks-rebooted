
	.intel_syntax	noprefix
	.text
	.code16

_start_text:
	mov	ax, 0xf000	# data segment が ROM(0xF0000-0xFFFFF) を指すように設定する
	mov	ds, ax
	lea	si, hello_world	# si = "Hello, World!\r\n\0"
	mov	dx, 0x3f8       # 0x3f8 = serial port

1:
	mov	al, [si]	# load from si
	test	al, al		# al == '\0'
	jz	quit		# quit if al == '\0'
	inc	si		# si++
	out	dx, al		# output char in al to 0x3f8 (serial port)
	jmp	1b		# loop


quit:
	## QEMU プロセスの終了
	##
	## QEMU では 0xcf9 に 4 を書くことでリセットできる
	## QEMUのコマンドラインに '-action reboot=shutdown' を付けて
	## reset 時にQEMUのプロセスを終了させる
	mov	dx, 0xcf9
	mov	ax, 0x4
	out	dx, ax

	hlt
	jmp	quit

hello_world:
	.ascii	"Hello, World!\r\n\0"
_end_text:


	## 0xFFF0 までを nop で埋める
	.equ	text_size, (_end_text - _start_text) # ここまで出したバイト列のサイズ
	.equ	padding_size, (0xfff0 - text_size)   # 0xfff0 から引いてパディングのサイズを決める
	.fill	padding_size, 1, 0x90		     # nop(0x90) で埋める

power_on_reset:			   # 0xFFF0 : リセット後にここに来る
	jmp	0xf000:_start_text # 0xF000:0 へジャンプ
	.ascii	"12/11/23"	   # BIOS作成日を入れるのが伝統(意味はない)
	.align	0x10000		   # 65536ちょうどにalign


