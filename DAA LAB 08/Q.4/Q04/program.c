/*
 * DAA Lab 08 - Q4: Longest Increasing Subsequence (strictly increasing)
 *
 * INPUT FORMAT (standard input):
 *     n            -> number of elements (n >= 1)
 *     a0 a1 ... an-1
 * OUTPUT: length of the LIS (and one LIS).
 *
 * Main algorithm - O(n^2) dynamic programming:
 *   lis[i] = length of the longest strictly increasing subsequence ENDING at a[i]
 *   lis[i] = 1 + max( lis[j] ) for all j < i with a[j] < a[i]  (or 1 if none)
 *   answer = max over i of lis[i]
 *
 * As an independent check we also compute the length using the faster
 * O(n log n) "tails + binary search" method and compare the two answers.
 */
#include <stdio.h>
#include <stdlib.h>

/* O(n^2) DP. Fills lis[] and prev[] (for reconstruction). Returns the index
 * where the best subsequence ends. */
int lisQuadratic(const int a[], int n, int lis[], int prev[])
{
    int i, j, bestEnd = 0;
    for (i = 0; i < n; i++) {
        lis[i] = 1;
        prev[i] = -1;
        for (j = 0; j < i; j++) {
            if (a[j] < a[i] && lis[j] + 1 > lis[i]) {
                lis[i] = lis[j] + 1;
                prev[i] = j;
            }
        }
        if (lis[i] > lis[bestEnd]) bestEnd = i;
    }
    return bestEnd;
}

/* O(n log n): tails[k] = smallest possible last element of an increasing
 * subsequence of length k+1. Binary search finds where a[i] goes. */
int lisFast(const int a[], int n)
{
    int *tails = (int *)malloc(n * sizeof(int));
    int size = 0, i;
    if (tails == NULL) return -1;
    for (i = 0; i < n; i++) {
        int low = 0, high = size;           /* first position with tails >= a[i] */
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (tails[mid] < a[i]) low = mid + 1;
            else high = mid;
        }
        tails[low] = a[i];
        if (low == size) size++;
    }
    free(tails);
    return size;
}

int main(void)
{
    int n, i, bestEnd, length, fast;
    int *a, *lis, *prev, *seq;

    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid n.\n");
        return 1;
    }
    a = (int *)malloc(n * sizeof(int));
    lis = (int *)malloc(n * sizeof(int));
    prev = (int *)malloc(n * sizeof(int));
    if (a == NULL || lis == NULL || prev == NULL) return 1;
    for (i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1) { printf("Invalid input.\n"); return 1; }
    }

    bestEnd = lisQuadratic(a, n, lis, prev);
    length = lis[bestEnd];
    fast = lisFast(a, n);

    printf("Length of LIS (O(n^2) DP)        = %d\n", length);
    printf("Length of LIS (O(n log n) check) = %d  -> %s\n", fast,
           (fast == length) ? "both methods agree" : "MISMATCH");

    seq = (int *)malloc(length * sizeof(int));
    if (seq != NULL) {
        int k = length - 1, idx = bestEnd;
        while (idx != -1) { seq[k--] = a[idx]; idx = prev[idx]; }
        printf("One LIS: ");
        for (i = 0; i < length; i++) printf("%d ", seq[i]);
        printf("\n");
        free(seq);
    }
    free(a); free(lis); free(prev);
    return 0;
}
