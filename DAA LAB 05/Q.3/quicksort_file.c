/* =========================================================================
   DAA LAB - 5   |   Program 3
   Problem : Implement QUICK SORT of N random elements stored in a file.

   Flow:
   1. Generate N random integers and write them to "input.txt".
   2. Read the N integers back from "input.txt" into an array.
   3. Sort the array in place using Quicksort (random pivot, Lomuto
      partition).
   4. Write the sorted array to "output.txt" (and print it on screen).
   ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(&arr[randomIndex], &arr[high]);

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quickSort(arr, low, p - 1);
        quickSort(arr, p + 1, high);
    }
}

/* Step 1: create a file of N random integers */
void generateRandomFile(const char *filename, int n, int range) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { printf("Could not open %s for writing.\n", filename); exit(1); }

    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", rand() % range);

    fclose(fp);
}

/* Step 2: read the N integers back from the file */
int *readFromFile(const char *filename, int *n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { printf("Could not open %s for reading.\n", filename); exit(1); }

    fscanf(fp, "%d", n);
    int *arr = (int *) malloc((*n) * sizeof(int));
    for (int i = 0; i < *n; i++) fscanf(fp, "%d", &arr[i]);

    fclose(fp);
    return arr;
}

/* Step 4: write the sorted array to a file */
void writeToFile(const char *filename, int arr[], int n) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { printf("Could not open %s for writing.\n", filename); exit(1); }

    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", arr[i]);

    fclose(fp);
}

int main() {
    int n;
    srand((unsigned int) time(NULL));

    printf("Enter number of random elements (N): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid N.\n");
        return 1;
    }

    /* 1. generate N random numbers into input.txt */
    generateRandomFile("input.txt", n, 1000);
    printf("Generated %d random numbers -> input.txt\n", n);

    /* 2. read them back */
    int count;
    int *arr = readFromFile("input.txt", &count);

    printf("Unsorted data (read from input.txt):\n");
    for (int i = 0; i < count; i++) printf("%d ", arr[i]);
    printf("\n");

    /* 3. quicksort */
    quickSort(arr, 0, count - 1);

    /* 4. write sorted result to output.txt */
    writeToFile("output.txt", arr, count);

    printf("Sorted data (also written to output.txt):\n");
    for (int i = 0; i < count; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
    return 0;
}

/* =========================================================================
   COMPLEXITY ANALYSIS  (see algorithm.md for full derivation)

   Best / Average case : O(n log n)
   Worst case           : O(n^2)   (random pivot makes this astronomically
                                     unlikely regardless of input pattern)
   Space complexity      : O(log n) expected recursion stack (in-place sort)
   File I/O               : O(n) to generate + O(n) to read + O(n) to write
   ========================================================================= */
