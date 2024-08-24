OUTPUT_FORMAT("binary")
SECTIONS {
         . = 0x1000;            /* 配置するアドレスを0x1000に設定 */
         .text : { *(.text) }
         /DISCARD/ : { *(*) }
}
