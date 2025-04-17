#include "head.h"


int main(int argc, char *argv[]) {
    int msg_status, pri, pr;
    char msg[SIZE];
    mqd_t mq;
    mq = mq_open(QNAME, O_CREAT|O_RDWR, 0600, NULL);
    CHECK(mq, perror, "queue init", (mqd_t)-1);

    while(1) {

        msg_status = mq_receive(mq, msg, sizeof(msg), NULL);
        CHECK(msg_status, perror, "mq_receive", -1);
        
        msg_status = mq_receive(mq, msg, strlen("q"), &pri);
        if(pri == 255) break;
        printf("Recieved Message: %s\n", msg);

        printf("Type: ");
        fgets(msg, SIZE, stdin);
        msg[strlen(msg)] = '\0';
        CHECK(msg, printf, "fgets", NULL);

        pri = 2;
        (strncmp(msg, "q", strlen(msg)) == 10) ? pri = 255 : 0;
        
        msg_status = mq_send(mq, msg, strlen(msg), pri);
        CHECK(msg_status, perror, "mq_send", -1);

        if(pri == 255) {
            msg_status = mq_close(mq);
            CHECK(msg_status, perror, "close", -1);
            break;
        }

    }
    exit(0);
}
