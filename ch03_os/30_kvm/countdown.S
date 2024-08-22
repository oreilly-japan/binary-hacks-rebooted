.intel_syntax noprefix
.code16

.text
.globl _start
_start:
    mov dx, 0x3F8   # dx = COM1 port
    in al, dx       # Get a user input

loop:               # do {
    out dx, al      #   print(al)
    dec al          #   al -= 1;
    cmp al, 0       # } while(al != 0);
    jnz loop
    hlt
