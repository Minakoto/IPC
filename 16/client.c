#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>

void send_file(int socket);
void recv_file(int socket);
void handle(int my_sock);

#define CHECK(item, msg, text, wrong) ({ if( item wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

int main(int argc, char *argv[])
{
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    printf("TCP DEMO CLIENT\n");
	
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK(my_sock, perror, "socket", < 0);

    portno = atoi(argv[2]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    CHECK(connect(my_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)), perror, "connect", < 0);

    handle(my_sock);

    close(my_sock);
    return 0;

}

void handle(int my_sock) {
    char buffer[1024];
    int n;
    while(1) {
        fflush(stdout);
        buffer[0] = 0;
        n = recv(my_sock, &buffer[0], sizeof(buffer), 0);
        CHECK(n, perror, "recieve", < 0);
        
        printf("S->%s", buffer);
        printf("C->");
        fgets(&buffer[0], sizeof(buffer) - 1, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        send(my_sock, &buffer[0], strlen(&buffer[0]), 0);


        if(strcmp(buffer, "5") == 0) {
            send_file(my_sock);
            continue;
        }
        else if(strcmp(buffer, "6") == 0) {
            recv_file(my_sock);
            continue;
        }
        else if(strcmp(buffer, "7") == 0) {
            printf("\nExit");
            break;
        }
        printf("C1->");
        fgets(&buffer[0], sizeof(buffer) - 1, stdin);
        send(my_sock, &buffer[0], strlen(&buffer[0]), 0);
        buffer[strcspn(buffer, "\n")] = 0;
        
        printf("C2->");
        fgets(&buffer[0], sizeof(buffer) - 1, stdin);
        send(my_sock, &buffer[0], strlen(&buffer[0]), 0);
        buffer[strcspn(buffer, "\n")] = 0;
        n = recv(my_sock, &buffer[0], sizeof(buffer), 0);
        CHECK(n, perror, "recieve", < 0);
        buffer[strcspn(buffer, "\n")] = 0;
        printf("S->%s\n", buffer);
    }
}

void send_file(int socket) {
    char buffer[1024];
    char filename[1024];
    int tmp = 0, bytes_read = 0, bytes_sent = 0, filesize = 0;
    int filefd;

    printf("File: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    filefd = open(filename, O_RDONLY);
    CHECK(filefd, perror, "file open", == -1);
    filesize = lseek(filefd, 0L, SEEK_END);
    lseek(filefd, 0L, SEEK_SET);
    bytes_sent = send(socket, &filename[0], sizeof(filename), 0);
    CHECK(bytes_sent, perror, "write fname", < 0);
    bytes_sent = send(socket, &filesize, sizeof(int), 0);
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
}

void recv_file(int socket) {
    char buffer[1024];
    char filename[1024];
    int tmp = 0, bytes_read = 0, bytes_sent = 0, filesize = 0;
    int filefd;
    
    printf("File: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    bytes_sent = send(socket, &filename[0], sizeof(filename), 0);
    CHECK(bytes_sent, perror, "write fname", < 0);

    filefd = open(filename, O_CREAT|O_WRONLY, 0600);
    CHECK(filefd, perror, "creat file", == -1);

    bytes_read = recv(socket, &filesize, sizeof(filesize), 0);
    CHECK(bytes_read, perror, "read flen", < 0);
    printf("%d", filesize);
    while(bytes_read < filesize) {
        tmp = recv(socket, &buffer[0], sizeof(buffer), 0);
        bytes_read += tmp;
        CHECK(tmp, perror, "read file", < 0);
        tmp = write(filefd, &buffer[0], strlen(buffer));
        CHECK(tmp, perror, "write file", < 0);
        printf("\n%d", filesize);
        if(bytes_read >= filesize) break;
    }
    int confirm = 1;
    send(socket, &confirm, sizeof(confirm), 0);
}
