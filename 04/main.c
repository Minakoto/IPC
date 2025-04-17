#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pid_t pid;
    int fd[2];
    if(pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if(argc != 2) {
        printf("Wrong input\n");
        exit(EXIT_FAILURE);
    }
    int num = atoi(argv[1]);
    float rnd = 0;
    if(num <= 0) {
        printf("Wrong input\n");
        exit(EXIT_FAILURE);
    }
    switch(pid = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            close(fd[0]);
            for(int i = 0; i < num; i++) {
                rnd = rand() % 5000;
                write(fd[1], &rnd, sizeof(float));
            }
            exit(EXIT_SUCCESS);
        default:
            close(fd[1]);
            int fl = open("ttt.txt", O_CREAT|O_WRONLY, 0777);
            for(int i = 0; i < num; i++) {
                read(fd[0], &rnd, sizeof(float));
                if(i != num - 1) printf("%.3f ", rnd);
                else printf("%.3f ", rnd);
                write(fl, &rnd, sizeof(float));
            }
            close(fl);
    }

}