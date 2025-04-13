CC = gcc
cmd = cmd/*.c
obj = *.o

all: build

build: exek main

main: main.o
	gcc main.o -o main
	rm *.o

main.o: main.c
	gcc -c main.c

cmds: $(cmd) cmd/head.h
	$(CC) -c $(cmd)
	
exek: cmds
	$(CC) max.o -o exec/max
	$(CC) rnd.o -o exec/rnd
	$(CC) square.o -o exec/square
	$(CC) sum.o -o exec/sum
	rm *.o

clean:
	rm exec/*
	rm main
	@rm *.o