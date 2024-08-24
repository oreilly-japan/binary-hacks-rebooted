	.intel_syntax	noprefix
	.text
	.code16

_start_text:
	jmp	_start_text
_end_text:

	## 0x7FF0 までを nop で埋める
	.equ	text_size, (_end_text - _start_text) # ここまで出したバイト列のサイズ
	.equ	padding_size, (0x7ff0 - text_size)   # 0x7ff0 から引いてパディングのサイズを決める
	.fill	padding_size, 1, 0x90		     # nop(0x90) で埋める

power_on_reset:			# リセット後にここに来る
	# Apollo Lakeでは、0xFFFF0000 と 0xF0000 の間のエイリアスは存在しないので CS を変更しない
	jmp	_start_text
	.align	0x8000		# 32768ょうどにalign
