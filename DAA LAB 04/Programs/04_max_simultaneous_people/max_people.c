/* ============================================================================
 * Application of Sorting - IV
 * ----------------------------------------------------------------------------
 * Problem : A camera records entry time a_i and exit time b_i (b_i > a_i)
 *           for each of n persons at a party. Find the time at which the
 *           MOST people were simultaneously present. All 2n times are
 *           distinct (no ties).
 *
 * Idea    : Classic sweep line.
 *           1. Build 2n EVENTS: (a_i, +1)  "arrival"
 *                                (b_i, -1)  "departure"
 *           2. Sort all events by time                        -> O(n log n)
 *           3. Sweep left to right, keeping a running counter
 *              'current'. At each event: current += delta.
 *              Track the maximum value of 'current' and the
 *              time at which it occurs                        -> O(n)
 *
 * Complexity : TIME  = O(n log n)   (dominated by sorting 2n events)
 *              SPACE = O(n)         (event array)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int time;
    int delta;   /* +1 for arrival, -1 for departure */
} Event;

int cmp_event(const void *a, const void *b) {
    const Event *ea = (const Event *)a;
    const Event *eb = (const Event *)b;
    return ea->time - eb->time;      /* times are guaranteed distinct */
}

/* Returns the time of maximum simultaneous presence and stores the
 * count in *max_count. */
int find_peak_time(int a[], int b[], int n, int *max_count) {
    Event *events = (Event *)malloc(sizeof(Event) * 2 * n);
    for (int i = 0; i < n; i++) {
        events[2 * i]     = (Event){a[i], +1};
        events[2 * i + 1] = (Event){b[i], -1};
    }

    qsort(events, 2 * n, sizeof(Event), cmp_event);   /* O(n log n) */

    int current = 0, best = 0, best_time = events[0].time;
    for (int i = 0; i < 2 * n; i++) {                  /* O(n) */
        current += events[i].delta;
        if (current > best) {
            best = current;
            best_time = events[i].time;
        }
    }

    free(events);
    *max_count = best;
    return best_time;
}

/* Brute-force O(n * range) check: for every integer time between the
 * overall min and max, count how many intervals [a_i,b_i) cover it, and
 * take the best. Used only to VALIDATE the sweep-line answer. */
static int brute_force_max(int a[], int b[], int n) {
    int lo = a[0], hi = b[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < lo) lo = a[i];
        if (b[i] > hi) hi = b[i];
    }
    int best = 0;
    for (int t = lo; t <= hi; t++) {
        int cnt = 0;
        for (int i = 0; i < n; i++)
            if (a[i] <= t && t <= b[i]) cnt++;
        if (cnt > best) best = cnt;
    }
    return best;
}

static void run_test(const char *label, int a[], int b[], int n) {
    printf("---------------------------------------------------------------\n");
    printf("%s  (n = %d)\n", label, n);
    printf("  Persons (entry, exit): ");
    for (int i = 0; i < n; i++) printf("(%d,%d) ", a[i], b[i]);
    printf("\n");

    int max_count;
    int peak_time = find_peak_time(a, b, n, &max_count);
    int brute_max  = brute_force_max(a, b, n);

    printf("  Sweep-line result : peak time = %d, max people = %d\n", peak_time, max_count);
    printf("  Brute-force check : max people = %d  -> %s\n",
           brute_max, (brute_max == max_count) ? "PASS" : "FAIL");
}

int main(void) {
    int a1[] = {1, 2, 4, 7, 9, 3};
    int b1[] = {5, 6, 8, 12, 11, 10};
    run_test("Test 1: general overlapping arrivals", a1, b1, 6);

    int a2[] = {1, 10, 20};
    int b2[] = {5, 15, 25};
    run_test("Test 2: no overlaps at all", a2, b2, 3);

    int a3[] = {1, 2, 3, 4, 5};
    int b3[] = {100, 99, 98, 97, 96};
    run_test("Test 3: everyone overlaps everyone", a3, b3, 5);

    int a4[] = {5, 1};
    int b4[] = {9, 20};
    run_test("Test 4: only two people", a4, b4, 2);

    printf("---------------------------------------------------------------\n");
    printf("Time Complexity : O(n log n)   [sort 2n events, single sweep]\n");
    printf("Space Complexity: O(n)         [event array]\n");
    return 0;
}
