#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <semaphore.h>

#define CHECK(item, msg, text, wrong) ({ if( item == wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

typedef struct Generated {
    int size;
    float gen_nums[50];
    float min;
    float max;
    char sig_stop;
} gen;

float max(float* array, int size) {
    float res = 0;
    if(size > 0) res = array[0];
    for(int i = 1; i < size; i++) {
        if(res <= array[i]) res = array[i];
    }
    return res;
}

float min(float* array, int size) {
    float res = 0;
    if(size > 0) res = array[0];
    for(int i = 1; i < size; i++) {
        if(res > array[i]) res = array[i];
    }
    return res;
}


sig_atomic_t sig_end = 0;

void endit(int signal) {
    sig_end = 1;
}

void unlock(sem_t* id) {
    CHECK(sem_post(id), perror, "post", -1);

}
void lock(sem_t* id) {
    CHECK(sem_wait(id), perror, "wait", -1);
}

int main() {
    srand(time(NULL));

    sem_t* sem_id;
    sem_id = sem_open("/mysem", O_CREAT, 0600, 1);
    CHECK(sem_id, perror, "sem init", SEM_FAILED);
    CHECK(sem_post(sem_id), perror, "initpost", -1);

    signal(SIGINT, endit);
    key_t key = ftok("shmfile", 65);
    CHECK(key, perror, "key", -1);
    int shm_id = shmget(key, 1024, 0666 | IPC_CREAT);
    CHECK(shm_id, perror, "get", -1);
    int count = 0;
    gen* numbers;
    numbers = (gen*) shmat(shm_id, NULL, 0);
    CHECK(numbers, perror, "shmat chil", (gen*)-1);
    numbers->sig_stop = 0;
    pid_t pid;
    while(!sig_end) {
        switch(pid = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            lock(sem_id);
            sig_end = numbers->sig_stop;
            numbers->max = max(numbers->gen_nums, numbers->size);
            numbers->min = min(numbers->gen_nums, numbers->size);
            unlock(sem_id);
            exit(EXIT_SUCCESS);
        default:
            lock(sem_id);
            sig_end = numbers->sig_stop;
            numbers->size = rand() % 50;
            CHECK(numbers->gen_nums, perror, "shmat par", (float*)-1);
            for(int i = 0; i < numbers->size; i++) numbers->gen_nums[i] = rand() % 1500;
            unlock(sem_id);
            lock(sem_id);
            printf("P found: min %f max %f\n", numbers->min, numbers->max);
            count++;
            unlock(sem_id);
        }
    }
    printf("\nWorked: %d\n", count);
    CHECK(shmdt(numbers), perror, "detach", -1);
    CHECK(shmctl(shm_id, IPC_RMID, NULL), perror, "rmid", -1);
    CHECK(sem_close(sem_id), perror, "close", -1);
    CHECK(sem_unlink("/mysem"), perror, "unlink", -1);
}

