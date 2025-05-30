// Author - Justin Jereb
// Begin Date - 8/29/2024
// End Date - 9/4/2024
// Purpose - Output a set of random numbers based on user arguments

#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
        int index;

        // This verifies that the correct minimum of arguments were entered

        if (argc < 5) {
                cout << "*** ERROR DETECTED: LESS THAN FIVE ARGUMENTS ***" << endl;
                exit(0);
        }

        // This section converts the input arguments into their data type

        string FileName = argv[1];
        int numItem = atoi(argv[2]);
        int range = atoi(argv[3]);
        int seedNum = atoi(argv[4]);

        // Opens the output file, generates the seed based on input, and outputs the requested amount of randomized numbers within the given range, and closes the output file

        ofstream outfile;
        outfile.open (FileName);
        srand(seedNum);
        for (int i = 0; i < numItem; i++) {
                int temp = (rand() % range);
                outfile << temp << endl;
        }
        outfile.close();

        // Opens the input stream, and outputs the numbers separating them by a tab, then closes the input

        ifstream infile;
        infile.open (FileName);
        string line;
        for (int i = 0; i < numItem; i++) {
                getline(infile,line);
                cout << line << "       ";
        }
        cout << endl;
        infile.close();


        return 0;
}
