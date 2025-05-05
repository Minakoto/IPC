#include "header.h"


void printusers(int nclients) { 
	if(nclients) printf("%d user on-line\n", nclients);
	else printf("No User on line\n");
}
int sum(int a, int b) {
    return a+b;
}
int sub(int a, int b) {
    return a - b;
}
int mult(int a, int b) {
    return a*b;
}
int divi(int a, int b) {
    if(a != 0) return a/b;
    return 0;
}

void recvpars(int socket, int* a, int* b, char* buffer) {
    int recieved = recv(socket, &buffer[0], sizeof(buffer), 0);
    CHECK(recieved, perror, "recv cmd", < 0);
    *a = atoi(buffer);
    printf("First: %d\n", *a);
    recieved = recv(socket, &buffer[0], sizeof(buffer), 0);
    CHECK(recieved, perror, "recv cmd", < 0);
    *b = atoi(buffer);
    printf("Second: %d\n", *b);
}
