/*
 * Q6: Use of loop invariants in sorting (Selection Sort)
 *
 * PSEUDOCODE (1-indexed, matching the question's A[1..n]):
 *   SELECTION-SORT(A, n)
 *     for i = 1 to n - 1
 *         min_idx = i
 *         for j = i + 1 to n
 *             if A[j] < A[min_idx]
 *                 min_idx = j
 *         exchange A[i] with A[min_idx]
 *
 * LOOP INVARIANT (for the outer loop, at the start of each iteration i):
 *   The subarray A[1 .. i-1] consists of the i-1 smallest elements of the
 *   original array, stored in final sorted (non-decreasing) order.
 *
 *   - Initialization: before the first iteration (i = 1), A[1..0] is
 *     empty, so the invariant holds trivially.
 *   - Maintenance: at the start of iteration i, A[1..i-1] is sorted and
 *     holds the i-1 smallest elements. The inner loop finds the minimum
 *     of A[i..n] and swaps it into position i. Now A[1..i] holds the i
 *     smallest elements in sorted order, so the invariant holds for i+1.
 *   - Termination: the loop ends when i = n. By the invariant,
 *     A[1..n-1] holds the n-1 smallest elements in sorted order. Since
 *     A[n] must then be the single largest remaining element, the whole
 *     array A[1..n] is sorted.
 *
 * WHY ONLY THE FIRST (n-1) ELEMENTS?
 *   After placing the n-1 smallest elements correctly in A[1..n-1], the
 *   single element left at A[n] is automatically the largest and is
 *   already in its correct final position -- there is nothing left to
 *   compare it against, so a final pass for i = n would be a no-op.
 *
 * RUNNING TIME:
 *   Worst case: Theta(n^2)  (always scans the remaining suffix)
 *   Best case:  Theta(n^2) as well -- unlike insertion sort, selection
 *   sort ALWAYS performs the same number of comparisons regardless of
 *   input order (it must scan the rest of the array each time to find
 *   the minimum), so the best case is NOT any better; only the number
 *   of swaps can differ (at most n-1 swaps).
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long comparisons = 0;
long swaps = 0;

void selectionSort(int A[], int n) {
    /* 0-indexed version of A[1..n]; loop i from 0 to n-2 (i.e. first n-1 elements) */
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (A[j] < A[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            int tmp = A[i]; A[i] = A[minIdx]; A[minIdx] = tmp;
            swaps++;
        }
    }
}

void printArray(int A[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\n");
}

int isSorted(int A[], int n) {
    for (int i = 1; i < n; i++)
        if (A[i - 1] > A[i]) return 0;
    return 1;
}

int main(void) {
    /* Demonstrate the loop invariant explicitly, step by step */
    int demo[] = {29, 10, 14, 37, 13};
    int n = sizeof(demo) / sizeof(demo[0]);

    printf("Initial array: ");
    printArray(demo, n);
    printf("\nStep-by-step (showing the invariant A[0..i-1] sorted before each pass):\n");

    for (int i = 0; i < n - 1; i++) {
        printf("Before pass i=%d, sorted prefix A[0..%d] = ", i, i - 1);
        for (int p = 0; p < i; p++) printf("%d ", demo[p]);
        printf("\n");

        int minIdx = i;
        for (int j = i + 1; j < n; j++)
            if (demo[j] < demo[minIdx]) minIdx = j;
        int tmp = demo[i]; demo[i] = demo[minIdx]; demo[minIdx] = tmp;

        printf("  -> found min in A[%d..%d] at index %d, swapped with A[%d]\n", i, n - 1, minIdx, i);
        printf("  -> array now: ");
        printArray(demo, n);
        printf("\n");
    }
    printf("After loop ends (i reached n-1=%d), full array is sorted:\n", n - 1);
    printArray(demo, n);

    /* Empirical validation on random data, worst case (descending) and best case (ascending) */
    printf("\n--- Empirical validation ---\n");
    int sizes[] = {10, 100, 1000, 5000};
    for (int s = 0; s < 4; s++) {
        int sz = sizes[s];

        /* Random */
        int *rnd = malloc(sizeof(int) * sz);
        srand(1);
        for (int i = 0; i < sz; i++) rnd[i] = rand() % 100000;
        comparisons = 0; swaps = 0;
        selectionSort(rnd, sz);
        printf("n=%-6d Random input:     comparisons=%-8ld swaps=%-6ld sorted=%s\n",
               sz, comparisons, swaps, isSorted(rnd, sz) ? "yes" : "NO");
        free(rnd);

        /* Best case: already sorted ascending */
        int *best = malloc(sizeof(int) * sz);
        for (int i = 0; i < sz; i++) best[i] = i;
        comparisons = 0; swaps = 0;
        selectionSort(best, sz);
        printf("n=%-6d Already sorted:   comparisons=%-8ld swaps=%-6ld sorted=%s\n",
               sz, comparisons, swaps, isSorted(best, sz) ? "yes" : "NO");
        free(best);

        /* Worst case: reverse sorted */
        int *worst = malloc(sizeof(int) * sz);
        for (int i = 0; i < sz; i++) worst[i] = sz - i;
        comparisons = 0; swaps = 0;
        selectionSort(worst, sz);
        printf("n=%-6d Reverse sorted:   comparisons=%-8ld swaps=%-6ld sorted=%s\n",
               sz, comparisons, swaps, isSorted(worst, sz) ? "yes" : "NO");
        free(worst);

        double expected = (double) sz * (sz - 1) / 2.0;
        printf("            Expected comparisons n(n-1)/2 = %.0f  "
               "(same for ALL inputs -> confirms Theta(n^2) in both best & worst case)\n\n",
               expected);
    }

    return 0;
}
