#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <iostream>

/* Declarations */
#define FIFO    0
#define LRU     1
#define NUM_REF 100
#define FRAME_START     3
#define FRAME_END       7

/* Page Recorder & References */
int pf_record[2][5];
int Ref[NUM_REF] = {1, 2, 7, 1, 2, 1, 3, 4, 2, 5, 2, 7, 8, 9, 1, 2, 7, 2, 1, 3,
                    1, 7, 2, 3, 2, 1, 2, 4, 5, 4, 5, 6, 7, 8, 9, 6, 3, 1, 2, 3,
                    1, 2, 3, 6, 7, 5, 6, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 7,
                    7, 7, 2, 2, 1, 7, 6, 7, 8, 7, 8, 9, 7, 8, 2, 7, 8, 3, 5, 3,
                    5, 7, 1, 3, 1, 3, 1, 3, 2, 3, 4, 5, 4, 6, 4, 7, 3, 8, 3, 9};

/* Function Prototypes */
void FIFO_Sim();
void LRU_Sim();
void PrintResults();

using namespace std;

int main() {
        /* Function Calls */
        FIFO_Sim();
        LRU_Sim();
        PrintResults();
        return 0;
}

/* Function which simulates the FIFO Page Replacement Algorithm */
void FIFO_Sim() {
        for (int frames = FRAME_START; frames <= FRAME_END; frames++) {
                /* Local Variable Declaration */
                int FIFO_queue[frames];
                int front = 0;
                int rear = 0;
                int size = 0;
                int pageFaults = 0;

                /* FIFO */
                for (int i = 0; i < NUM_REF; i++) {
                        int page = Ref[i];
                        bool found = false;

                        for (int j = 0; j < size; j++) {
                                if (FIFO_queue[(front + j) % frames] == page) {
                                        found = true;
                                        break;
                                }
                        }

                        if (!found) {
                                if (size == frames) {
                                        front = (front + 1) % frames;
                                        size--;
                                }
                                FIFO_queue[rear] = page;
                                rear = (rear + 1) % frames;
                                size++;
                                pageFaults++;
                        }
                }

                /* Record Number of Faults */
                pf_record[FIFO][frames - FRAME_START] = pageFaults;
        }
}

void LRU_Sim() {
        for (int frames = FRAME_START; frames <= FRAME_END; frames++) {
                /* Local Variable Declaration */
                int LRU_list[frames];
                int LRU_time[frames];
                int time = 0;
                int pageFaults = 0;

                /* Initialization of LRU List & Time Arrays */
                for (int i = 0; i < frames; i++) {
                        LRU_list[i] = -1;
                        LRU_time[i] = 0;
                }

                /* LRU */
                for (int i = 0; i < NUM_REF;i++) {
                        int page = Ref[i];
                        bool found = false;
                        int LRU_index = 0;

                        for (int j = 0; j < frames; j++) {
                                if (LRU_list[j] == page) {
                                        found = true;
                                        LRU_time[j] = time++;
                                        break;
                                }
                                if (LRU_time[j] < LRU_time[LRU_index]) {
                                        LRU_index = j;
                                }
                        }

                        if (!found) {
                                LRU_list[LRU_index] = page;
                                LRU_time[LRU_index] = time++;
                                pageFaults++;
                        }
                }

                /* Record Number of Faults */
                pf_record[LRU][frames - FRAME_START] = pageFaults;
        }
}

void PrintResults() {

        /* Local Variable Declaration */
        int minType;
        int minFaults = 99999;
        int minFrame = 999999;

        /* Output for Page Faults for each Page Replacement Algorithm */
        cout << "Page Replacement Algorithm Evaluation Results" << endl;

        /* FIFO Output Segment */
        cout << "FIFO Results" << endl;
        for (int i = 0; i < 5; i++) {
                cout << "Frame Number " << i+3 << ": " << pf_record[0][i] << " Page Faults" << endl;
        }
        cout << endl;

        /* LRU Output Segment */
        cout << "LRU Results" << endl;
        for (int i = 0; i < 5; i++) {
                cout << "Frame Number " << i+3 << ": " << pf_record[1][i] << " Page Faults" << endl;
        }
        cout << endl;

        /* Find Min Type, Frame, and Faults */
        for (int i = 1; i <= FRAME_END - FRAME_START; i++) {
                if (pf_record[FIFO][i] < minFaults) {
                        minFaults = pf_record[FIFO][i];
                        minType = 0;
                        minFrame = FRAME_START + i;
                }
                if (pf_record[LRU][i] < minFaults) {
                        minFaults = pf_record[LRU][i];
                        minType = 1;
                        minFrame = FRAME_START + i;
                }

        }
        /* Output for Minimum Page Fault between Algorithms with Frame Number and Fault Number listed */
        cout << "Minimum Page Fault: Algorithm [";
        if (minType == 0) {
                cout << "F]" << endl;
        }
        if (minType == 1) {
                cout << "L]" << endl;
        }

        cout << "Frame Number: " << minFrame << endl;
        cout << "Page Faults: " << minFaults << endl;
}
