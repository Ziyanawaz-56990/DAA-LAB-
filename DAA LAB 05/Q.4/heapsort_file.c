/* =========================================================================
   DAA LAB - 5   |   Program 4
   Problem : Implement HEAP SORT to sort N randomly generated elements
             stored in a file.

   Flow:
   1. Generate N random integers and write them to "input.txt".
   2. Read the N integers back from "input.txt" into an array.
   3. Build a MAX-HEAP from the array, then repeatedly extract the max
      element to the end of the array (standard in-place Heap Sort).
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

/* Sift the node at index i down so the subtree rooted at i satisfies the
   max-heap property. 'heapSize' = number of elements currently in the heap. */
void heapify(int arr[], int heapSize, int i) {
    int largest = i;
    int left    = 2 * i + 1;
    int right   = 2 * i + 2;

    if (left  < heapSize && arr[left]  > arr[largest]) largest = left;
    if (right < heapSize && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, heapSize, largest);   /* continue sifting down */
    }
}

void heapSort(int arr[], int n) {
    /* Phase 1: Build max-heap - start from the last internal node
       and heapify every node up to the root. O(n) total. */
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    /* Phase 2: Repeatedly move the current max (root) to the end of the
       array and shrink the heap by one, then re-heapify the root. */
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

void generateRandomFile(const char *filename, int n, int range) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { printf("Could not open %s for writing.\n", filename); exit(1); }

    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", rand() % range);

    fclose(fp);
}

int *readFromFile(const char *filename, int *n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { printf("Could not open %s for reading.\n", filename); exit(1); }

    fscanf(fp, "%d", n);
    int *arr = (int *) malloc((*n) * sizeof(int));
    for (int i = 0; i < *n; i++) fscanf(fp, "%d", &arr[i]);

    fclose(fp);
    return arr;
}

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

    /* 3. heap sort */
    heapSort(arr, count);

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

   Build-heap phase : O(n)          (tighter bound than the naive O(n log n))
   Extract phase    : O(n log n)    (n extractions, each O(log n) heapify)
   Overall          : O(n log n)  in ALL cases - best, average, AND worst
                       (unlike Quicksort, Heap Sort has no bad input case)
   Space complexity  : O(1) extra  (fully in-place, iterative extraction)
   File I/O          : O(n) to generate + O(n) to read + O(n) to write
   ========================================================================= */
