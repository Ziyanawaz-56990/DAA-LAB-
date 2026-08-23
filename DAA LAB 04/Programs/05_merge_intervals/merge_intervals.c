/* ============================================================================
 * Application of Sorting - V
 * ----------------------------------------------------------------------------
 * Problem : Given a list I of n intervals (x_i, y_i), merge all overlapping
 *           intervals and return the resulting disjoint list.
 *           e.g. {(1,3),(2,6),(8,10),(7,18)}  ->  {(1,6),(7,18)}
 *
 * Idea    : 1. Sort the intervals by their START value x_i     -> O(n log n)
 *           2. Sweep through once, keeping a "current" merged
 *              interval [curStart, curEnd]:
 *                 - if next interval's start <= curEnd, they overlap
 *                   -> extend curEnd = max(curEnd, next.end)
 *                 - else the current interval is finalised,
 *                   push it to the output and start a new one    -> O(n)
 *
 * Complexity : TIME  = O(n log n)  (dominated by the sort)
 *              SPACE = O(n)        (output list)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x; /* start */
    int y; /* end   */
} Interval;

int cmp_interval(const void *a, const void *b) {
    const Interval *ia = (const Interval *)a;
    const Interval *ib = (const Interval *)b;
    return ia->x - ib->x;
}

/* Merges overlapping intervals in-place-ish; returns count written to out[]. */
int merge_intervals(Interval in[], int n, Interval out[]) {
    if (n == 0) return 0;

    Interval *tmp = (Interval *)malloc(sizeof(Interval) * n);
    for (int i = 0; i < n; i++) tmp[i] = in[i];
    qsort(tmp, n, sizeof(Interval), cmp_interval);   /* O(n log n) */

    int count = 0;
    Interval cur = tmp[0];
    for (int i = 1; i < n; i++) {                      /* O(n) */
        if (tmp[i].x <= cur.y) {
            if (tmp[i].y > cur.y) cur.y = tmp[i].y;    /* extend */
        } else {
            out[count++] = cur;                        /* finalise */
            cur = tmp[i];
        }
    }
    out[count++] = cur;                                 /* last interval */

    free(tmp);
    return count;
}

/* Validates the merged output:
 *  (1) result intervals are sorted and pairwise disjoint (with a gap),
 *  (2) every point covered by some input interval is covered by exactly
 *      one output interval, checked via brute force over all integer
 *      points in range. */
static int validate(Interval in[], int n, Interval out[], int m) {
    for (int i = 1; i < m; i++)
        if (out[i].x <= out[i - 1].y) return 0;              /* (1) disjoint & sorted */

    int lo = in[0].x, hi = in[0].y;
    for (int i = 1; i < n; i++) {
        if (in[i].x < lo) lo = in[i].x;
        if (in[i].y > hi) hi = in[i].y;
    }
    for (int p = lo; p <= hi; p++) {
        int covered_in = 0;
        for (int i = 0; i < n; i++) if (in[i].x <= p && p <= in[i].y) { covered_in = 1; break; }
        int covered_out = 0;
        for (int i = 0; i < m; i++) if (out[i].x <= p && p <= out[i].y) { covered_out = 1; break; }
        if (covered_in != covered_out) return 0;              /* (2) same coverage */
    }
    return 1;
}

static void run_test(const char *label, Interval I[], int n) {
    printf("---------------------------------------------------------------\n");
    printf("%s\n", label);
    printf("  Input : { ");
    for (int i = 0; i < n; i++) printf("(%d,%d) ", I[i].x, I[i].y);
    printf("}\n");

    Interval out[64];
    int m = merge_intervals(I, n, out);

    printf("  Merged: { ");
    for (int i = 0; i < m; i++) printf("(%d,%d) ", out[i].x, out[i].y);
    printf("}\n");
    printf("  Check : %s\n", validate(I, n, out, m) ? "PASS (disjoint, coverage-preserving)" : "FAIL");
}

int main(void) {
    Interval t1[] = { {1, 3}, {2, 6}, {8, 10}, {7, 18} };
    run_test("Test 1: problem-statement example", t1, 4);

    Interval t2[] = { {1, 4}, {4, 5}, {10, 12}, {11, 15}, {20, 25}, {2, 3} };
    run_test("Test 2: touching + nested + separate intervals", t2, 6);

    Interval t3[] = { {5, 10}, {1, 2}, {12, 15} };
    run_test("Test 3: already disjoint (no merging needed)", t3, 3);

    Interval t4[] = { {1, 100}, {2, 3}, {4, 5}, {50, 60} };
    run_test("Test 4: one interval swallows all others", t4, 4);

    Interval t5[] = { {7, 9} };
    run_test("Test 5: single interval", t5, 1);

    printf("---------------------------------------------------------------\n");
    printf("Time Complexity : O(n log n)   [dominated by sorting on start]\n");
    printf("Space Complexity: O(n)         [output list]\n");
    return 0;
}
