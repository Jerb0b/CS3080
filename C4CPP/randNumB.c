#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {


        /* Verifies if minimum arguments was entered */
        if (argc < 5) {
                printf("***ERROR DETECTED: LESS THAN FIVE ARGUMENTS***\n");
                exit(0);
        }

        /* Converts input arguments into respective data types */
        char *FileName = argv[1];
        int numItem = atoi(argv[2]);
        int range = atoi(argv[3]);
        int seedNum = atoi(argv[4]);

        /* Creates file, generates the seed based on input, then outputs the
        requested amount of randomized number within the given range, and
        closes the output file */

        FILE *outFile;
        outFile = fopen(FileName, "w");
        srand(seedNum);
        for (int i = 0; i < numItem; i++) {
                int temp = (rand() % range);
                fprintf(outFile, "%d\t\0", temp);
        }
        fprintf(outFile, "\n");
        fclose(outFile);

        /* Opens the input file, then outputs the numbers from the file while
        separating them by a tab, then closes the file */

        FILE *inFile;
        inFile = fopen(FileName, "r");
        char buffer[128];
        fgets(buffer, sizeof(buffer), inFile);
        printf("%s\0", buffer);
        fclose(inFile);

        return 0;
}
