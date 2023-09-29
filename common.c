/************************************************
*
* Author: Michael Mathews
* Assignment: Program 1
* Class: CSI 4337
* File: common.c
* Description: Contains function implementations for message queue attribute
*              settings, opening message queues, and message construction to a
*              void code duplication in server.c and client.c.
*
* Note: All function comments in common.h
************************************************/

#include "common.h"

void setAttr(struct mq_attr *attr, int flags, int maxmsg, int msgsize, int curmsgs) {
    attr->mq_flags = flags;
    attr->mq_maxmsg = maxmsg;
    attr->mq_msgsize = msgsize;
    attr->mq_curmsgs = curmsgs;
}

void getAttr(mqd_t mq, struct mq_attr *attr) {
    if (mq_getattr(mq, attr) == -1) {
        perror("mq_getattr");
        exit(1);
    }
}

mqd_t openMq(char *name, int flags, struct mq_attr *attr) {
    mqd_t mq = mq_open(name, flags, 0644, attr);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }
    return mq;
}

void sendMessage(mqd_t mq, char *message, int len) {
    if (mq_send(mq, message, (size_t)len, 0) == -1) {
        perror("mq_send");
        exit(1);
    }
}

int receiveMessage(mqd_t mq, char *buffer) {
    ssize_t bytesRead;
    if ((bytesRead = mq_receive(mq, buffer, MAX, NULL)) == -1) {
        perror("mq_receive");
        exit(1);
    }
    return (int)bytesRead;
}

int findLen(char **argv, int argc) {
    int len = 0, i;
    for (i = 1; i < argc; i++) {
        len += (int) strlen(argv[i]);
    }
    // Add spaces between words, remove \0 byte
    len += argc - 2;
    return len;
}

void constructMessage(char *message, char **argv, int argc) {
    int bytesRead, i;

    if ((bytesRead = findLen(argv, argc)) > 1024) {
        fprintf(stderr, "Error: Message longer than 1024 characters\n");
        exit(1);
    }

    // Combine words
    strcpy(message, argv[1]);
    for (i = 2; i < argc; i++) {
        strcat(message, " ");
        strcat(message, argv[i]);
    }
}

