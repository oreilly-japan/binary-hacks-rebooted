all:
	clang use-after-free.c -o use-after-free
	clang use-after-free.c -fsanitize=address -o use-after-free-asan
	clang integer-overflow.c -fsanitize=undefined -o integer-overflow-ubsan.bin
clean:
	rm -f use-after-free use-after-free-asan integer-overflow-ubsan.bin
