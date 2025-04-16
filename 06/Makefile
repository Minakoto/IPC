CC = gcc

all: build

build: first second

first: first.o
	touch chat
	$(CC) first.o -o first
	rm first.o

second: second.o
	$(CC) second.o -o second
	rm second.o

first.o: first.c head.h
	$(CC) -c first.c

second.o: second.c head.h
	$(CC) -c second.c

clean:
	rm first second chat