/* Author: Justin Jereb */
/* Class: CS3080 Operating Systems */
/* Problem: Use multiple threads in order to generate and calculate min, max, & avg */
/* Date: 11/7/2024 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <vector>

using namespace std;

/* Global Definitions */
#define BUFFER_SIZE 10
#define NUM_THREADS 6

/* Global Variables */
int buf[BUFFER_SIZE];
int count = 0;
int nextIn = 0;
int nextOut = 0;
int numItems = 0;
int range = 0;
int threader = 0;

/* Global Vectors */
vector<int> globalMin = {10000, 10000, 10000, 10000, 10000};
vector<int> globalMax = {0, 0, 0, 0, 0, 0};
vector<int> globalAvg = {0, 0, 0, 0, 0, 0};


/* Pthread Stuff */
pthread_cond_t empty, full;
pthread_mutex_t lock;

void* producer(void *) {
        for (int i = 0; i < numItems; i++) {
                pthread_mutex_lock(&lock);

                /* Check if full */
                while (count >= BUFFER_SIZE) {
                        pthread_cond_wait(&empty, &lock);
                }

                /* Random Number Generation & Count Increment */
                int num = rand() % 1000;
                buf[nextIn] = num;
                nextIn = (nextIn + 1) % BUFFER_SIZE;
                count++;

                /* Signal full and unlock */
                pthread_cond_signal(&full);
                pthread_mutex_unlock(&lock);
        }
        pthread_exit(0);
}

void* consumer(void *) {
        /* Local Variables */
        int myMin = 10000;
        int myMax = 0;
        int myAvg = 0;

        /* Loop to grab each number from the buffer and calc based on it */
        for (int i = 0; i < numItems; i++) {
                pthread_mutex_lock(&lock);

                /* Check if empty */
                while (count <= 0) {
                        pthread_cond_wait(&full, &lock);
                }

                /* Number Receiver & Count Decrement */
                int num = buf[nextOut];
                nextOut = (nextOut + 1) % BUFFER_SIZE;
                count--;

                /* Signal empty and unlock */
                pthread_cond_signal(&empty);
                pthread_mutex_unlock(&lock);

                /* Max/Min Check */
                if (num > myMax) {
                        myMax = num;
                }
                if (num < myMin) {
                        myMin = num;
                }

                /* Adds num to average regardless of changing max/min */
                myAvg += num;
        }


        /* Average Calculation */
        myAvg = myAvg / numItems;

        /* Consumer Individual Output */
        pthread_t myID = pthread_self();
        printf("Thread: %d, My Min: %d, My Max: %d, My Avg: %d\n", myID, myMin, myMax, myAvg);


        /* Pass myVariables to global */
        pthread_mutex_lock(&lock);

        globalMin[threader] = myMin;
        globalMax[threader] = myMax;
        globalAvg[threader] = myAvg;
        threader++;

        pthread_mutex_unlock(&lock);

        pthread_exit(0);
}

int main(int argc, char* argv[]) {

        /* Validates whether numItems & range were submitted*/
        if (argc < 3) {
                cout << "Error, numItems/range not detected, please try again" << endl;
                return 1;
        }

        /* Pthread's Init Statements */
        pthread_cond_init(&empty, NULL);
        pthread_cond_init(&full, NULL);
        pthread_mutex_init(&lock, NULL);

        /* Seed Generation for rand() */
        srand(101);

        /* Importing to global variables */
        numItems = atoi(argv[1]);
        range = atoi(argv[2]);

        /* Variable Declaration */
        int totMin;
        int totMax;
        double totAvg;

        pthread_t prod[NUM_THREADS];
        pthread_t cons[NUM_THREADS];

        /* Thread Creation & Joining */
        for (int i = 0; i < NUM_THREADS; i++) {
                pthread_create(&prod[i], NULL, producer, NULL);
                pthread_create(&cons[i], NULL, consumer, NULL);
        }
        for (int i = 0; i < NUM_THREADS; i++) {
                pthread_join(prod[i], NULL);
                pthread_join(cons[i], NULL);
        }

        /* Global Variables Calculation */
        for (int i = 0; i < NUM_THREADS; i++) {
                if (totMin > globalMin[i]) {
                        totMin = globalMin[i];
                }
                if (totMax < globalMax[i]) {
                        totMax = globalMax[i];
                }
                totAvg += globalAvg[i];
        }
        totAvg = totAvg / NUM_THREADS;

        /* Final Output */
        cout << "Min: " << totMin << " Max: " << totMax << " Average: " << totAvg << endl;

        /* Cleaning */
        pthread_cond_destroy(&empty);
        pthread_cond_destroy(&full);
        pthread_mutex_destroy(&lock);

        return 0;
}
