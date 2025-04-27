#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <semaphore.h>

#define CHECK(item, msg, text, wrong) ({ if( item == wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

void unlock(sem_t* id) {
    CHECK(sem_post(id), perror, "post", -1);

}
void lock(sem_t* id) {
    CHECK(sem_wait(id), perror, "wait", -1);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pid_t pid;

    sem_t* sem_id;
    sem_id = sem_open("/mysem", O_CREAT, 0600, 1);
    CHECK(sem_id, perror, "sem init", SEM_FAILED);
    CHECK(sem_post(sem_id), perror, "sem init post", -1);


    int fd[2];
    char msg[128];
    CHECK(pipe(fd), perror, "pipe", -1);
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
        char buf[256];
        for(int i = 0; i < num; i++) {
            rnd = rand() % 5000;
            CHECK(write(fd[1], &rnd, sizeof(float)), perror, "write", -1);
            lock(sem_id);

            int llf = open("ttt.txt", O_RDWR);
            if(llf == -1) {
                perror("file");
                exit(EXIT_FAILURE);
            }
            CHECK(read(llf, buf, sizeof(float)), perror, "read", -1);
            close(llf);
            unlock(sem_id);

            int log_c = open("logc.txt", O_CREAT|O_WRONLY, 0777);
            CHECK(log_c, perror, "log open", -1);
            write(log_c, buf, sizeof(buf));
            close(log_c);
        }
        exit(EXIT_SUCCESS);
    default:
        close(fd[1]);
        int fl;
        int lf = open("log.txt", O_CREAT|O_WRONLY|O_TRUNC, 0777);
        CHECK(lf, perror, "log open", -1);
        close(lf);
        for(int i = 0; i < num; i++) {
            lock(sem_id);
            fl = open("ttt.txt", O_CREAT|O_WRONLY|O_APPEND, 0777);
            CHECK(fl, perror, "file open", -1);
            CHECK(read(fd[0], &rnd, sizeof(float)), perror, "read fd", -1);
            if(i != num - 1) printf("%.3f ", rnd);
            else printf("%.3f\n", rnd);
            CHECK(write(fl, &rnd, sizeof(float)), perror, "write", -1);
            close(fl);
            unlock(sem_id);
            int lf = open("log.txt", O_WRONLY|O_APPEND);
            CHECK(lf, perror, "log open", -1);
            strncpy(msg, "\nParent: OPEN", sizeof(msg));
            CHECK(write(lf, msg, strlen(msg)), perror, "write", -1);

            snprintf(msg, sizeof(msg), "\nParent: READ %f", rnd);
            CHECK(write(lf, msg, strlen(msg)), perror, "write", -1);

            strncpy(msg, "\nParent: CLOSE", sizeof(msg));
            CHECK(write(lf, msg, strlen(msg)), perror, "write", -1);
            close(lf);

        }
    }
    CHECK(sem_close(sem_id), perror, "close", -1);
    CHECK(sem_unlink("/mysem"), perror, "unlink", -1);
}

