OUTPUT_FORMAT("binary")

/* -m16でコンパイルした場合、32bit ELFになり、リンカに文句を言われるので付ける
 * バイナリを出力するので効果はない */
OUTPUT_ARCH(i386)

SECTIONS {
         . = 0x100;             /* DOSでは0x100に配置される */
         .text : { *(.text) } .rodata : { *(.rodata) } /DISCARD/ : { *(*) }
}
