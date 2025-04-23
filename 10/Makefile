all: main
	
main: main.o
	gcc main.o -o main
	rm *.o

main.o: main.c
	gcc -c main.c -lrt

clean:
	rm main
	rm log*
	rm ttt.txt