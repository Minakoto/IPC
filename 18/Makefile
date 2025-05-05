CC = gcc

all: build

build: client server

client: client.o
	$(CC) client.o -o client
	rm client.o

server: server.o aux.o
	$(CC) server.o aux.o -o server
	rm server.o aux.o

client.o: client.c
	$(CC) -c client.c

server.o: server.c header.h
	$(CC) -c server.c

aux.o: aux.c header.h
	$(CC) -c aux.c

clean:
	rm client server