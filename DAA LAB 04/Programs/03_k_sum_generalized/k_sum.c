/* ============================================================================
 * Application of Sorting - III
 * ----------------------------------------------------------------------------
 * Problem : Given a set S of n integers and an integer T, test whether some
 *           k of the integers in S add up to T.
 *
 * Idea    : 1. Sort S                                             -> O(n log n)
 *           2. Fix the first (k-1) elements using (k-1) nested
 *              STRICTLY INCREASING index loops (implemented here
 *              as recursion of depth k-1)                          -> O(n^(k-1)) combinations
 *           3. For every such fixed combination, binary-search the
 *              sorted array for the single remaining value
 *                    need = T - (sum of the fixed k-1 elements)     -> O(log n)
 *
 *           Total combinations O(n^(k-1)) each costing O(log n) for
 *           the binary search closing step.
 *
 * Complexity : TIME  = O(n^(k-1) * log n)
 *              SPACE = O(k) extra (recursion depth / index stack)
 *
 * Note: This is the classical textbook generalisation of two-sum /
 *       three-sum to k-sum using sorting + binary search.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int cmp_int(const void *a, const void *b) {
    return (*(const int *)a - *(const int *)b);
}

/* Binary search for 'target' in sorted arr[0..n-1]. O(log n). */
bool binary_search(int arr[], int n, int target) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return true;
        else if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

int n_global;
int *arr_global;
int k_global;
long T_global;
int chosen[64];          /* indices chosen so far (k-1 of them) */
bool solution_found = false;
int solution_indices[64];

/* Recursively fix (k-1) elements at strictly increasing indices,
 * then binary-search for the value that completes the sum to T. */
void fix_elements(int start_idx, int depth, long partial_sum) {
    if (solution_found) return;

    if (depth == k_global - 1) {
        long need = T_global - partial_sum;
        /* binary search among indices NOT already chosen is the strict
         * version; for clarity/standard textbook treatment we search the
         * whole sorted array (elements assumed distinct enough that the
         * match is not one of the already-chosen positions in practice). */
        int lo = 0, hi = n_global - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (arr_global[mid] == need) {
                /* verify it's not reusing an already chosen index */
                bool reused = false;
                for (int t = 0; t < depth; t++)
                    if (chosen[t] == mid) { reused = true; break; }
                if (!reused) {
                    solution_found = true;
                    for (int t = 0; t < depth; t++) solution_indices[t] = chosen[t];
                    solution_indices[depth] = mid;
                }
                return;
            } else if (arr_global[mid] < need) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return;
    }

    for (int i = start_idx; i < n_global && !solution_found; i++) {
        chosen[depth] = i;
        fix_elements(i + 1, depth + 1, partial_sum + arr_global[i]);
    }
}

/* returns true if k elements of S sum to T; fills idx[0..k-1] with the
 * indices (into the sorted array) that form the solution. */
bool k_sum_exists(int S[], int n, int k, long T, int idx_out[]) {
    int *sorted = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) sorted[i] = S[i];
    qsort(sorted, n, sizeof(int), cmp_int);      /* O(n log n) */

    n_global = n;
    arr_global = sorted;
    k_global = k;
    T_global = T;
    solution_found = false;

    fix_elements(0, 0, 0);                        /* O(n^(k-1) log n) */

    bool result = solution_found;
    if (result)
        for (int i = 0; i < k; i++) idx_out[i] = solution_indices[i];

    free(sorted);
    return result;
}

/* Brute-force O(2^n) subset-sum-of-exactly-k-elements check, used only to
 * VALIDATE the fast algorithm's yes/no answer on small inputs. */
static bool brute_force_k_sum(int S[], int n, int k, long T) {
    for (long mask = 0; mask < (1L << n); mask++) {
        int popcount = 0;
        long sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1L << i)) { popcount++; sum += S[i]; }
        }
        if (popcount == k && sum == T) return true;
    }
    return false;
}

int main(void) {
    int S[] = {2, 7, 4, 11, 15, -3, 9, 1};
    int n = sizeof(S) / sizeof(S[0]);

    struct { int k; long T; } tests[] = {
        {2, 12},
        {3, 20},
        {4, 24},
        {2, 100},
        {5, 26},
        {6, 41},
    };
    int ntests = sizeof(tests) / sizeof(tests[0]);

    printf("S = { ");
    for (int i = 0; i < n; i++) printf("%d ", S[i]);
    printf("}   (n = %d)\n", n);
    printf("---------------------------------------------------------------------\n");
    printf("%-5s %-8s %-38s %-6s\n", "k", "T", "Result", "Check");
    printf("---------------------------------------------------------------------\n");

    for (int t = 0; t < ntests; t++) {
        int k = tests[t].k;
        long T = tests[t].T;
        int idx[64];

        int *sorted = (int *)malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) sorted[i] = S[i];
        qsort(sorted, n, sizeof(int), cmp_int);

        bool found        = k_sum_exists(S, n, k, T, idx);
        bool ground_truth  = brute_force_k_sum(S, n, k, T);
        const char *check  = (found == ground_truth) ? "PASS" : "FAIL";

        char result_str[64];
        if (found) {
            int pos = 0;
            pos += snprintf(result_str + pos, sizeof(result_str) - pos, "{ ");
            for (int i = 0; i < k; i++)
                pos += snprintf(result_str + pos, sizeof(result_str) - pos, "%d ", sorted[idx[i]]);
            snprintf(result_str + pos, sizeof(result_str) - pos, "}");
        } else {
            snprintf(result_str, sizeof(result_str), "no such combination");
        }

        printf("%-5d %-8ld %-38s %-6s\n", k, T, result_str, check);
        free(sorted);
    }

    printf("---------------------------------------------------------------------\n");
    printf("(Each 'Check' column cross-validates against a brute-force O(2^n)\n");
    printf(" subset search that tries every combination of k elements.)\n\n");
    printf("Time Complexity : O(n^(k-1) * log n)\n");
    printf("Space Complexity: O(k) extra (+ O(n) for sorted copy)\n");
    return 0;
}
