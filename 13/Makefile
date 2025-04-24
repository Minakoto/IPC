all: main
	
main: main.o
	touch shmfile
	gcc main.o -o main
	rm *.o

main.o: main.c
	gcc -c main.c

clean:
	rm main
	rm shmfile