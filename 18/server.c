#include "header.h"

void handle(int);
void reorganize(int);
void endit();
void set_server(struct sockaddr_in*, char*, int*);
void server_loop();
void send_ops(int);


int sockfd, newsockfd;
int end = 1, nclients = 1;
struct pollfd fds[MAXCLIENTS + 1];

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    signal(SIGINT, endit);

    char buff[1024];    // Буфер для различных нужд
	int status, portno, pid;
    struct sockaddr_in serv_addr;

	
	set_server(&serv_addr, argv[1], &portno);

    server_loop();

    close(sockfd);
    shutdown(sockfd, SHUT_RDWR);
    return 0;
}

void set_server(struct sockaddr_in* serv_addr, char* argv, int* portno) {
    bzero((char *) serv_addr, sizeof(*serv_addr));
    *portno = atoi(argv);
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = INADDR_ANY;
    serv_addr->sin_port = htons(*portno);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    CHECK(sockfd, perror, "socket", < 0);
	int status = bind(sockfd, (struct sockaddr *) serv_addr, sizeof(struct sockaddr_in));
    CHECK(status, perror, "bind", < 0);

	listen(sockfd,MAXCLIENTS);
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    printf("TCP SERVER DEMO\n");
}

void server_loop() {
    struct sockaddr_in client_addr;
    socklen_t c_len = sizeof(client_addr);
    int newsockfd;
    int poll_count;

    while(end) {
        poll_count = poll(fds, nclients, -1);
        CHECK(poll_count, perror, "poll", == -1);

        for(int i = 1; i < nclients + 1; i++) {
            if(fds[i].revents & POLLIN) {
                handle(fds[i].fd);
            }
            else if(fds[i].revents & POLLHUP) {
                close(fds[i].fd);
                reorganize(i);
                --nclients;
                --i;
            }
        }

        if(fds[0].revents & POLLIN) {
            newsockfd = accept(sockfd, (struct sockaddr*) &client_addr, &c_len);
            CHECK(newsockfd, perror, "accept", == -1);
            if(nclients >= MAXCLIENTS + 1) {
                printf("New connection refused\n");
                close(newsockfd);
            } else {
                fds[nclients].fd = newsockfd;
                fds[nclients].events = POLLIN | POLLHUP;
                send_ops(newsockfd);
                nclients++;
                printf("New connection\n");
            }
        }



    }
    for(int i = 1; i < nclients + 1; i++) close(fds[i].fd);

}

void handle(int socket) {
    char buffer[1024];
    int recieved, a, b, cmd, result;
    a = b = cmd = result = 0;
    printf("Open for input\n");
    recieved = recv(socket, &buffer[0], sizeof(buffer), 0);
    CHECK(recieved, perror, "recv cmd", < 0);
    cmd = atoi(buffer);
    printf("Operation %d\n", cmd);
    if(cmd < 5) {

        switch(cmd) {
            case 0:
                send_ops(socket);
                break;
            case 1:
                recvpars(socket, &a, &b, buffer);
                result = sum(a, b);
                break;
            case 2:
                recvpars(socket, &a, &b, buffer);
                result = sub(a, b);
                break;
            case 3:
                recvpars(socket, &a, &b, buffer);
                result = mult(a, b);
                break;
            case 4:
                recvpars(socket, &a, &b, buffer);
                result = divi(a, b);
                break;
        }
        snprintf(buffer, strlen(buffer), "%d", result);
        buffer[strlen(buffer)] = '\n';
    } else if(cmd == 5) {
        return;
    }
    else strcpy(buffer, "Error\n");
    send(socket, &buffer[0], sizeof(buffer), 0);    
}

void send_ops(int socket) {
    #define MSG_MSG "Command(1 - +, 2 - -, 3 - *, 4 - /)\n"
    CHECK(send(socket, MSG_MSG, sizeof(MSG_MSG), 0), perror, "send ops", == -1);
}

void reorganize(int returnee) {
    for(int i = returnee; i < nclients - 1; i++) fds[i] = fds[i + 1];
}

void endit() {
    end = 0;
    close(sockfd);
    shutdown(sockfd, SHUT_RDWR);
    for(int i = 1; i < nclients; i++) close(fds[i].fd);
}