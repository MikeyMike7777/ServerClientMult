/************************************************
*
* Author: Michael Mathews
* Assignment: Program 1
* Class: CSI 4337
* File: client.c
* Description: Handles client-side message queue operations, including sending
*              messages to the server and receiving previous messages.
*
************************************************/

#include "common.h"

int main(int argc, char **argv){
    char oldMessage[MAX], message[MAX];

    // Remove mask for file creation
    mode_t mask = umask(0);

    // Set message queue attributes
    struct mq_attr attr;

    // Check command line arguments
    if(argc != 2){
        fprintf(stderr, "Usage: client <message>\n");
        exit(1);
    }

    // Get length of entire message
    int len = findLen(argv, argc);
    // Put message into one character array
    constructMessage(message, argv, argc);

    // Open message queues
    mqd_t get = openMq("/mathewsm-client-queue", O_RDONLY, &attr);
    mqd_t send = openMq("/mathewsm-server-queue", O_WRONLY, &attr);

    // Get the sending mq's attributes
    getAttr(get, &attr);

    // Receive and display the previous message
    ssize_t bytesRead = receiveMessage(get, oldMessage);
    write(1, "Previous Message: ", 18);
    write(1, oldMessage, (size_t) bytesRead);
    write(1, "\n", 1);

    // Send new message
    sendMessage(send, message, len);

    // Close message queues and restore umask
    mq_close(get);
    mq_close(send);
    umask(mask);

    return 0;
}

