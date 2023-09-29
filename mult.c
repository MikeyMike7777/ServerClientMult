/************************************************
*
* Author: Michael Mathews
* Assignment: Program 1
* Class: CSI 4337
* File: mult.c
* Description: Uses multiple processes to compute f(x) efficiently
*
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//Function Prototype
double f(int x);

#define READ_END 0
#define WRITE_END 1
#define MAX_CHILDREN 100

int main(int argc, char *argv[]) {
    // Verify correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./mult <N> <P>\n");
        exit(1);
    }

    // Get input
    int N = atoi(argv[1]);
    int P = atoi(argv[2]);

    // Validate atoi worked or return 1 if 0!
    if (N == 0) {
        if(strcmp(argv[1], "0") != 0) {
            fprintf(stderr, "Error: Invalid N or P value\n");
            exit(1);
        }
        else {
            printf("Answer: 1\n");
        }
    }
    N += 1;

    // Validate numbers
    if (N < 0 || P < 1 || P > N) {
        fprintf(stderr, "Error: Invalid N or P value\n");
        exit(1);
    }

    if (P > MAX_CHILDREN){
        fprintf(stderr, "Error: Requested more than max processes (100)\n");
        exit(1);
    }

    // Create pipe
    int pipefd[MAX_CHILDREN][2];
    pid_t childPIDs[MAX_CHILDREN];

    // Find how many calculations per process
    int rangePerChild = N / P;
    int remainder = N % P;
    int i, x;
    
    for (i = 0; i < P; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("Error: Failed to create pipe");
            exit(1);
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("Error: Failed to fork child");
            exit(1);
        } 
        else if (pid == 0) {
            close(pipefd[i][READ_END]);

            // Set starting and ending locations for process calculations
            double ans = 1.0;
            int start = i * rangePerChild;
            int end = start + rangePerChild;
            
            // Evenly distribute extra values to processes
            if (i < remainder) {
                start += i;
                end += i+1;
            }
            else {
                start += remainder;
                end += remainder;
            }

            // Do calculation
            for (x = start; x < end; x++) {
                double fx = f(x);
                if (fx == 0) {
                    ans = 0;
                    break;
                }
                ans *= fx;
            }
            
            // Send answer
            write(pipefd[i][WRITE_END], &ans, sizeof(ans));
            close(pipefd[i][WRITE_END]);
            exit(0);
        } else {
            childPIDs[i] = pid;
        }
    }

    double finalProduct = 1.0;
    for (i = 0; i < P; i++) {
        close(pipefd[i][WRITE_END]);

        double childProduct;

        // Recieve info
        read(pipefd[i][READ_END], &childProduct, sizeof(childProduct));
        
        // Calculate total
        if (childProduct == 0) {
            finalProduct = 0;
            break;
        }
        finalProduct *= childProduct;

        // Cleanup
        close(pipefd[i][READ_END]);
        waitpid(childPIDs[i], NULL, 0);
    }

    printf("Answer: %.0f\n", finalProduct);
    return 0;
}

