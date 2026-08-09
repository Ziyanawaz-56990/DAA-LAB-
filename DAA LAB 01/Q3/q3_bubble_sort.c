/*
 * DAA Lab-01 | Q3 - Performance analysis of Bubble Sort
 * Version A: early termination when a pass makes no swaps.
 * Version B: always performs n-1 passes.
 */

#include <stdio.h>
#include <stdlib.h>

static void copy_array(const int *src, int *dst, int n) {
    for (int i = 0; i < n; ++i) dst[i] = src[i];
}

static long long bubble_early(int *a, int n, long long *swaps) {
    long long comparisons = 0;
    *swaps = 0;

    for (int pass = 0; pass < n - 1; ++pass) {
        int swapped = 0;

        for (int j = 0; j < n - 1 - pass; ++j) {
            ++comparisons;

            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                ++(*swaps);
                swapped = 1;
            }
        }

        if (!swapped) break;
    }

    return comparisons;
}

static long long bubble_full(int *a, int n, long long *swaps) {
    long long comparisons = 0;
    *swaps = 0;

    for (int pass = 0; pass < n - 1; ++pass) {
        for (int j = 0; j < n - 1 - pass; ++j) {
            ++comparisons;

            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                ++(*swaps);
            }
        }
    }

    return comparisons;
}

int main(void) {
    const int sizes[] = {10, 25, 50, 100, 200, 400, 600, 800, 1000};
    const int count = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("q3_bubble_sort.csv", "w");
    if (!fp) {
        perror("q3_bubble_sort.csv");
        return 1;
    }

    fprintf(fp, "n,early_comparisons,full_comparisons,early_swaps,full_swaps\n");

    srand(20260809);

    for (int s = 0; s < count; ++s) {
        int n = sizes[s];
        int *original = malloc((size_t)n * sizeof(int));
        int *a = malloc((size_t)n * sizeof(int));
        int *b = malloc((size_t)n * sizeof(int));

        if (!original || !a || !b) {
            fprintf(stderr, "Memory allocation failed for n=%d\n", n);
            free(original); free(a); free(b);
            fclose(fp);
            return 1;
        }

        for (int i = 0; i < n; ++i) original[i] = rand();

        copy_array(original, a, n);
        copy_array(original, b, n);

        long long early_swaps, full_swaps;
        long long early_comparisons = bubble_early(a, n, &early_swaps);
        long long full_comparisons = bubble_full(b, n, &full_swaps);

        fprintf(fp, "%d,%lld,%lld,%lld,%lld\n",
                n, early_comparisons, full_comparisons,
                early_swaps, full_swaps);

        free(original);
        free(a);
        free(b);
    }

    fclose(fp);
    printf("Bubble-sort experiment complete.\n");
    printf("CSV written to q3_bubble_sort.csv\n");
    return 0;
}
