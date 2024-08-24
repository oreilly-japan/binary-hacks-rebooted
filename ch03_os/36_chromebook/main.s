	.code16
	.intel_syntax noprefix

	## オリジナルのROMに入っているIBBLとサイズをあわせる (筆者の環境では32KiB)
	.equ	IBBL_LENGTH, 0x8000

	## PCIコンフィグ空間への書き込み
.macro	pci_write_config bus, dev, fn, offset, AX
	mov	dx, 0xcf8 	# 0xcf8 = コンフィグ空間のアドレス
	mov	eax, (0x80000000 + (\dev<<11) + (\fn<<8) + \offset)
	out	dx, eax
	mov	dx, 0xcfc	# 0xcfc = コンフィグ空間へ書き込むデータ
	mov	eax, ecx
	out	dx, \AX
.endm

	## PCIコンフィグ空間への32bit書き込み
.macro	pci_write_config32 bus, dev, fn, offset
	pci_write_config \bus, \dev, \fn, \offset, eax
.endm
	## PCIコンフィグ空間への16bit書き込み
.macro	pci_write_config16 bus, dev, fn, offset
	pci_write_config \bus, \dev, \fn, \offset, ax
.endm

	## P2SB下のGPIOからピンの設定をする
	## 設定値は2個の32bit値からなる(val0, val1)
.macro	config_pin	port, pin, val0, val1
	## 下位32bit書きこみ (Pad COnfiguration DW0)
	mov	ecx, \val0
	## 0xd0000000 = P2SB の BARで設定したアドレス
	## P2SB 下のデバイスは、port で指定する
	## P2SB 下のデバイスにあるレジスタはそこからのオフセットで指定する
	mov	eax, (0xd0000000 | (\port<<16) | (0x500 + \pin*8))
	mov	[eax], ecx
	## 書き込み保証のためにread backが必要(corebootのソースより)
	mov	ecx, [eax]

	## 上位32bit書きこみ (Pad COnfiguration DW1)
	mov	ecx, \val1
	add	eax, 4
	mov	[eax], ecx
	## 書き込み保証のためにread backが必要(corebootのソースより)
	mov	ecx, [eax]
.endm

start:
	cli

 	## GDTのアドレスは、24bitに入っていないので
	## lgdtではなく、lgdtdを使う。
	lgdtd	[cs:gdt]

        ## プロテクトモードへ移行
	mov	ecx, cr0
	or	ecx, 1
	mov	cr0, ecx

        ## DS = 8 (data, 32bit full access) にする
	mov	ax, 8
	mov	ds, ax


        ## P2SBを設定する。ここでは 0xd000_0000 以下にレジスタが見えるようにする
	## pci_writ_config(16|32) は、I/Oポート 0xcf8,0xcfc 経由で、
	## PCIコンフィグ空間へ16bit,32bit値を書き込むマクロ

	## P2SB(00:0d.0) の PCI_COMMAND (0x04) へ 0x2 | 0x4 を書いて
	## MEM_EN(0x2) と BUS_MASTER_EN(0x4) を有効にする
	mov	cx, 0x02 | 0x04
	pci_write_config16 0, 0x0d, 0x0, 0x4

	## P2SB(00:0d.0) のBAR (0x10,0x14) を0x00000000_D0000000 に設定
	mov	ecx, 0xd0000000
	pci_write_config32 0, 0x0d, 0x0, 0x10 # PCI_BASE_ADDRESS の下位32bit
	mov	ecx, 0x00000000
	pci_write_config32 0, 0x0d, 0x0, 0x14 # PCI_BASE_ADDRESS の上位32bit

	## config_pin は P2SB 経由で GPIO を設定するマクロ
	## P2SBは、ポートとオフセットでデバイスを識別する
        ## LPCのピン設定をするGPIOデバイスは 0xc0 ポートの、オフセット 34-42 にある。
        ## 設定値は、corebootの実行ログから取得
	config_pin	0xc0, 34, 0x40000402, 0x00003000
	config_pin	0xc0, 35, 0x40000400, 0x00000000
	config_pin	0xc0, 36, 0x40000102, 0x00027000
	config_pin	0xc0, 37, 0x40000402, 0x00003000
	config_pin	0xc0, 38, 0x40000402, 0x00003000
	config_pin	0xc0, 39, 0x40000402, 0x00003000
	config_pin	0xc0, 40, 0x40000402, 0x00003000
	config_pin	0xc0, 41, 0x40000400, 0x00003000
	config_pin	0xc0, 42, 0x40000400, 0x00003c00


        ## シンボルstrにある文字列 "Hello, World!\r\n" をport 0x80に順番に出力する

	lea	esi, [str]
	## データは32bit物理アドレスでアクセスするので、実際にROMがロードされている
	## 0xFFFF_0000 のオフセットを付ける
	add	esi, 0xffff0000

	## 伝統的なPCでは、ポート0x80 へout命令を使ってデータを出力することでPOST CODEが表示できる
	## C223NAを含むChromebookの一部は、ポート0x80がLPC経由でECに接続されていて
	## ECへPOST CODEを送信できる。
	## 送信したPOST CODEの履歴はECのコンソール(ttyUSB2)からport80コマンドで取得できる
	mov	dx, 0x80

	## 文字列をport80へ出力するループ
loop:
	mov	al, [esi]
	cmp	al, 0
	je	1f
	out	dx, al
	inc	esi
	jmp	loop

1:
        # プログラム終了
	hlt
	jmp	1b


str:
	.string	"Hello, World!\r\n\0"

        # 32bitアクセス用GDT
	.align	16
gdt:
	.word	(8*2)-1
	.long	gdt_ptr32 	# main.lds参照。リンカスクリプト内で0xffff0000オフセットを付ける

	.align	16
	.globl	gdt_ptr
gdt_ptr:			# GDT
	.quad	0						   # selector[0]
	.quad	((0xc)<<52) | (0xf<<48) | (0x93<<40) | (0xffff<<0) # selector[8] : full data access

end:

	.fill	(IBBL_LENGTH - (end-start))-16, 1, 0x90

	.code16
reset:
	jmp	start

	.align	16
