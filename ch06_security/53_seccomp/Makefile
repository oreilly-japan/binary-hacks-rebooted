all:
	gcc -c sandbox.c -lseccomp
	gcc -c good_write.c
	gcc -c bad_open.c
	gcc good_write.o sandbox.o -lseccomp -o good_write
	gcc bad_open.o sandbox.o -lseccomp -o bad_open
clean:
	rm -f *.o good_write bad_write
