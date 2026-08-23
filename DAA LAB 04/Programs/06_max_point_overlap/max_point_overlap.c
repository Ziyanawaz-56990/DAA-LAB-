/* ============================================================================
 * Application of Sorting - VI
 * ----------------------------------------------------------------------------
 * Problem : Given a set S of n intervals [l_i, r_i] on a line (endpoints are
 *           INCLUSIVE - an endpoint counts as being inside its interval),
 *           find a point p that lies in the largest number of intervals.
 *
 * Idea    : Sweep line, same spirit as Problem IV, adapted for inclusive
 *           endpoints:
 *           1. For every interval [l_i, r_i] create two events on the
 *              integer line:
 *                    (l_i,      +1)   coverage starts AT l_i
 *                    (r_i + 1,  -1)   coverage ends right AFTER r_i
 *              (the "+1 past the end" trick correctly keeps r_i itself
 *               counted, since the decrement only takes effect after it)
 *           2. Sort all 2n events by coordinate                 -> O(n log n)
 *           3. Sweep left to right maintaining a running counter
 *              'current'; whenever current exceeds the best seen
 *              so far, record it together with the coordinate     -> O(n)
 *
 * Complexity : TIME  = O(n log n)   (dominated by sorting 2n events)
 *              SPACE = O(n)         (event array)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int coord;
    int delta;
} Event;

int cmp_event(const void *a, const void *b) {
    const Event *ea = (const Event *)a;
    const Event *eb = (const Event *)b;
    if (ea->coord != eb->coord) return ea->coord - eb->coord;
    return eb->delta - ea->delta; /* process +1 (starts) before -1 (ends) at same coord */
}

/* Returns a point p achieving the maximum coverage; stores the coverage
 * count in *max_coverage. */
int find_max_overlap_point(int l[], int r[], int n, int *max_coverage) {
    Event *events = (Event *)malloc(sizeof(Event) * 2 * n);
    for (int i = 0; i < n; i++) {
        events[2 * i]     = (Event){ l[i],     +1 };
        events[2 * i + 1] = (Event){ r[i] + 1, -1 };
    }

    qsort(events, 2 * n, sizeof(Event), cmp_event);   /* O(n log n) */

    int current = 0, best = 0, best_point = l[0];
    for (int i = 0; i < 2 * n; i++) {                  /* O(n) */
        current += events[i].delta;
        if (current > best) {
            best = current;
            best_point = events[i].coord;              /* coverage starts here */
        }
    }

    free(events);
    *max_coverage = best;
    return best_point;
}

/* Brute force over every integer point in range: count how many intervals
 * contain it (inclusive), take the max. Used only to VALIDATE the result,
 * and also to show every point that attains the maximum. */
static int brute_force_max(int l[], int r[], int n) {
    int lo = l[0], hi = r[0];
    for (int i = 1; i < n; i++) {
        if (l[i] < lo) lo = l[i];
        if (r[i] > hi) hi = r[i];
    }
    int best = 0;
    for (int p = lo; p <= hi; p++) {
        int cnt = 0;
        for (int i = 0; i < n; i++)
            if (l[i] <= p && p <= r[i]) cnt++;
        if (cnt > best) best = cnt;
    }
    return best;
}

static void run_test(const char *label, int l[], int r[], int n) {
    printf("---------------------------------------------------------------\n");
    printf("%s  (n = %d)\n", label, n);
    printf("  Intervals: ");
    for (int i = 0; i < n; i++) printf("(%d,%d) ", l[i], r[i]);
    printf("\n");

    int max_coverage;
    int p = find_max_overlap_point(l, r, n, &max_coverage);
    int brute_max = brute_force_max(l, r, n);

    printf("  Sweep-line result : p = %d is covered by %d interval(s)\n", p, max_coverage);
    printf("  Brute-force check : true maximum coverage = %d  -> %s\n",
           brute_max, (brute_max == max_coverage) ? "PASS" : "FAIL");
}

int main(void) {
    int l1[] = {10, 20, 50, 15};
    int r1[] = {40, 60, 90, 70};
    run_test("Test 1: problem-statement example", l1, r1, 4);

    int l2[] = {1, 2, 3, 4};
    int r2[] = {10, 10, 10, 10};
    run_test("Test 2: all intervals share one point", l2, r2, 4);

    int l3[] = {1, 5, 10};
    int r3[] = {2, 6, 11};
    run_test("Test 3: no interval overlaps another", l3, r3, 3);

    int l4[] = {1, 3, 5};
    int r4[] = {3, 5, 7};
    run_test("Test 4: intervals touch exactly at endpoints", l4, r4, 3);

    printf("---------------------------------------------------------------\n");
    printf("Time Complexity : O(n log n)   [sort 2n events, single sweep]\n");
    printf("Space Complexity: O(n)         [event array]\n");
    return 0;
}
