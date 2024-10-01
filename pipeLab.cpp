#include <iostream>
#include <sys/types.h>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>


using namespace std;

int main(int argc, char* argv[]) {
	int buf_id[100];
	int count = 0;
	int pid;
	int pipefd[2];
	
	int numItems = atoi(argv[1]);
	int range = atoi(argv[2]);
	int seed = atoi(argv[3]);

	pid = fork();

	if (pid < 0) {
		cout << "ERROR, PID LESS THAN ZERO" << endl;
		return 1;
	}

	if (pid == 0) {
		close(pipefd[1]);
		int numItemsRec = 0;
		int min, max;
		double avg;

		int realID = getpid();

		for (int i = 0; i < numItems; i++) {
			read(pipefd[0], buf_id, sizeof(buf_id));
			if (i == 0) {
				min = buf_id[i];
				max = buf_id[i];
				avg += buf_id[i];
			}
			if (i > 0) {
				if (buf_id[i] < min) {
					min = buf_id[i];
				}
				if (buf_id[i] > max) {
					max = buf_id[i];
				}
				avg += buf_id[i];
			}
			numItemsRec++;
		}

		close(pipefd[0]);
		avg = avg / numItemsRec;
		cout << realID << ": Number Items Received: " << numItemsRec << " Min: " << min << " Max: " << max << "Average: " << avg;

		}

	if (pid > 0) {
		close(pipefd[0]);
		srand(seed);
		int numItemsWrit = 0;
		int num;
		int realID;
		
		realID = getpid();

		for (int i = 0; i < numItems; i++) {
			buf_id[i] = rand();
			write(pipefd[1], buf_id, sizeof(buf_id));
			numItemsWrit++;
		}

		close(pipefd[1]);
		cout << realID << ": Number of Items Written to Pipe: " << numItemsWrit << endl;
	}
	

	return 0;
}