#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

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
void recv_file(int socket);
void send_file(int socket);
void endit();

int shm_id, sockfd;



int main(int argc, char *argv[])
{
    signal(SIGINT, endit);


    char buff[1024];    // Буфер для различных нужд
	int status;
    printf("TCP SERVER DEMO\n");
	
    shm_id = shm_open("/shmget", O_CREAT | O_RDWR, 0666);
    CHECK(shm_id, perror, "get", == -1);
    ftruncate(shm_id, sizeof(int));
    nclients = (int*) mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_id, 0);
    CHECK(nclients, perror, "Map", == (int*)-1);

	int newsockfd;
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

     return 0;
}

void endit() {
    close(sockfd);
    shutdown(sockfd, SHUT_RDWR);
    close(shm_id);
    shm_unlink("/shmget");
}
void printusers() { 
	if(*nclients)
	{printf("%d user on-line\n",*nclients);}
	else {printf("No User on line\n");}
}
int sum(int a, int b) {
    return a + b;
}
int sub(int a, int b) {
    return a - b;
}
int mult(int a, int b) {
    return a * b;
}
int divi(int a, int b) {
    if(a != 0) return a / b;
    return 0;
}

void handle(int socket) {
    char buffer[1024];
    int recieved, a, b, cmd, result;
    #define MSG_MSG "Command\n1 - +\n2 - -\n3 - *\n4 - /\n5 - send\n6 - receive\n7 - exit\n"
    while(1) {
        a = b = cmd = result = 0;
        printf("Open for input\n");
        CHECK(send(socket, MSG_MSG, sizeof(MSG_MSG), 0), perror, "write msg", == -1);

        recieved = read(socket, &buffer[0], sizeof(buffer));
        CHECK(recieved, perror, "read cmd", < 0);
        cmd = atoi(buffer);
        printf("Operation %d\n", cmd);
        if(cmd > 0 && cmd < 7) {
            switch(cmd) {
                case 1:
                    readpars(socket, &a, &b, buffer);
                    result = sum(a, b);
                    break;
                case 2:
                    readpars(socket, &a, &b, buffer);
                    result = sub(a, b);
                    break;
                case 3:
                    readpars(socket, &a, &b, buffer);
                    result = mult(a, b);
                    break;
                case 4:
                    readpars(socket, &a, &b, buffer);
                    result = divi(a, b);
                    break;
                case 5:
                    recv_file(socket);
                    break;
                case 6:
                    send_file(socket);
                    break;
            }
            if(cmd < 5) {
                snprintf(buffer, strlen(buffer), "%d", result);
                buffer[strlen(buffer)] = '\n';
                write(socket, &buffer[0], sizeof(buffer));
            }

        } else if(cmd == 7 && cmd == 0) {
            return;
        }
        else strcpy(buffer, "Error\n");
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


void send_file(int socket) {
    char buffer[1024];
    char filename[1024] = "serv/";
    int tmp = 0;
    int bytes_read = recv(socket, buffer, sizeof(buffer), 0);
    CHECK(bytes_read, perror, "read fname", < 0);
    buffer[strcspn(buffer, "\n")] = 0;
    strcat(filename, buffer);
    int filefd = open(filename, O_RDONLY);
    CHECK(filefd, perror, "file open", == -1);
    int filesize = 0;
    filesize = lseek(filefd, 0L, SEEK_END);
    lseek(filefd, 0L, SEEK_SET);
    int bytes_sent = send(socket, &filesize, sizeof(int), 0);
    CHECK(bytes_sent, perror, "write flen", < 0);
    bytes_sent = 0;
    while(bytes_sent < filesize) {
        bytes_read = read(filefd, &buffer[0], sizeof(buffer));
        CHECK(bytes_read, perror, "read file", < 0);
        tmp = send(socket, &buffer[0], bytes_read, 0);
        CHECK(bytes_sent, perror, "write file", < 0);
        bytes_sent += tmp;
    }
    close(filefd);
    int confirm;
    recv(socket, &confirm, sizeof(confirm), 0);
    if(confirm) return;
}

void recv_file(int socket) {
    char buffer[1024];
    char filename[1024] = "serv/";
    int bytes_read = 0, filesize = 0, tmp = 0;
    int filefd;
    bytes_read = recv(socket, &buffer[0], sizeof(buffer), 0);
    CHECK(bytes_read, perror, "read fname", < 0);
    buffer[strcspn(buffer, "\n")] = 0;
    strcat(filename, buffer);
    filefd = open(filename, O_CREAT|O_WRONLY, 0600);
    CHECK(filefd, perror, "creat file", == -1);
    bytes_read = recv(socket, &filesize, sizeof(filesize), 0);
    CHECK(bytes_read, perror, "read flen", < 0);

    while(bytes_read < filesize) {
        tmp = recv(socket, &buffer[0], sizeof(buffer), 0);
        printf("%d", tmp);
        CHECK(tmp, perror, "read file", < 0);
        tmp = write(filefd, &buffer[0], strlen(buffer));
        CHECK(tmp, perror, "write file", < 0);
        bytes_read += tmp;
    }
}