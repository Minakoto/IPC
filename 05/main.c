#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

void sig_file1(int);
void sig_file2(int);

volatile sig_atomic_t fstatus = 1;

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pid_t pid;
    int fd[2];
    char msg[128];
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
        char buf[129];
        buf[128] = '\0';
        struct sigaction sa;
        sa.sa_handler = sig_file1;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
        sa.sa_handler = sig_file2;
        sigaction(SIGUSR2, &sa, NULL);
        for(int i = 0; i < num; i++) {
            while(!fstatus) pause();
            int llf = open("log.txt", O_RDWR);
            if(llf == -1) {
                perror("file");
                exit(EXIT_FAILURE);
            }
            read(llf, buf, sizeof(msg));
            rnd = rand() % 5000;
            write(fd[1], &rnd, sizeof(float));
        }
        exit(EXIT_SUCCESS);
    default:
        close(fd[1]);
        int fl;
        int lf = open("log.txt", O_CREAT|O_WRONLY|O_TRUNC, 0777);
        if(lf == -1) {
            perror("p: file");
            exit(EXIT_FAILURE);
        }
        close(lf);
        for(int i = 0; i < num; i++) {
            fl = open("ttt.txt", O_CREAT|O_WRONLY|O_APPEND, 0777);
            if(fl == -1) {
                perror("p file");
                exit(EXIT_FAILURE);
            }
            read(fd[0], &rnd, sizeof(float));
            int lf = open("log.txt", O_WRONLY|O_APPEND);
            kill(pid, SIGUSR1);
            strncpy(msg, "\nParent: OPEN", sizeof(msg));
            write(lf, msg, sizeof(msg));

            snprintf(msg, sizeof(msg), "\nParent: READ %f", rnd);
            write(lf, msg, sizeof(msg));

            if(i != num - 1) printf("%.3f ", rnd);
            else printf("%.3f ", rnd);
            write(fl, &rnd, sizeof(float));
            close(fl);
            strncpy(msg, "\nParent: CLOSE", sizeof(msg));
            write(lf, msg, sizeof(msg));
            close(lf);
            kill(pid, SIGUSR2);
        }            
    }
}

void sig_file1(int signal) {
    fstatus = 1;
}

void sig_file2(int signal) {
    fstatus = 0;
}