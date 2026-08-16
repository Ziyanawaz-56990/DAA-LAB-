/*
 * Q1: Binary vs Ternary Search
 * Search for element x in a sorted array of size n using both
 * Binary Search and Ternary Search, counting comparisons made by each,
 * and validate that Binary Search does fewer comparisons on average.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ---------- Binary Search ---------- */
int binarySearch(int arr[], int n, int x, long *comparisons) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        (*comparisons)++;
        if (arr[mid] == x) return mid;
        (*comparisons)++;
        if (arr[mid] < x) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

/* ---------- Ternary Search ---------- */
int ternarySearch(int arr[], int n, int x, long *comparisons) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid1 = low + (high - low) / 3;
        int mid2 = high - (high - low) / 3;

        (*comparisons)++;
        if (arr[mid1] == x) return mid1;
        (*comparisons)++;
        if (arr[mid2] == x) return mid2;

        (*comparisons)++;
        if (x < arr[mid1]) {
            high = mid1 - 1;
        } else {
            (*comparisons)++;
            if (x > arr[mid2]) {
                low = mid2 + 1;
            } else {
                low = mid1 + 1;
                high = mid2 - 1;
            }
        }
    }
    return -1;
}

int main(void) {
    int n = 100000;
    int *arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) arr[i] = 2 * i; /* sorted, even numbers */

    int trials[] = {0, n / 4, n / 2, (3 * n) / 4, n - 1};
    int numTrials = sizeof(trials) / sizeof(trials[0]);

    long totalBinary = 0, totalTernary = 0;

    printf("Array size n = %d\n\n", n);
    printf("%-12s %-15s %-18s\n", "Target idx", "Binary comps", "Ternary comps");
    for (int t = 0; t < numTrials; t++) {
        int idx = trials[t];
        int target = arr[idx];

        long cb = 0, ct = 0;
        int rb = binarySearch(arr, n, target, &cb);
        int rt = ternarySearch(arr, n, target, &ct);

        printf("%-12d %-15ld %-18ld  (found at %d / %d)\n", idx, cb, ct, rb, rt);
        totalBinary += cb;
        totalTernary += ct;
    }

    /* Also test a not-found case */
    long cb = 0, ct = 0;
    int rb = binarySearch(arr, n, 2 * n + 5, &cb);   /* not present */
    int rt = ternarySearch(arr, n, 2 * n + 5, &ct);
    printf("\nSearching absent element:\n");
    printf("Binary: comparisons = %ld, result = %d\n", cb, rb);
    printf("Ternary: comparisons = %ld, result = %d\n", ct, rt);

    printf("\nTotal comparisons over %d found-trials: Binary = %ld, Ternary = %ld\n",
           numTrials, totalBinary, totalTernary);

    printf("\nWorst-case comparison bound: Binary ~ 2*log2(n) = %.2f, "
           "Ternary ~ 4*log3(n) = %.2f\n",
           2 * (log(n) / log(2)), 4 * (log(n) / log(3)));

    if (totalBinary < totalTernary)
        printf("=> Binary search used FEWER comparisons overall -> Binary search is better.\n");
    else
        printf("=> Ternary search used fewer/equal comparisons in this run.\n");

    free(arr);
    return 0;
}
