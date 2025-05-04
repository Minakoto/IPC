all: sniffer
	
sniffer: sniffer.o
	gcc sniffer.o -o sniffer
	rm *.o

sniffer.o: sniffer.c
	gcc -c sniffer.c

clean:
	rm sniffer
	rm -f packets.txt
su:
	sudo ./sniffer 127.0.0.1 9001