OUTPUT_FORMAT("binary")
SECTIONS {
         /* IBBLの長さが0x8000なので、0x1_0000 から 0x0_8000 を引いて先頭アドレスは 0x8000 */
         . = 0x8000;
         .text : { *(.text) }
         /* lgdt命令が使うポインタは物理アドレスを使う
          * 物理アドレスはシンボルの値 + 0xFFFF0000になる */
         gdt_ptr32 = gdt_ptr + 0xffff0000;
         /DISCARD/ : { *(*) }
}
