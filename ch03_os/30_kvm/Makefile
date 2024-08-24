all: guest vmm

guest: countdown.S
	as -32 -o countdown.o countdown.S
	ld -m elf_i386 -N -e _start -o countdown.elf countdown.o
	strip -R .note.gnu.property countdown.elf
	objcopy -O binary countdown.elf countdown.bin

vmm: kvm_run.c
	gcc -o kvm_run kvm_run.c -Wall -Wextra -Werror

clean:
	rm -f *.o *.bin *.elf kvm_run
