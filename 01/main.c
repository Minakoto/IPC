#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void cnt(int, char*);

int main(int argc, char* argv[]) {
    float area = 0, size = 0;
    int wt;
    pid_t pid;
    switch(pid = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0: //CHILD
            for(int i = 1; i < (argc - 1)/2; i++) cnt(i, argv[i]);
            exit(EXIT_SUCCESS);
        default: //PARENT
            for(int i = (argc - 1)/2; i < argc; i++) cnt(i, argv[i]);
    }
    exit(EXIT_SUCCESS);
}

void cnt(int i, char* arg) {
    float size = atof(arg);
    if(size <= 0) {
        printf("Wrong size\n");
        exit(EXIT_FAILURE);
    }
    float area = size * size;
    printf("num: %f\n", area);
}