all:
	gcc -fPIC libhello.c -o libhello.so -shared
	gcc dlopen-sample.c -o dlopen-sample.out
	gcc malloc-hook.c -shared -fPIC -o malloc-hook.so
	gcc say_hello.c -o say_hello.out
	gcc main-hook.c -shared -fPIC -o main-hook.so
clean:
	rm -f *.so *.out
