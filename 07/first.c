#include "head.h"


int main(int argc, char *argv[]) {
    int msg_status, pri;
    char msg[SIZE];
    mqd_t mq;
    struct mq_attr q_attr;
    q_attr.mq_maxmsg = 16;
    q_attr.mq_msgsize = 256;
    mq = mq_open(QNAME, O_CREAT|O_RDWR, 0600, &q_attr);
    CHECK(mq, perror, "queue init", (mqd_t)-1);

    while(1) {
        printf("Type: ");
        fgets(msg, SIZE, stdin);
        msg[strlen(msg)] = '\0';
        CHECK(msg, printf, "fgets", NULL);

        pri = 1;
        (strncmp(msg, "q", strlen(msg)) == 10) ? pri = 255 : 0;
        
        msg_status = mq_send(mq, msg, strlen(msg), pri);
        CHECK(msg_status, perror, "mq_send", -1);

        if(pri == 255) {
            msg_status = mq_close(mq);
            CHECK(msg_status, perror, "close", -1);
            break;
        }
        msg_status = mq_receive(mq, msg, sizeof(msg), &pri);
        CHECK(msg_status, perror, "mq_receive", -1);
        if(pri == 255) break;
        printf("Recieved Message: %s\n", msg);
    }
    exit(0);
}
