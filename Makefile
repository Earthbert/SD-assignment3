all: build

build: main.c tree.c listfile.c
	gcc -g -Wall -Wextra main.c tree.c listfile.c -o sd_fs

clean:
	rm *.o sd_fs

run:
	./sd_fs
