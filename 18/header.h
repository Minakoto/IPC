#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>

#define TIMEOUT 60
#define MAXCLIENTS 100

#define CHECK(item, msg, text, wrong) ({ if( item wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })



int sum(int, int);
int sub(int, int);
int mult(int, int);
int divi(int, int);
void recvpars(int socket, int* a, int* b, char* buffer);
void printusers(int);