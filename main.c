#include <stdio.h>
#include <stdlib.h>

int partition(int left, int right, int pivot, int* A){
    int partitionIndex = left;
    for (int i=left; i<=right; i++){
        if (A[i] < A[pivot]){
            partitionIndex++;
        }
    }
    int temp = A[pivot];
    A[pivot] = A[partitionIndex];
    A[partitionIndex] = temp;
    for (int i=left; i<partitionIndex; i++){
        if (A[i] > A[partitionIndex]){
            for (int j=partitionIndex+1; j<=right; j++){
                if (A[j] <= A[partitionIndex]){
                    int temp = A[i];
                    A[i] = A[j];
                    A[j] = temp;
                }
            }
        }
    }
    return partitionIndex;
}

void quicksort(int left, int right, int* A) {
    if (left<right){
        int pivot = (left + right) / 2;
        int partitionIndex=partition(left, right, pivot, A);
        quicksort(left, partitionIndex - 1, A);
        quicksort(partitionIndex + 1, right, A);
    }
}

int main() {


    return 0;
}