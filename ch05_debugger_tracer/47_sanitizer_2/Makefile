FLAGS = -S \
		-fno-asynchronous-unwind-tables \
		-fno-dwarf2-cfi-asm \
		-fno-crash-diagnostics \
		-O1 -masm=intel
all:
	clang-17 -o array.s $(FLAGS) array.c
	clang-17 -o array-asan.s  $(FLAGS) -fsanitize=address \
		array.c
	clang -fsanitize=address -o array-false-negative array-false-negative.c
clean:
	rm -f array-false-negative
