#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <fcntl.h>




#define CHECK(item, msg, text, wrong) ({ if( item wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

int* nclients;

void printusers();
void handle(int);
int sum(int, int);
int sub(int, int);
int mult(int, int);
int divi(int, int);
void readpars(int socket, int* a, int* b, char* buffer);



int main(int argc, char *argv[])
{
    char buff[1024];    // Буфер для различных нужд
	int status;
    printf("TCP SERVER DEMO\n");
	
    int shm_id = shm_open("/shmget", O_CREAT | O_RDWR, 0666);
    CHECK(shm_id, perror, "get", == -1);
    ftruncate(shm_id, sizeof(int));
    nclients = (int*) mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_id, 0);
    CHECK(nclients, perror, "Map", == (int*)-1);

	int sockfd, newsockfd;
	int portno;
	int pid;
    socklen_t c_len;
    struct sockaddr_in serv_addr, cli_addr;
	
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    CHECK(sockfd, perror, "socket", < 0);
	
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

	status = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    CHECK(status, perror, "bind", < 0);

	listen(sockfd,5);
    c_len = sizeof(cli_addr);
	
	while (1)  {
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &c_len);
        CHECK(newsockfd, perror, "accept", < 0);
	    *nclients+=1;
        printusers();

        switch(pid = fork()) {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
            close(sockfd);
            handle(newsockfd);
            close(newsockfd);
            *nclients =- 1;
            exit(EXIT_SUCCESS);
            default: 
            close(newsockfd);
        }
     }
     close(sockfd);
     shutdown(sockfd, SHUT_RDWR);
     close(shm_id);
     shm_unlink("/shmget");
     return 0;
}

void printusers() { 
	if(*nclients)
	{printf("%d user on-line\n",*nclients);}
	else {printf("No User on line\n");}
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

void handle(int socket) {
    char buffer[1024];
    int recieved, a, b, cmd, result;
    #define MSG_MSG "Command(1 - +, 2 - -, 3 - *, 4 - /,)\n"
    while(1) {
        a = b = cmd = result = 0;
        CHECK(write(socket, MSG_MSG, sizeof(MSG_MSG)), perror, "write msg", == -1);
        printf("Open for input\n");

        recieved = read(socket, &buffer[0], sizeof(buffer));
        CHECK(recieved, perror, "read cmd", < 0);
        cmd = atoi(buffer);
        printf("Operation %d\n", cmd);
        if(cmd < 5) {
            readpars(socket, &a, &b, buffer);
            switch(cmd) {
                case 1:
                    result = sum(a, b);
                    break;
                case 2:
                    result = sub(a, b);
                    break;
                case 3:
                    result = mult(a, b);
                    break;
                case 4:
                    result = divi(a, b);
                    break;
            }
            snprintf(buffer, strlen(buffer), "%d", result);
            buffer[strlen(buffer)] = '\n';
        } else if(cmd == 5) {
            return;
        }
        else strcpy(buffer, "Error\n");
        write(socket, &buffer[0], sizeof(buffer));
    }
}

void readpars(int socket, int* a, int* b, char* buffer) {
    int recieved = read(socket, &buffer[0], sizeof(buffer));
    CHECK(recieved, perror, "read cmd", < 0);
    *a = atoi(buffer);
    printf("First: %d\n", *a);
    recieved = read(socket, &buffer[0], sizeof(buffer));
    CHECK(recieved, perror, "read cmd", < 0);
    *b = atoi(buffer);
    printf("Second: %d\n", *b);
}

