/************************************************
*
* Author: Michael Mathews
* Assignment: Program 1
* Class: CSI 4337
* File: common.h
* Description: Contains reusable functions for message queue attribute
*              settings, opening message queues, and message construction to a
*              void code duplication in server.c and client.c.
*
************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 1024

/************************************************
*
* Function: setAttr
* Parameters: mq_attr struct pointer, four integers (flags, maxmsg, msgsize, curmsgs)
* Return: void
* Description: Sets the attributes of a message queue
*
************************************************/
void setAttr(struct mq_attr *attr, int flags, int maxmsg, int msgsize, int curmsgs);

/************************************************
*
* Function: getAttr
* Parameters: mqd_t (message queue descriptor), mq_attr struct pointer
* Return: void
* Description: Retrieves the attributes of a message queue and checks for errors
*
************************************************/
void getAttr(mqd_t mq, struct mq_attr *attr);

/************************************************
*
* Function: openMq
* Parameters: char pointer (queue name), int (flags), mq_attr struct pointer
* Return: mqd_t - message queue descriptor
* Description: Opens a message queue and returns its descriptor
*
************************************************/
mqd_t openMq(char *name, int flags, struct mq_attr *attr);

/************************************************
*
* Function: sendMessage
* Parameters: mqd_t (message queue descriptor), char pointer (message), int (message length)
* Return: void
* Description: Sends a message to a message queue
*
************************************************/
void sendMessage(mqd_t mq, char *message, int len);

/************************************************
*
* Function: receiveMessage
* Parameters: mqd_t (message queue descriptor), char pointer (message buffer)
* Return: int - number of bytes read
* Description: Receives a message from a message queue
*
************************************************/
int receiveMessage(mqd_t mq, char *buffer);

/************************************************
*
* Function: findLen
* Parameters: char pointer array (argv), int (argc)
* Return: int - length of the message
* Description: Calculates the length of a message constructed from command-line arguments.
*
************************************************/
int findLen(char **argv, int argc);

/************************************************
*
* Function: constructMessage
* Parameters: char pointer (message buffer), char pointer array (argv), int (argc)
* Return: void
* Description: Constructs a message from command-line arguments. Exits if the message is longer than 1024 characters.
*
************************************************/
void constructMessage(char *message, char **argv, int argc);

#endif // COMMON_H

