/* =========================================================================
   DAA LAB - 5   |   Program 2
   Problem : Find the K'th SMALLEST element in a list of N numbers
             WITHOUT sorting the list.
   Method  : Randomized Quickselect (order-statistic selection)

   Idea:
   - Same partitioning strategy as Quicksort (Lomuto partition with a
     RANDOM pivot to avoid worst-case behaviour on sorted/adversarial data).
   - After one partition step, the pivot lands at its correct sorted
     position 'p'. If p == k-1 (0-indexed) we are done.
     Otherwise we recurse ONLY into the half that must contain the
     k-th smallest element - the other half is thrown away, which is
     exactly why the whole array never needs to be fully sorted.
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

/* Returns the k-th smallest element (0-indexed position 'k') */
int quickSelect(int arr[], int low, int high, int k) {
    if (low == high) return arr[low];

    int pivotIndex = partition(arr, low, high);

    if (k == pivotIndex)
        return arr[k];
    else if (k < pivotIndex)
        return quickSelect(arr, low, pivotIndex - 1, k);
    else
        return quickSelect(arr, pivotIndex + 1, high, k);
}

int main() {
    int n, k;
    srand((unsigned int) time(NULL));

    printf("Enter number of elements (N): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid N.\n");
        return 1;
    }

    int *arr = (int *) malloc(n * sizeof(int));
    if (!arr) { printf("Memory allocation failed.\n"); return 1; }

    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    printf("Enter K (1-indexed, 1 <= K <= %d): ", n);
    scanf("%d", &k);

    if (k < 1 || k > n) {
        printf("Invalid K.\n");
        free(arr);
        return 1;
    }

    int *original = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) original[i] = arr[i];

    int result = quickSelect(arr, 0, n - 1, k - 1);   /* convert to 0-index */

    printf("\nOriginal list (unsorted, as entered): ");
    for (int i = 0; i < n; i++) printf("%d ", original[i]);

    printf("\nThe %d%s smallest element is: %d\n", k,
           (k % 10 == 1 && k != 11) ? "st" :
           (k % 10 == 2 && k != 12) ? "nd" :
           (k % 10 == 3 && k != 13) ? "rd" : "th",
           result);

    free(arr);
    free(original);
    return 0;
}

/* =========================================================================
   COMPLEXITY ANALYSIS  (see algorithm.md for full derivation)

   Best / Average case : O(n)     -- random pivot halves the search range
                                      in expectation each recursive call:
                                      T(n) = T(n/2) + O(n)  =>  O(n)
   Worst case           : O(n^2)  -- pivot always the smallest/largest
                                      remaining element (rare with random
                                      pivoting; only likely with a naive
                                      fixed-pivot choice on sorted data)
   Space complexity      : O(1) extra (in-place partitioning)
                            + O(log n) expected recursion stack depth
                              (O(n) worst case)
   ========================================================================= */
