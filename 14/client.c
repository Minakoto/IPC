#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define CHECK(item, msg, text, wrong) ({ if( item wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

#define IP "127.0.0.1"


int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK(sock, perror, "socket", < 0);
    int status;

    struct sockaddr_in s_adr, c_adr;

    s_adr.sin_family = AF_INET;
    s_adr.sin_port = htons(9001);
    s_adr.sin_addr.s_addr = inet_addr(IP);

    c_adr.sin_family = AF_INET;
    c_adr.sin_port = htons(9002);
    c_adr.sin_addr.s_addr = inet_addr(IP);
    status = bind(sock, (struct sockaddr*)&c_adr, sizeof(c_adr));
    CHECK(status, perror, "bind", < 0);

    char buffer[1024];
    ssize_t size = 0;
    while(1) {
        printf("OUT_MSG->");
        fgets(buffer, 1024, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        size = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&s_adr, sizeof(s_adr));
        if(strcmp(buffer, "exit") == 0) break;
        CHECK(size, perror, "send", == -1);
        size = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
        if(strcmp(buffer, "exit") == 0) break;
        buffer[size] = 0;
        printf("IN_MSG->%s\n", buffer);
    }
    close(sock);
}