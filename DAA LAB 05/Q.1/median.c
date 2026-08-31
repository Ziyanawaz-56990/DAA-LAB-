/* =========================================================================
   DAA LAB - 5   |   Program 1
   Problem : Find the MEDIAN of a list of N numbers WITHOUT sorting the list.
   Method  : Randomized Quickselect (order-statistic selection)

   Idea:
   - The median is just the (n/2)-th smallest element (for odd n) or the
     average of the (n/2 - 1)-th and (n/2)-th smallest elements (for even n).
   - Quickselect finds the k-th smallest element in an unsorted array using
     the same partitioning idea as Quicksort, but it recurses into only ONE
     side of the partition instead of both -> the array never gets fully
     sorted.
   ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Swap two integers */
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* Lomuto partition scheme using a RANDOM pivot (avoids worst case on
   already-sorted / adversarial input) */
int partition(int arr[], int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(&arr[randomIndex], &arr[high]);   /* move random pivot to the end */

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;   /* final position of pivot */
}

/* Returns the k-th smallest element (0-indexed) of arr[low..high].
   Recurses into only the half that can contain the k-th element. */
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

double findMedian(int arr[], int n) {
    if (n % 2 == 1) {
        /* single middle element, 0-indexed position n/2 */
        return (double) quickSelect(arr, 0, n - 1, n / 2);
    } else {
        /* average of the two middle elements */
        int left  = quickSelect(arr, 0, n - 1, n / 2 - 1);
        /* NOTE: array got partially rearranged above; that's fine because
           quickSelect only needs an unsorted array as input, not the
           original order. We select the second middle element next. */
        int right = quickSelect(arr, 0, n - 1, n / 2);
        return (left + right) / 2.0;
    }
}

int main() {
    int n;
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

    /* keep an untouched copy just to display the original list */
    int *original = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) original[i] = arr[i];

    double median = findMedian(arr, n);

    printf("\nOriginal list (unsorted, as entered): ");
    for (int i = 0; i < n; i++) printf("%d ", original[i]);

    printf("\nMedian = %.2f\n", median);

    free(arr);
    free(original);
    return 0;
}

/* =========================================================================
   COMPLEXITY ANALYSIS  (see algorithm.md for full derivation)

   Let T(n) = expected time for quickSelect on an array of size n.

   Partitioning around a random pivot costs O(n).
   With a random pivot, the expected size of the recursive sub-problem
   shrinks geometrically, giving the recurrence (expected case):
        T(n) = T(n/2) + O(n)   =>   T(n) = O(n)      [Master Theorem, case 3
                                                       loosely / geometric
                                                       series argument]

   Best / Average case : O(n)
   Worst case           : O(n^2)   (pivot is always the min/max element,
                                     e.g., adversarial input - extremely
                                     unlikely with random pivoting)
   Space complexity      : O(1) extra (in-place) + O(n) recursion stack
                            in the worst case, O(log n) expected.
   ========================================================================= */
