/*
 * DAA Lab-01 | Q6 - Element Uniqueness
 * Checks a set of random integers for duplicates using pairwise comparison.
 * The program also records the number of comparisons for the experiment.
 */

#include <stdio.h>
#include <stdlib.h>

static int has_duplicate(const int a[], int n, long long *comparisons) {
    *comparisons = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            ++(*comparisons);
            if (a[i] == a[j]) return 1;
        }
    }

    return 0;
}

int main(void) {
    const int sizes[] = {10, 25, 50, 100, 200, 400, 600, 800, 1000};
    const int count = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("q6_element_uniqueness.csv", "w");
    if (!fp) {
        perror("q6_element_uniqueness.csv");
        return 1;
    }

    fprintf(fp, "n,comparisons,result,worst_case_comparisons\n");

    srand(20260809);

    for (int s = 0; s < count; ++s) {
        int n = sizes[s];
        int *a = malloc((size_t)n * sizeof(int));

        if (!a) {
            fprintf(stderr, "Memory allocation failed for n=%d\n", n);
            fclose(fp);
            return 1;
        }

        /*
         * The range is 4n, so duplicates become increasingly likely,
         * while still retaining a genuinely random input sequence.
         */
        for (int i = 0; i < n; ++i)
            a[i] = rand() % (4 * n);

        long long comparisons;
        int duplicate = has_duplicate(a, n, &comparisons);
        long long worst_case = (long long)n * (n - 1) / 2;

        fprintf(fp, "%d,%lld,%s,%lld\n",
                n, comparisons, duplicate ? "duplicate_found" : "all_unique",
                worst_case);

        printf("n=%d -> %s, comparisons=%lld\n",
               n, duplicate ? "duplicate found" : "all elements unique",
               comparisons);

        free(a);
    }

    fclose(fp);
    printf("CSV written to q6_element_uniqueness.csv\n");
    return 0;
}
