CFLAG = -Wall -Wextra -std=c99 -g
EXEC = main
CC = gcc

all: main

build: main

main: main.o tree.o listfile.o
	${CC} ${CFLAG} -o ${EXEC} $?

main.o: main.c
	${CC} ${CFLAG} -c -o $@ $?

tree.o: tree.c
	${CC} ${CFLAG} -c -o $@ $?

listfile.o: listfile.c
	${CC} ${CFLAG} -c -o $@ $?

clean:
	rm -f *.o ${EXEC}