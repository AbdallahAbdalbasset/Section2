#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

const int arrSize = 1e8;
const int processCnt = 2;

// Get the maximum number of an array using two processes and a shared file.
int main(){
    int* arr = malloc(sizeof(int) * arrSize);

    srand(time(NULL));
    for(int i = 1; i< arrSize; i++){
        arr[i] = rand() % 10000;
    }
    arr[0] = 1e7;// Our maximum number - for testing

    int maxNum = 0;
    int slice = arrSize/processCnt;

    int rc = fork();
    assert(rc >= 0);

    if(rc == 0){// Child process
        for(int i = slice; i < arrSize;i++){
            if(arr[i] > maxNum){
                maxNum = arr[i];
            }
        }

        // Write child's local maximum to a file.
        FILE *fp = fopen("maxNum.txt", "w");
        fprintf(fp, "%d\n", maxNum);

    }else{// Parent process
        for(int i = 0; i < slice;i++){
            if(arr[i] > maxNum){
                maxNum = arr[i];
            }
        }
        wait(NULL); // Make sure that child saves it's local maximum in the file.

        // Read the child's local maximum value and compare it with the parent's local maximum.
        FILE* fp = fopen("maxNum.txt", "r");
        int childMax;
        fscanf(fp, "%d", &childMax);
        printf("The maximum number is: %d\n", childMax > maxNum? childMax : maxNum);

    }

    return 0;
}