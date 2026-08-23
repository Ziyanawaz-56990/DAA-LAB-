/* ============================================================================
 * Application of Sorting - I
 * ----------------------------------------------------------------------------
 * Problem : n pairs (number, colour) are given, ALREADY SORTED BY NUMBER.
 *           colour is one of {RED, BLUE, YELLOW}.
 *           Re-order the array so that all REDs come before all BLUEs before
 *           all YELLOWs, while the numbers WITHIN each colour stay sorted
 *           (i.e. the sort must be STABLE).
 *
 * Idea    : Because the array is already sorted by number, a stable
 *           3-way bucket (counting) sort by colour automatically keeps the
 *           numbers sorted inside every colour bucket - we never need to
 *           compare numbers again.
 *
 *             Pass 1 : count how many items of each colour exist   -> O(n)
 *             Pass 2 : compute starting offset of each colour      -> O(1)  (3 colours)
 *             Pass 3 : place every item into its bucket position,
 *                      scanning the input left to right (stable)   -> O(n)
 *
 * Complexity : TIME  = O(n)      (three linear passes, no comparisons)
 *              SPACE = O(n)      (one auxiliary output array)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef enum { RED = 0, BLUE = 1, YELLOW = 2 } Colour;

typedef struct {
    int    number;
    Colour colour;
} Item;

static const char *colour_name(Colour c) {
    switch (c) {
        case RED:    return "RED";
        case BLUE:   return "BLUE";
        default:     return "YELLOW";
    }
}

/* ---------------------------------------------------------------------- *
 * sort_by_colour : stable O(n) counting sort over 3 colour buckets       *
 * ---------------------------------------------------------------------- */
void sort_by_colour(Item arr[], int n) {
    int count[3] = {0, 0, 0};           /* Pass 1: O(n) */
    for (int i = 0; i < n; i++)
        count[arr[i].colour]++;

    int start[3];                        /* Pass 2: O(1) - only 3 colours */
    start[0] = 0;
    start[1] = start[0] + count[0];
    start[2] = start[1] + count[1];

    Item *out = (Item *)malloc(sizeof(Item) * n);
    int next[3] = { start[0], start[1], start[2] };

    for (int i = 0; i < n; i++) {        /* Pass 3: O(n), stable */
        Colour c = arr[i].colour;
        out[next[c]++] = arr[i];
    }

    for (int i = 0; i < n; i++)          /* copy back */
        arr[i] = out[i];

    free(out);
}

/* ---------------------------------------------------------------------- *
 * Validation helpers                                                     *
 * ---------------------------------------------------------------------- */
static void print_items(Item arr[], int n) {
    printf("[ ");
    for (int i = 0; i < n; i++)
        printf("(%d,%s) ", arr[i].number, colour_name(arr[i].colour));
    printf("]\n");
}

/* Checks: (1) colours appear in RED,BLUE,YELLOW block order,
 *         (2) numbers are non-decreasing WITHIN each colour block
 *             (stability requirement),
 *         (3) the multiset of items is unchanged (a permutation of input). */
static int validate(Item before[], Item after[], int n) {
    for (int i = 1; i < n; i++)
        if (after[i - 1].colour > after[i].colour) return 0;           /* (1) */

    for (int i = 1; i < n; i++)
        if (after[i - 1].colour == after[i].colour &&
            after[i - 1].number > after[i].number) return 0;           /* (2) */

    int count_before[3] = {0}, count_after[3] = {0};
    for (int i = 0; i < n; i++) { count_before[before[i].colour]++; count_after[after[i].colour]++; }
    for (int c = 0; c < 3; c++)
        if (count_before[c] != count_after[c]) return 0;                /* (3) */

    return 1;
}

static void run_test(const char *label, Item items[], int n) {
    Item original[64];
    for (int i = 0; i < n; i++) original[i] = items[i];

    printf("---------------------------------------------------------------\n");
    printf("%s  (n = %d)\n", label, n);
    printf("  Input : "); print_items(items, n);

    sort_by_colour(items, n);

    printf("  Output: "); print_items(items, n);
    printf("  Check : %s\n", validate(original, items, n)
                              ? "PASS (RED<BLUE<YELLOW order, stable within colour)"
                              : "FAIL");
}

int main(void) {
    Item t1[] = {
        {1,  BLUE}, {2, RED}, {4, YELLOW}, {5, RED},
        {7,  BLUE}, {8, YELLOW}, {9, BLUE}, {12, RED}
    };
    run_test("Test 1: mixed colours", t1, sizeof(t1) / sizeof(t1[0]));

    Item t2[] = { {3, RED}, {6, RED}, {9, RED} };
    run_test("Test 2: single colour only", t2, sizeof(t2) / sizeof(t2[0]));

    Item t3[] = { {1, YELLOW}, {2, BLUE}, {3, RED} };
    run_test("Test 3: reverse colour order in input", t3, sizeof(t3) / sizeof(t3[0]));

    Item t4[] = { {10, RED} };
    run_test("Test 4: single element", t4, sizeof(t4) / sizeof(t4[0]));

    printf("---------------------------------------------------------------\n");
    printf("Time Complexity : O(n)   [three linear passes, no comparisons]\n");
    printf("Space Complexity: O(n)   [one auxiliary output array]\n");
    return 0;
}
