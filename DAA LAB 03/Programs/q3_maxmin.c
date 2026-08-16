/*
 * Q3: Find Maximum and Minimum using Divide and Conquer
 *
 * Elements are divided into pairs (the natural D&C split at the leaf
 * level): each pair is "conquered" with a single comparison to find its
 * local larger/smaller element, then these local winners are "combined"
 * by comparing against the running max/min. This guarantees the number
 * of comparisons never exceeds ceil(3n/2) - 2, for ANY n (not just
 * powers of two).
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int max;
    int min;
} Pair;

Pair maxMin(int arr[], int n, long *comparisons) {
    Pair result;
    int i;

    if (n == 1) {
        result.max = result.min = arr[0];
        return result;
    }

    if (n % 2 == 0) {
        (*comparisons)++;
        if (arr[0] > arr[1]) { result.max = arr[0]; result.min = arr[1]; }
        else                 { result.max = arr[1]; result.min = arr[0]; }
        i = 2;
    } else {
        result.max = result.min = arr[0];
        i = 1;
    }

    while (i < n - 1) {
        int larger, smaller;
        (*comparisons)++;
        if (arr[i] > arr[i + 1]) { larger = arr[i]; smaller = arr[i + 1]; }
        else                     { larger = arr[i + 1]; smaller = arr[i]; }

        (*comparisons)++;
        if (larger > result.max) result.max = larger;

        (*comparisons)++;
        if (smaller < result.min) result.min = smaller;

        i += 2;
    }
    return result;
}

void bruteForce(int arr[], int n, int *mn, int *mx) {
    *mn = *mx = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < *mn) *mn = arr[i];
        if (arr[i] > *mx) *mx = arr[i];
    }
}

int main(void) {
    srand(42);
    int sizes[] = {1, 2, 3, 6, 10, 99, 100, 1000, 9999, 10000, 100000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *arr = malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) arr[i] = rand() % 1000000;

        long comparisons = 0;
        Pair p = maxMin(arr, n, &comparisons);

        int bfMin, bfMax;
        bruteForce(arr, n, &bfMin, &bfMax);

        long bound = (3L * n + 1) / 2 - 2; /* ceil(3n/2) - 2 */
        if (bound < 0) bound = 0;

        printf("n = %-7d max=%-8d min=%-8d | comparisons=%-6ld | bound(ceil(3n/2)-2)=%-6ld | %s\n",
               n, p.max, p.min, comparisons, bound,
               (comparisons <= bound) ? "OK (within bound)" : "EXCEEDS BOUND");

        if (p.max != bfMax || p.min != bfMin)
            printf("   ERROR: mismatch with brute force! bfMax=%d bfMin=%d\n", bfMax, bfMin);

        free(arr);
    }
    return 0;
}
