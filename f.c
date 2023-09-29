/************************************************
*
* Author: Michael Mathews
* Assignment: Program 1
* Class: CSI 4337
* File: f.c
* Description: Function to be computed for each value of x
*
************************************************/

#include <stdio.h>
#include <unistd.h>

double f(int x) {
    return (x == 0) ? 1.0 : (double) x;
}

//double f(int x) {
//  sleep(1);
//  return 5.3;
//}

//double f(int x) {
//  static unsigned int executions = 0;
//  if (executions++ > 0) {
//    fprintf(stderr, "Warning:  f(x) executed %u times\n", executions);
//  }
//  return 0;
//}
