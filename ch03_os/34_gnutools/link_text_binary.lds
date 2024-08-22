OUTPUT_FORMAT("binary") /* 出力フォーマットbinaryを指示 */
SECTIONS {
         .text : { *(.text) }
         /DISCARD/ : { *(*) }
}
