OUTPUT_FORMAT("binary")
SECTIONS {
         .text : { *(.text) }
         .rodata : { *(.rodata) }
         .data : { *(.data) }

         __bss_start = .;       /* シンボル__bss_startを今のアドレス(.bssの直前)として定義 */
         .bss : { *(.bss) }
         _end = .;              /* シンボル_endを今のアドレス(.bssの直後)として定義 */

         /DISCARD/ : { *(*) }
}
