/* ============================================================================
 * Application of Sorting - II
 * ----------------------------------------------------------------------------
 * Problem : Given two sets S1 and S2 (each of size n) and a number x,
 *           determine whether there exists a1 in S1 and a2 in S2 such that
 *           a1 + a2 == x.
 *
 * Idea    : 1. Sort S2                                   -> O(n log n)
 *           2. Sort S1 as well and use the two-pointer
 *              technique: i at the start of sorted S1,
 *              j at the end of sorted S2.
 *                 if S1[i] + S2[j] == x  -> found, stop
 *                 if S1[i] + S2[j] <  x  -> i++   (need a bigger sum)
 *                 if S1[i] + S2[j] >  x  -> j--   (need a smaller sum)
 *              This scans each array at most once        -> O(n)
 *
 * Complexity : TIME  = O(n log n)   (dominated by the two sorts)
 *              SPACE = O(1) extra   (sort in place, besides input arrays)
 *
 * (An equally valid O(n log n) alternative: sort only S2, then for every
 *  element of S1 binary-search for x - element in S2. Both are shown to be
 *  O(n log n); the two-pointer version below also demonstrates the pair.)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int cmp_int(const void *a, const void *b) {
    return (*(const int *)a - *(const int *)b);
}

/* ---------------------------------------------------------------------- *
 * find_pair_with_sum : returns true and fills *out1,*out2 if a pair       *
 *                       (one from S1, one from S2) sums to x              *
 * ---------------------------------------------------------------------- */
bool find_pair_with_sum(int S1[], int n1, int S2[], int n2, int x,
                         int *out1, int *out2) {
    qsort(S1, n1, sizeof(int), cmp_int);   /* O(n log n) */
    qsort(S2, n2, sizeof(int), cmp_int);   /* O(n log n) */

    int i = 0, j = n2 - 1;                  /* two-pointer scan: O(n) */
    while (i < n1 && j >= 0) {
        long sum = (long)S1[i] + (long)S2[j];
        if (sum == x) {
            *out1 = S1[i];
            *out2 = S2[j];
            return true;
        } else if (sum < x) {
            i++;
        } else {
            j--;
        }
    }
    return false;
}

/* Brute-force O(n^2) cross-check used only to VALIDATE the fast algorithm. */
static bool brute_force_check(int S1[], int n1, int S2[], int n2, int x) {
    for (int i = 0; i < n1; i++)
        for (int j = 0; j < n2; j++)
            if (S1[i] + S2[j] == x) return true;
    return false;
}

int main(void) {
    int S1[] = {8, 1, 9, 3, 15, 4};
    int S2[] = {20, 2, 7, 11, 5, 30};
    int n1 = sizeof(S1) / sizeof(S1[0]);
    int n2 = sizeof(S2) / sizeof(S2[0]);

    printf("S1 = { ");
    for (int i = 0; i < n1; i++) printf("%d ", S1[i]);
    printf("}\nS2 = { ");
    for (int i = 0; i < n2; i++) printf("%d ", S2[i]);
    printf("}\n");
    printf("-----------------------------------------------------------------\n");
    printf("%-6s %-10s %-38s %-6s\n", "x", "Result", "Pair (a in S1, b in S2)", "Check");
    printf("-----------------------------------------------------------------\n");

    int tests[] = {12, 45, 3, 50, -1, 0};
    int ntests = sizeof(tests) / sizeof(tests[0]);

    for (int t = 0; t < ntests; t++) {
        int a = 0, b = 0;
        int S1copy[16], S2copy[16];
        for (int i = 0; i < n1; i++) S1copy[i] = S1[i];
        for (int i = 0; i < n2; i++) S2copy[i] = S2[i];

        bool found      = find_pair_with_sum(S1copy, n1, S2copy, n2, tests[t], &a, &b);
        bool ground_truth = brute_force_check(S1, n1, S2, n2, tests[t]);
        const char *check = (found == ground_truth) ? "PASS" : "FAIL";

        char result_str[48];
        if (found)
            snprintf(result_str, sizeof(result_str), "(%d, %d) -> sum=%d", a, b, a + b);
        else
            snprintf(result_str, sizeof(result_str), "no such pair");

        printf("%-6d %-10s %-38s %-6s\n", tests[t], found ? "FOUND" : "NOT FOUND", result_str, check);
    }

    printf("-----------------------------------------------------------------\n");
    printf("(Each 'Check' column cross-validates the O(n log n) result against\n");
    printf(" a brute-force O(n^2) search over every possible pair.)\n\n");
    printf("Time Complexity : O(n log n)   [dominated by sorting S1 and S2]\n");
    printf("Space Complexity: O(1) extra\n");
    return 0;
}
