#ifndef HEAD
#define HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_MAX 256

struct msgbuf {
    long msgtype;
    char msgtext[MSG_MAX];
};

#define CHECK(item, msg, wrong) ({ if( item == wrong) {\
msg(#item);\
exit(EXIT_FAILURE);\
} })


#endif