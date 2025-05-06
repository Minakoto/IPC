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
#include <sys/mman.h>

#define READER 5

#define CHECK(item, msg, text, wrong) ({ if( item == wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

sem_t* r_id, *w_id;
int *count;


void r_lock() {
    sem_wait(r_id);
    *count++;
    if (*count == 1) {
        sem_wait(w_id);
    }
    sem_post(r_id);
}

void r_unlock() {
    sem_wait(r_id);
    *count--;
    if (*count == 0) {
        sem_post(w_id);
    }
    sem_post(r_id);
}

void w_lock() {
    sem_wait(w_id);

}
void w_unlock() {
    sem_post(w_id);

}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    pid_t pid;

    count = mmap(NULL, sizeof(count), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    CHECK(count, perror, "mmap", MAP_FAILED);
    r_id = sem_open("/read_sem", O_CREAT, 0600, 1);
    CHECK(r_id, perror, "sem init", SEM_FAILED);
    w_id = sem_open("/write_sem", O_CREAT, 0600, 1);
    CHECK(w_id, perror, "sem init", SEM_FAILED);

    int fd[2];
    char msg[128];
    CHECK(pipe(fd), perror, "pipe", -1);
    if(argc != 2) {
        printf("Wrong input\n");
        exit(EXIT_FAILURE);
    }

    int num = atoi(argv[1]);
    int rnd = 0;
    if(num <= 0) {
        printf("Wrong input\n");
        exit(EXIT_FAILURE);
    }
    for(int j = 0; j < READER; j++) {
        switch(pid = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            close(fd[0]);
            char buf[256];
            for(int i = 0; i < num; i++) {
                if(j == 0) {
                    rnd = rand() % 5000;
                    CHECK(write(fd[1], &rnd, sizeof(int)), perror, "write", -1);
                }
                r_lock();
                int llf = open("ttt.txt", O_RDWR);
                if(llf == -1) {
                    perror("file");
                    exit(EXIT_FAILURE);
                }
                CHECK(read(llf, buf, sizeof(buf)), perror, "read", -1);
                close(llf);
                r_unlock();
                
                int log_c = open("logc.txt", O_CREAT|O_WRONLY|O_APPEND, 0777);
                CHECK(log_c, perror, "log open", -1);
                write(log_c, "Reader", sizeof("Reader"));
                // write(log_c, count, sizeof(int));
                write(log_c, buf, strlen(buf));
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
        w_lock();
        fl = open("ttt.txt", O_CREAT|O_WRONLY|O_APPEND, 0777);
        CHECK(fl, perror, "file open", -1);
        CHECK(read(fd[0], &rnd, sizeof(int)), perror, "read fd", -1);
        if(i != num - 1) printf("%d ", rnd);
        else printf("%d\n", rnd);
        CHECK(write(fl, &rnd, sizeof(int)), perror, "write", -1);
        close(fl);
        w_unlock();
        lf = open("log.txt", O_WRONLY|O_APPEND);
        CHECK(lf, perror, "log open", -1);
        strncpy(msg, "\nParent: OPEN", sizeof(msg));
        CHECK(write(lf, msg, strlen(msg)), perror, "write", -1);
        snprintf(msg, sizeof(msg), "\nParent: READ %d", rnd);
        CHECK(write(lf, msg, strlen(msg)), perror, "write", -1);
        strncpy(msg, "\nParent: CLOSE", sizeof(msg));
        CHECK(write(lf, msg, strlen(msg)), perror, "write", -1);
        close(lf);
    }
    CHECK(sem_close(r_id), perror, "close", -1);
    CHECK(sem_unlink("/read_sem"), perror, "unlink", -1);
    CHECK(sem_close(w_id), perror, "close", -1);
    CHECK(sem_unlink("/write_sem"), perror, "unlink", -1);
}

