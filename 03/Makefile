CC = gcc

all: build

build: menu

menu: menu.o phonebook.o list
	$(CC) menu.o phonebook.o -o menu -L. list.a
	rm *.o

menu.o: menu.c phonebook.h
	$(CC) -c menu.c

phonebook.o: phonebook.c phonebook.h
	$(CC) -c phonebook.c

list: list.o
	ar q list.a list.o


list.o: list.c list.h
	$(CC) -c list.c

clean:
	rm menu
	rm list.a