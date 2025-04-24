#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define READER 5

#define CHECK(item, msg, text, wrong) ({ if( item == wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

void r_lock(int id) {
    struct sembuf op1 = {1, 0, SEM_UNDO};
    struct sembuf op2[2] = {{2, 0, SEM_UNDO}, {2, 1, SEM_UNDO}};
    struct sembuf op3[2] = {{2, 0, SEM_UNDO}, {0, 1, SEM_UNDO}};
    CHECK(semop(id, &op1, 1), perror, "op1", -1);
    int ctl = semctl(id, 0, GETVAL);
    if(ctl < READER) CHECK(semop(id, op3, 2), perror, "op2", -1);
    ctl = semctl(id, 0, GETVAL);
    if(ctl == READER) CHECK(semop(id, op2, 2), perror, "op3", -1);
}

void r_unlock(int id) {
    struct sembuf op1 = {0, -1, SEM_UNDO};
    struct sembuf op2 = {2, -1, SEM_UNDO};
    int ctl = semctl(id, 0, GETVAL);
    if(ctl == READER) CHECK(semop(id, &op1, 1), perror, "op4", -1);
    CHECK(semop(id, &op1, 1), perror, "op5", -1);
}

void w_lock(int id) {
    struct sembuf op1[3] = {{0, 0, SEM_UNDO}, {1, 0, SEM_UNDO}, {1, 1, SEM_UNDO}};
    CHECK(semop(id, op1, 3), perror, "op6", -1);
}
void w_unlock(int id) {
    struct sembuf op1 = {1, -1, SEM_UNDO};
    CHECK(semop(id, &op1, 1), perror, "op7", -1);   
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pid_t pid;

    int sem_key = ftok(".", 'S');
    CHECK(sem_key, perror, "key", -1);
    int sem_id = semget(sem_key, 3, 0666|IPC_CREAT); // 0 - read 1 - write 2 - filled
    CHECK(sem_id, perror, "sem init", -1);
    
    int sem_status = semctl(sem_id, 0, SETVAL, 0);
    CHECK(sem_status, perror, "setval", -1);
    sem_status = semctl(sem_id, 1, SETVAL, 0);
    CHECK(sem_status, perror, "setval", -1);
    sem_status = semctl(sem_id, 2, SETVAL, 0);
    CHECK(sem_status, perror, "setval", -1);

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
    for(int i = 0; i < READER; i++) {
        switch(pid = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            close(fd[0]);
            char buf[256];
            for(int i = 0; i < num; i++) {
                rnd = rand() % 5000;
                CHECK(write(fd[1], &rnd, sizeof(float)), perror, "writefd", -1);
    
                r_lock(sem_id);
                int llf = open("ttt.txt", O_RDWR);
                if(llf == -1) {
                    perror("file");
                    exit(EXIT_FAILURE);
                }
                CHECK(read(llf, buf, sizeof(buf)), perror, "read", -1);
                close(llf);
                r_unlock(sem_id);
                int log_c = open("logc.txt", O_CREAT|O_WRONLY|O_APPEND, 0777);
                CHECK(log_c, perror, "log open", -1);
                write(log_c, buf, sizeof(float));
                write(log_c, "\n", sizeof("\n"));
                close(log_c);
        }
        exit(EXIT_SUCCESS);
        }
    }
    
        close(fd[1]);
        int fl;
        int lf = open("log.txt", O_CREAT|O_WRONLY|O_TRUNC, 0777);
        CHECK(lf, perror, "log open", -1);
        close(lf);
        for(int i = 0; i < num; i++) {
            w_lock(sem_id);
            fl = open("ttt.txt", O_CREAT|O_WRONLY|O_APPEND, 0777);
            CHECK(fl, perror, "file open", -1);
            CHECK(read(fd[0], &rnd, sizeof(float)), perror, "read fd", -1);
            if(i != num - 1) printf("%.3f ", rnd);
            else printf("%.3f\n", rnd);
            CHECK(write(fl, &rnd, sizeof(float)), perror, "writep", -1);
            close(fl);
            w_unlock(sem_id);

            int lf = open("log.txt", O_WRONLY|O_APPEND);
            CHECK(lf, perror, "log open", -1);
            strncpy(msg, "\nParent: OPEN", sizeof(msg));
            CHECK(write(lf, msg, strlen(msg)), perror, "write5", -1);

            snprintf(msg, sizeof(msg), "\nParent: READ %f", rnd);
            CHECK(write(lf, msg, strlen(msg)), perror, "write6", -1);

            strncpy(msg, "\nParent: CLOSE", sizeof(msg));
            CHECK(write(lf, msg, strlen(msg)), perror, "write7", -1);
            close(lf);
        }
        sem_status = semctl(sem_id, 0, IPC_RMID);
        CHECK(sem_status, perror, "semctl", -1);
}

