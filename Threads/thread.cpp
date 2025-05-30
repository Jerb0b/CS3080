/* Author: Justin Jereb */
/* Class: CS3080 Operating Systems */
/* Problem: Use threads in order to generate and calculate min, max, & avg */
/* Date: 10/23/2024 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

using namespace std;

/* Global Definitions */
#define NUM_ITEMS 10
#define BUFFER_SIZE 10

/* Global Variables */
int buf[BUFFER_SIZE];
int maxi = 0;
int mini = 10000;
int average = 0;

void* producer(void *) {

        /* Loop to produce numbers into the buffer */
        for (int i = 0; i < NUM_ITEMS; i++) {
                int num = rand() % 1000;
                buf[i] = num;
        }

        pthread_exit(0);
}

void* consumer(void *) {

        /* Loop to grab each number from the buffer and calc based on it */
        for (int i = 0; i < NUM_ITEMS; i++) {
                int num = buf[i];

                /* Checks whether num meets the req to change max/min */
                if (num > maxi) {
                maxi = num;
                }
                if (num < mini) {
                mini = num;
                }

                /* Adds num to average regardless of changing max/min */
                average += num;
        }

        /* Average Calculation */
        average = average / NUM_ITEMS;

        pthread_exit(0);
}

int main(int argc, char* argv[]) {

        /* Seed Generation for use with rand() */
        srand(101);


        pthread_t prod;
        pthread_t cons;

        /* Thread Creation & Joining */
        pthread_create(&prod, NULL, producer, NULL);
        pthread_create(&cons, NULL, consumer, NULL);

        pthread_join(prod,NULL);
        pthread_join(cons,NULL);

        /* Final Output */
        cout << "Min: " << mini << " Max: " << maxi << " Average: " << average << endl;

        return 0;
}
