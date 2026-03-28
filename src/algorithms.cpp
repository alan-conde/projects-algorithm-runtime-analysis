#include "algorithms.h"

// Swap Helper Method
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// --------------------
// Bubble Sort
// --------------------

void Algorithms::bubbleSort(int* arr, int n) {
    bool swapped;
    
    for(int i = 0; i < n-1; ++i) {
        swapped = false;
        for(int j = 0; j < n-i-1; ++j) {
            if(arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
                swapped = true;
            }
        }

        // If no elements were swapped, break
        if(!swapped) {
            break;
        }
    }
}

// --------------------
// Selection Sort
// --------------------

void Algorithms::selectionSort(int* arr, int n) {
    for(int i = 0; i < n-1; ++i) {
        int min_idx = i;
        for(int j = i+1; j < n; ++j) {
            if(arr[j] < arr[min_idx]) {
                min_idx = j;
            }

            if (min_idx != i) {
                int temp = arr[i];
                arr[i] = arr[min_idx];
                arr[min_idx] = temp;
            }
        }
    }
}

// --------------------
// Insertion Sort
// --------------------

void Algorithms::insertionSort(int* arr, int n) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}