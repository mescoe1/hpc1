#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define MAX_RANDOM 10000

void sequentialMergeSort(int arr[], int left, int right);
void merge(int arr[], int left, int mid, int right);
void openmpMergeSort(int arr[], int left, int right);
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

// Sequential merge sort
void sequentialMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Merge function for merge sort
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Parallel merge sort using OpenMP
void openmpMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            openmpMergeSort(arr, left, mid);
            #pragma omp section
            openmpMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
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

    // Sequential Merge Sort
    struct timeval start, end;
    gettimeofday(&start, NULL);
    sequentialMergeSort(arr_sequential, 0, size - 1);
    gettimeofday(&end, NULL);
    double sequential_time = (end.tv_sec - start.tv_sec) * 1000.0; // sec to ms
    sequential_time += (end.tv_usec - start.tv_usec) / 1000.0;      // us to ms

    // OpenMP Merge Sort
    gettimeofday(&start, NULL);
    openmpMergeSort(arr_openmp, 0, size - 1);
    gettimeofday(&end, NULL);
    double openmp_time = (end.tv_sec - start.tv_sec) * 1000.0; // sec to ms
    openmp_time += (end.tv_usec - start.tv_usec) / 1000.0;      // us to ms

    if (choice == 2) {
        printf("\nSorted Array:\n");
        printArray(arr_openmp, size);
    }

    printf("\nSequential Merge Sort Time: %.3f milliseconds\n", sequential_time);
    printf("OpenMP Merge Sort Time: %.3f milliseconds\n", openmp_time);

    return 0;
}
