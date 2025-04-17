#ifndef HEAD
#define HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <mqueue.h>

#define QNAME "/queue"
#define MSG 32
#define SIZE 256


#define CHECK(item, msg, text, wrong) ({ if( item == wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })


#endif