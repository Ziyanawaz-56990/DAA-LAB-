/*
 * DAA Lab-02, Question 2: Merge Sort vs Modified (3-way) Merge Sort
 * -------------------------------------------------------------------
 * Standard Merge Sort splits the array into halves:      T(n) = 2T(n/2) + Theta(n)
 * Modified Merge Sort splits the array into thirds and combines the three
 * sorted thirds with a three-way merge:                  T(n) = 3T(n/3) + Theta(n)
 * Both solve, by the Master Theorem (case 2, since f(n) = Theta(n^{log_b a})),
 * to Theta(n log n). The base of the logarithm is a constant factor and does
 * not change the asymptotic class.
 *
 * This program actually runs both algorithms on identical random input for a
 * range of n, counts the true number of key comparisons and array writes
 * performed, and verifies the output is sorted, so the experiment is a real
 * empirical validation rather than a restatement of the formula.
 *
 * Compile: gcc -O2 -Wall -o q2_merge_sort q2_merge_sort.c
 * Run:     ./q2_merge_sort
 */
#include <stdio.h>
#include <stdlib.h>

static long long comparisons, writes;

/* ---------------- standard 2-way merge sort ---------------- */
static void merge2(int *a, int lo, int mid, int hi, int *tmp) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        comparisons++;
        tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
        writes++;
    }
    while (i <= mid) { tmp[k++] = a[i++]; writes++; }
    while (j <= hi)  { tmp[k++] = a[j++]; writes++; }
    for (i = lo; i <= hi; i++) { a[i] = tmp[i]; writes++; }
}
static void mergeSort2(int *a, int lo, int hi, int *tmp) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort2(a, lo, mid, tmp);
    mergeSort2(a, mid + 1, hi, tmp);
    merge2(a, lo, mid, hi, tmp);
}

/* ---------------- modified 3-way merge sort ---------------- */
/* Merge three sorted runs a[lo..m1], a[m1+1..m2], a[m2+1..hi] using a
 * tournament of pairwise comparisons: to pick the smallest of up to three
 * candidates we use two comparisons in the worst case (candidate vs
 * candidate, winner vs candidate), matching a textbook 3-way merge. */
static void merge3(int *a, int lo, int m1, int m2, int hi, int *tmp) {
    int i = lo, j = m1 + 1, k = m2 + 1, p = lo;
    while (i <= m1 || j <= m2 || k <= hi) {
        int bi = -1, bv = 0;
        if (i <= m1) { bi = 0; bv = a[i]; }
        if (j <= m2) {
            if (bi == -1) { bi = 1; bv = a[j]; }
            else { comparisons++; if (a[j] < bv) { bi = 1; bv = a[j]; } }
        }
        if (k <= hi) {
            if (bi == -1) { bi = 2; bv = a[k]; }
            else { comparisons++; if (a[k] < bv) { bi = 2; bv = a[k]; } }
        }
        if (bi == 0) i++; else if (bi == 1) j++; else k++;
        tmp[p++] = bv; writes++;
    }
    for (int z = lo; z <= hi; z++) { a[z] = tmp[z]; writes++; }
}
static void mergeSort3(int *a, int lo, int hi, int *tmp) {
    int n = hi - lo + 1;
    if (n <= 1) return;
    if (n == 2) { /* only two elements: fall back to a normal 2-way merge */
        comparisons++;
        if (a[lo] > a[hi]) { int t = a[lo]; a[lo] = a[hi]; a[hi] = t; writes += 2; }
        return;
    }
    int third = n / 3;
    int m1 = lo + third - 1;
    int m2 = lo + 2 * third - 1;
    mergeSort3(a, lo, m1, tmp);
    mergeSort3(a, m1 + 1, m2, tmp);
    mergeSort3(a, m2 + 1, hi, tmp);
    merge3(a, lo, m1, m2, hi, tmp);
}

static int is_sorted(int *a, int n) { for (int i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0; return 1; }

int main(void) {
    int N[] = { 30, 60, 120, 240, 480, 960, 1920, 3840, 7680, 15360 };
    int NN = (int)(sizeof(N) / sizeof(N[0]));
    FILE *f = fopen("q2_merge_sort.csv", "w");
    if (!f) { fprintf(stderr, "cannot open output file\n"); return 1; }
    fprintf(f, "n,two_way_comparisons,three_way_comparisons,two_way_writes,three_way_writes\n");

    srand(20260809u);
    for (int z = 0; z < NN; z++) {
        int n = N[z];
        int *base = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) base[i] = rand();

        int *a = malloc(n * sizeof(int)), *tmpA = malloc(n * sizeof(int));
        int *b = malloc(n * sizeof(int)), *tmpB = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) a[i] = b[i] = base[i];

        comparisons = writes = 0;
        mergeSort2(a, 0, n - 1, tmpA);
        long long c2 = comparisons, w2 = writes;
        if (!is_sorted(a, n)) { fprintf(stderr, "BUG: 2-way merge sort failed to sort n=%d\n", n); return 1; }

        comparisons = writes = 0;
        mergeSort3(b, 0, n - 1, tmpB);
        long long c3 = comparisons, w3 = writes;
        if (!is_sorted(b, n)) { fprintf(stderr, "BUG: 3-way merge sort failed to sort n=%d\n", n); return 1; }

        fprintf(f, "%d,%lld,%lld,%lld,%lld\n", n, c2, c3, w2, w3);
        free(base); free(a); free(tmpA); free(b); free(tmpB);
    }
    fclose(f);
    puts("Merge-sort comparison/write data generated and verified (all outputs sorted).");
    return 0;
}
