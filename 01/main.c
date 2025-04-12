#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char* argv[]) {
    float area = 0, size = 0;
    int wt;
    int fd[2];
    if(argv[1] == NULL) {
        printf("No size specified\n");
        exit(EXIT_FAILURE);
    }
    size = atof(argv[1]);
    if(size <= 0) {
        printf("Wrong size\n");
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    pipe(fd);
    switch(pid = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0: //CHILD
            close(fd[0]);
            float t = size * (size/2);
            write(fd[1], &t, sizeof(float));
            exit(EXIT_SUCCESS);
        default: //PARENT
            close(fd[1]);
            area += size * (size/2);
            float tmp;
            wait(&wt);
            read(fd[0], &tmp, sizeof(float));
            area += tmp;
    }
    printf("Area: %.0f\n", area);
    exit(EXIT_SUCCESS);
}