/*
 * DAA Lab-02, Question 3: Merging k Sorted Arrays
 * ---------------------------------------------------
 * Given k sorted arrays of n elements each (kn elements total):
 *
 *   Method 1 (Sequential): merge array 1 with array 2, merge that result
 *   with array 3, ..., merge with array k. The i-th merge combines a run of
 *   length (i-1)n with a run of length n, costing Theta(i*n).
 *   Total: sum_{i=2}^{k} Theta(i*n) = Theta(n * k^2).
 *
 *   Method 2 (Balanced pairwise / tournament): merge the k arrays in pairs,
 *   then merge the results in pairs, and so on (exactly like the merge
 *   stage of merge sort, but starting from k sorted runs instead of single
 *   elements). There are Theta(log k) levels and each level does Theta(kn)
 *   total work.
 *   Total: Theta(n * k * log k).
 *
 * This program actually builds k random sorted arrays of n elements each,
 * runs BOTH real algorithms, counts true comparisons/writes, and verifies
 * that both produce the fully sorted kn-element output.
 *
 * Compile: gcc -O2 -Wall -o q3_k_sorted_arrays q3_k_sorted_arrays.c
 * Run:     ./q3_k_sorted_arrays
 */
#include <stdio.h>
#include <stdlib.h>

static long long comparisons, writes;

static int cmp_int(const void *a, const void *b) { return (*(const int *)a) - (*(const int *)b); }

/* standard 2-way merge of two sorted runs a[0..na-1] and b[0..nb-1] into out */
static void merge2(const int *a, int na, const int *b, int nb, int *out) {
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        comparisons++;
        out[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
        writes++;
    }
    while (i < na) { out[k++] = a[i++]; writes++; }
    while (j < nb) { out[k++] = b[j++]; writes++; }
}

/* ---------- Method 1: sequential merging ---------- */
static void merge_sequential(int **arrays, int k, int n, int *result) {
    int acc_len = n;
    int *acc = malloc((long)k * n * sizeof(int));
    for (int i = 0; i < n; i++) acc[i] = arrays[0][i];
    for (int idx = 1; idx < k; idx++) {
        int *tmp = malloc((long)(acc_len + n) * sizeof(int));
        merge2(acc, acc_len, arrays[idx], n, tmp);
        free(acc);
        acc = tmp;
        acc_len += n;
    }
    for (int i = 0; i < acc_len; i++) result[i] = acc[i];
    free(acc);
}

/* ---------- Method 2: balanced pairwise (tournament) merging ---------- */
static void merge_balanced(int **arrays, int k, int n, int *result) {
    /* current[] holds pointers to sorted runs; lens[] their lengths */
    int cur_k = k;
    int **current = malloc(k * sizeof(int *));
    int *lens = malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        current[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) current[i][j] = arrays[i][j];
        lens[i] = n;
    }
    while (cur_k > 1) {
        int next_k = (cur_k + 1) / 2;
        int **next = malloc(next_k * sizeof(int *));
        int *nlens = malloc(next_k * sizeof(int));
        int p = 0;
        for (int i = 0; i < cur_k; i += 2) {
            if (i + 1 < cur_k) {
                int outlen = lens[i] + lens[i + 1];
                int *out = malloc(outlen * sizeof(int));
                merge2(current[i], lens[i], current[i + 1], lens[i + 1], out);
                free(current[i]); free(current[i + 1]);
                next[p] = out; nlens[p] = outlen; p++;
            } else { /* odd one out carries forward unmerged */
                next[p] = current[i]; nlens[p] = lens[i]; p++;
            }
        }
        free(current); free(lens);
        current = next; lens = nlens; cur_k = next_k;
    }
    for (int i = 0; i < lens[0]; i++) result[i] = current[0][i];
    free(current[0]); free(current); free(lens);
}

static int is_sorted(const int *a, int n) { for (int i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0; return 1; }

int main(void) {
    int K[] = { 2, 4, 8, 16, 32, 64 };
    int N[] = { 10, 20, 50, 100, 200 };
    int NK = (int)(sizeof(K) / sizeof(K[0]));
    int NN = (int)(sizeof(N) / sizeof(N[0]));

    FILE *f = fopen("q3_k_sorted_arrays.csv", "w");
    if (!f) { fprintf(stderr, "cannot open output file\n"); return 1; }
    fprintf(f, "k,n,method1_comparisons,method2_comparisons,method1_writes,method2_writes\n");

    srand(20260809u);
    for (int ki = 0; ki < NK; ki++) {
        for (int ni = 0; ni < NN; ni++) {
            int k = K[ki], n = N[ni];
            int **arrays = malloc(k * sizeof(int *));
            for (int i = 0; i < k; i++) {
                arrays[i] = malloc(n * sizeof(int));
                for (int j = 0; j < n; j++) arrays[i][j] = rand() % 1000000;
                qsort(arrays[i], n, sizeof(int), cmp_int); /* each input array is sorted */
            }

            int *res1 = malloc((long)k * n * sizeof(int));
            comparisons = writes = 0;
            merge_sequential(arrays, k, n, res1);
            long long c1 = comparisons, w1 = writes;
            if (!is_sorted(res1, k * n)) { fprintf(stderr, "BUG: method 1 not sorted (k=%d n=%d)\n", k, n); return 1; }

            int *res2 = malloc((long)k * n * sizeof(int));
            comparisons = writes = 0;
            merge_balanced(arrays, k, n, res2);
            long long c2 = comparisons, w2 = writes;
            if (!is_sorted(res2, k * n)) { fprintf(stderr, "BUG: method 2 not sorted (k=%d n=%d)\n", k, n); return 1; }

            for (int i = 0; i < k * n; i++) {
                if (res1[i] != res2[i]) { fprintf(stderr, "BUG: methods disagree (k=%d n=%d)\n", k, n); return 1; }
            }

            fprintf(f, "%d,%d,%lld,%lld,%lld,%lld\n", k, n, c1, c2, w1, w2);

            for (int i = 0; i < k; i++) free(arrays[i]);
            free(arrays); free(res1); free(res2);
        }
    }
    fclose(f);
    puts("k-way merge experiment generated and verified (both methods agree and are sorted).");
    return 0;
}
