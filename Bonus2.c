#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
const int arrSize = 1e2;
const int processCnt = 2;

// Get the sum of an array using four processes and shared files.
int main(){
    int* arr = malloc(sizeof(int) * arrSize);

    for(int i = 0; i < arrSize; i++){
        arr[i] = i+1;
    }

    int sum = 0;
    int slice = arrSize/processCnt;

    int rc = fork();
    assert(rc >= 0);

    if(rc == 0){// Child process
        // This process (The Child) should work on half of the array, it will create a child process and divide the data between them.
        int rc2 = fork();
        assert(rc2>=0);

        int halfSlice = slice/2;

        if(rc2 == 0){// child of child
            for(int i = halfSlice; i < slice;i++){
                sum += arr[i];
            }

            // Write child of child's sum to a file.
            FILE *fp = fopen("maxNum1.txt", "w");
            fprintf(fp, "%d\n", sum);

        }else{ // child
            for(int i = 0; i < halfSlice;i++){
                sum += arr[i];
            }

            wait(NULL);// Make sure that my child finished.

            // Write chlild's sukm to a file.
            FILE *fp = fopen("maxNum2.txt", "w");
            fprintf(fp, "%d\n", sum);

        }
    }else{// Parent process

        int rc2 = fork();
        assert(rc2>=0);

        int halfSlice = slice/2;

        if(rc2 == 0){// child of parent
            for(int i = slice; i < slice + halfSlice; i++){
                sum += arr[i];
            }

            FILE *fp = fopen("maxNum3.txt", "w");
            fprintf(fp, "%d\n", sum);
        }
        else{ // parent
            for(int i = slice + halfSlice; i < arrSize;i++){
                sum += arr[i];
            }

            // Make sure that my two childs finished.
            wait(NULL);
            wait(NULL);

            int first, second, third;
            FILE *fp1 = fopen("maxNum1.txt", "r");
            fscanf(fp1, "%d\n", &first);
            fclose(fp1);

            FILE *fp2 = fopen("maxNum2.txt", "r");
            fscanf(fp2, "%d\n", &second);
            fclose(fp2);

            FILE *fp3 = fopen("maxNum3.txt", "r");
            fscanf(fp3, "%d\n", &third);
            fclose(fp3);

            sum += first + second + third;
            printf("Sum of the array: %d\n", sum);
        }
    }

    return 0;
}


