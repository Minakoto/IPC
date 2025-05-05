#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h> 

#define CHECK(item, msg, text, wrong) ({ if( item wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

int main(int argc, char *argv[])
{
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
	char buffer[1024];
    printf("TCP DEMO CLIENT\n");
	
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK(my_sock, perror, "socket", < 0);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    portno = atoi(argv[2]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    CHECK(connect(my_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)), perror, "connect", < 0);

    while(1) {

        n = recv(my_sock, &buffer[0], sizeof(buffer), 0);
        CHECK(n, perror, "recieve", < 0);
        buffer[n] = 0;
        printf("S->%s", buffer);
        printf("C->");
        fgets(&buffer[0], sizeof(buffer) - 1, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        send(my_sock, &buffer[0], strlen(&buffer[0]), 0);

        if(strcmp(buffer, "5") == 0) {
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
        send(my_sock, "0", sizeof("0"), 0);
    }
    close(my_sock);
    return 0;

}
