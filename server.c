/************************************************
*
* Author: Michael Mathews
* Assignment: Program 1
* Class: CSI 4337
* File: server.c
* Description: Manages the server-side message queue, handles incoming client
*              messages, and keeps track of the number of connected clients.
*
************************************************/

#include "common.h"
#include <signal.h>

int clientCount = 0;
mqd_t get = -1, send;
char buffer[MAX];
int bytesRead = 0;

/************************************************
*
* Function: signalHandler
* Parameters: int sig (signal received)
* Return: void
* Description: Handles the SIGINT signal (Ctrl+C) by giving the total number of
*              connected clients and unlinks the message queues.
*
************************************************/
void sigintHandler(int sig) {
    write(1, "Number of clients: ", 19);
    printf("%d\n", clientCount);
    write(1, "Current Message: ", 17);
    write(1, buffer, (size_t) bytesRead);
    write(1, "\n", 1);

    // Remove connections
    mq_unlink("/mathewsm-client-queue");
    mq_unlink("/mathewsm-server-queue");
    
    (void)sig;
    exit(0);
}

int main(int argc, char** argv) {
    struct sigaction sa;
    sa.sa_handler = sigintHandler;

    // Set message queue attributes
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX;
    attr.mq_curmsgs = 0; 

    // Check command line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: server <message>\n");
        exit(1);
    }

    // Construct command line message and find its length
    bytesRead = findLen(argv, argc);
    constructMessage(buffer, argv, argc);

    // Set up signal handler
    sigaction(SIGINT, &sa, NULL);

    // Open message queues
    mq_open("/mathewsm-server-queue", O_CREAT, 0644, &attr);
    send = openMq("/mathewsm-client-queue", O_CREAT | O_WRONLY, &attr);
    get = mq_open("/mathewsm-server-queue", O_RDONLY, 0644, &attr);

    // Main loop for receiving and sending messages
    while (1) {
        // Get the sending mq's attributes
        getAttr(get, &attr);

        // Exchange messages with client
        sendMessage(send, buffer, bytesRead);
        bytesRead = receiveMessage(get, buffer);
        
        clientCount++;
    }

    // Close message queues
    mq_close(send);
    mq_close(get);

    return 0;
}
