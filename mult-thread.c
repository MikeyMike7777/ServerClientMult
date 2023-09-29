/************************************************
*
* Author: Michael Mathews
* Assignment: Program 2
* Class: CSI 4337
* File: mult-thread.c
* Description: Uses multiple threads to compute f(x) efficiently
*
************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function rototype
double f(int x);

typedef struct ThreadArgs {
    int start;
    int end;
} ThreadArgs;

// Global mutex and shared variable
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
double shared_product = 1.0;

/************************************************
*
* Function: threadFunct
* Parameters: ThreadArgs (includes a start and end number)
* Return: none
* Description: Computes one or more f(x) functions efficiently and multiplies
* them
*
************************************************/
void *threadFunct(void *args) {
    ThreadArgs *arguments = (ThreadArgs *)args;
    int start = arguments->start;
    int end = arguments->end;
    int x;

    double local_product = 1.0;
    for (x = start; x < end; x++) {
        double fx = f(x);
        if (fx == 0) {
            local_product = 0;
            break;
        }
        local_product *= fx;
    }

    // Update shared resource
    pthread_mutex_lock(&mutex);
    shared_product *= local_product;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // Verify number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./mult <N> <T>\n");
        exit(1);
    }

    // Get input
    int N = atoi(argv[1]);
    int T = atoi(argv[2]);

    if (N == 0) {
        if(strcmp(argv[1], "0") != 0) {
            fprintf(stderr, "Error: Invalid N or T value\n");
            exit(1);
        }
        else {
            printf("Answer: 1\n");
            exit(0);
        }
    }
    N += 1;

    if (N < 0 || T < 1 || T > N) {
        fprintf(stderr, "Error: Invalid N or T value\n");
        exit(1);
    }

    // Initialize threads and arguments
    pthread_t *threads = malloc(sizeof(pthread_t) * (size_t)T);
    ThreadArgs *threadArgs = malloc(sizeof(ThreadArgs) * (size_t)T);

    int rangePerChild = N / T;
    int remainder = N % T;
    int i;

    for (i = 0; i < T; i++) {
        // Split up work
        int start = i * rangePerChild;
        int end = start + rangePerChild;

        if (i < remainder) {
            start += i;
            end += i + 1;
        } else {
            start += remainder;
            end += remainder;
        }

        threadArgs[i].start = start;
        threadArgs[i].end = end;

        // Create and run threads
        pthread_create(&threads[i], NULL, threadFunct, &threadArgs[i]);
    }

    // Wait for threads to finish
    for (i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(threadArgs);

    printf("Answer: %.0f\n", shared_product);
    return 0;
}

