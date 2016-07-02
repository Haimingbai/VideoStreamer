#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "droneCommandHandler.h"


void *startDroneCommandHandlerThread(void *data);
    
void *startDroneCommandHandler(uint16_t port) {
    int32_t err                     = 0; 
	COMMAND_DATA_T *commandData		= NULL;;

    commandData = (COMMAND_DATA_T *)malloc(sizeof(COMMAND_DATA_T));
    if (!commandData) {
        return NULL;
    }
    memset(commandData, 0, sizeof(COMMAND_DATA_T));
    commandData->port = port;
    err = pthread_create(&commandData->threadId, NULL, startDroneCommandHandlerThread, (void *)commandData);
    if(err) {
        printf("Thread creation failed\n");
    }
    return commandData;
}

void *startDroneCommandHandlerThread(void *args) {
    int32_t nBytes              = 0;
    int32_t err                 = 0;
    char buffer[1024]           = {0};
    COMMAND_DATA_T *commandData = args; 
     
    printf("Starting command handler\n");
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    
    /*Create UDP socket*/
    commandData->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(commandData->fd < 0) {
        printf("ERROR opening socket\n");
        strncpy(commandData->shortErrMsg, "ERROR opening socket", 511);
        pthread_exit(NULL);
    }

    /*Configure settings in address struct*/
    memset(&serverAddr, 0, sizeof(serverAddr));  
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(commandData->port);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.42.1");

    /*Bind socket with address struct*/
    err = bind(commandData->fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if(err < 0) {
        printf("ERROR Bind failed\n");
        strncpy(commandData->shortErrMsg, "ERROR Bind failed", 511);
        pthread_exit(NULL);
    }
    printf("Bind pass\n");

    /*Initialize size variable to be used later on*/
    addr_size = sizeof(serverStorage);

    commandData->running = 1;
    while(commandData->running){
        printf("Waiting for messages\n");
        /* Try to receive any incoming UDP datagram. Address and port of 
        requesting client will be stored on serverStorage variable */
        nBytes = recvfrom(commandData->fd,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);

        /*Convert message received to uppercase*/
        // for(i=0;i<nBytes-1;i++)
        //   buffer[i] = toupper(buffer[i]);

        printf("Recieved buffer %d\n", nBytes);
        /*Send uppercase message back to client, using serverStorage as the address*/
        //sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
    }
    pthread_exit(NULL);
}

int32_t isCommandDataHandlerRunning(void *handle) {
    assert(handle);
    COMMAND_DATA_T *data = handle; 
    
    return data->running;
}


