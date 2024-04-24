#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define MAX_RANDOM 10000

void sequentialBubbleSort(int arr[], int n);
void openmpBubbleSort(int arr[], int n);
void swap(int *a, int *b);
void printArray(int arr[], int n);
void fillRandomArray(int arr[], int n);
void takeInputArray(int arr[], int n);

// Utility function to swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Sequential bubble sort
void sequentialBubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

// Parallel bubble sort using OpenMP with odd-even transposition
void openmpBubbleSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        if (i % 2 == 0) {
            #pragma omp parallel for private(j) shared(arr)
            for (j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1])
                    swap(&arr[j], &arr[j + 1]);
            }
        } else {
            #pragma omp parallel for private(j) shared(arr)
            for (j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1])
                    swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Function to print array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Function to fill array with random numbers
void fillRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % MAX_RANDOM;
}

// Function to take array input from user
void takeInputArray(int arr[], int n) {
    printf("Enter the elements of the array:\n");
    for (int i = 0; i < n; i++) {
        printf("Element %d: ", i + 1);
        scanf("%d", &arr[i]);
    }
}

int main() {
    int choice;
    int size = 0;

    printf("Do you want to generate a random array or manually input array elements?\n");
    printf("1. Generate a random array\n");
    printf("2. Manually input array elements\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    int arr_sequential[MAX_RANDOM];
    int arr_openmp[MAX_RANDOM];

    if (choice == 1) {
        size = MAX_RANDOM;
        // Fill arrays with random numbers
        fillRandomArray(arr_sequential, size);
        for (int i = 0; i < size; i++)
            arr_openmp[i] = arr_sequential[i];
    } else if (choice == 2) {
        printf("Enter the size of the array: ");
        scanf("%d", &size);
        if (size <= 0 || size > MAX_RANDOM) {
            printf("Invalid size. Exiting...\n");
            return 1;
        }
        takeInputArray(arr_sequential, size);
        printf("\nOriginal Array:\n");
        printArray(arr_sequential, size);
        for (int i = 0; i < size; i++)
            arr_openmp[i] = arr_sequential[i];
    } else {
        printf("Invalid choice. Exiting...\n");
        return 1;
    }

    // Sequential Bubble Sort
    struct timeval start, end;
    gettimeofday(&start, NULL);
    sequentialBubbleSort(arr_sequential, size);
    gettimeofday(&end, NULL);
    double sequential_time = (end.tv_sec - start.tv_sec) * 1000.0; // sec to ms
    sequential_time += (end.tv_usec - start.tv_usec) / 1000.0;      // us to ms

    // OpenMP Bubble Sort
    gettimeofday(&start, NULL);
    openmpBubbleSort(arr_openmp, size);
    gettimeofday(&end, NULL);
    double openmp_time = (end.tv_sec - start.tv_sec) * 1000.0; // sec to ms
    openmp_time += (end.tv_usec - start.tv_usec) / 1000.0;      // us to ms

    if (choice == 2) {
        printf("\nSorted Array:\n");
        printArray(arr_openmp, size);
    }

    printf("\nSequential Bubble Sort Time: %.3f milliseconds\n", sequential_time);
    printf("OpenMP Bubble Sort Time: %.3f milliseconds\n", openmp_time);

    return 0;
}
