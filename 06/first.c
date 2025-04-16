#include "head.h"

int main(int argc, char *argv[]) {
    key_t chat_key;
    int queue_id, msg_status;
    chat_key = ftok("chat", 'C');
    CHECK(chat_key, perror, -1);
    queue_id = msgget(chat_key, 0666|IPC_CREAT);
    CHECK(queue_id,perror, -1);
    struct msgbuf msg;
    while(1) {
        printf("Type: ");
        fgets(msg.msgtext, MSG_MAX, stdin);
        CHECK(msg.msgtext, printf, NULL);
        msg.msgtype = 1;
        (strncmp(msg.msgtext, "q", strlen(msg.msgtext)) == 10) ? msg.msgtype = 255 : 0;
        msg_status = msgsnd(queue_id, &msg, sizeof(msg.msgtext), 0);
        CHECK(msg_status, perror, -1);
        if(msg.msgtype == 255) break;
        msg_status = msgrcv(queue_id, &msg, sizeof(msg.msgtext), 2, 0);
        CHECK(msg_status, perror, -1);
        printf("Recieved Message: %s\n", msg.msgtext);
    }
    msgctl(queue_id, IPC_RMID, NULL);
    exit(0);
}
