all:
	gcc -shared -fPIC mylib.c -o mylib.so
	gcc dlopen.c -o dlopen
	gcc ./ffi-asm-thunk.c ./thunk.S -o ffi-asm-thunk
	gcc ./ffi-asm-thunk-many-args.c ./thunk.S -o many-args
	gcc ffi-libc.c -o ffi-libc -lffi

# rustc mylib.rs --crate-type=cdylib -o mylib.so

clean:
	rm -f mylib.so dlopen ffi-asm-thunk many-args ffi-libc
