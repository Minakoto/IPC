CC = gcc

all: build

build: client server

client: client.o
	touch chat
	$(CC) client.o -o client
	rm client.o

server: server.o
	$(CC) server.o -o server
	rm server.o

client.o: client.c
	$(CC) -c client.c

server.o: server.c
	$(CC) -c server.c

clean:
	rm client server chat