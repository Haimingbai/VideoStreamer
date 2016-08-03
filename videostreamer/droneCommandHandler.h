#ifndef DRONE_COMMAND_HANDLER_H
#define DRONE_COMMAND_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "socklibCommon.h"


#if 0
typedef struct COMMAND_DATA_T {
    char        shortErrMsg[512];
    int32_t     running;
    uint16_t    port;
	int32_t		fd;
    pthread_t   threadId;
    struct sockaddr_in serverAddr;
}COMMAND_DATA_T;
#endif

void *startDroneCommandHandler(unsigned char *droneIp, uint16_t dronePort);
void *initDroneComm(char *droneIp, uint16_t senderPort, uint16_t receiverPort, RECEIVER_CB callback);
int32_t startVideoStreaming(void *handle);
void sendAck(void *handle, unsigned char *buffer, int32_t bufLen);

#endif //DRONE_COMMAND_HANDLER_H
