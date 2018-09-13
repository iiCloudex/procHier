#ifndef _MESSG_H
#define _MESSG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <stdbool.h>

#define SERVER_NAME "/SERVER" // message queue name must start with '/'

#define MAX_MSG_SIZE 1024
#define TYPE 1

typedef struct messg {
    char sender_name[16];
    float val;
} MESSG;

#define oops(ermsg,erno) {perror(ermsg); exit(erno); }

#endif