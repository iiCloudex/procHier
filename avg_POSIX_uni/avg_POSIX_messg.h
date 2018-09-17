#ifndef _MESSG_H
#define _MESSG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <stdbool.h>

#define MONITOR_QUEUE "/MONITOR" // message queue name must start with '/'
#define NODE_NAME_PREFIX "NODE_"

#define MAX_MSG_SIZE 1024
#define TYPE 1

typedef struct messg {
    bool stable;
    int nodeId;
    float temperature;
} MESSG;

typedef struct temperature {
	mqd_t msqid;
	float previousTemperature;
} TEMPERATURE;


#define oops(ermsg,erno) {perror(ermsg); exit(erno); }

#endif
