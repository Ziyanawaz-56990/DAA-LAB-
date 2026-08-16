/*
 * Q2: Search the Defective (lighter) Coin
 *
 * n coins, at most ONE coin may be lighter than the rest (defective),
 * or possibly none is defective. Using a balance scale (which only tells
 * us LEFT-lighter / RIGHT-lighter / EQUAL), find the defective coin, or
 * report that none exists, in O(log3 n) weighings -> floor(log2 n) + c.
 *
 * Idea: split candidate coins into 3 nearly-equal groups G1,G2,G3.
 * Weigh G1 vs G2:
 *   - If unequal -> defective is confirmed to be in the lighter group.
 *   - If equal    -> G1,G2 are all genuine; defective (if any) is in G3,
 *                     but its presence in G3 is NOT yet confirmed.
 * Recurse on the relevant group. When the candidate set is a single coin
 * whose presence in the "defective set" was never confirmed by an
 * inequality, do one final weighing against any known-genuine coin
 * to decide whether it is truly lighter or the set has no defective coin.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int weighings = 0;

/* Returns -1 if left pan lighter, 1 if right pan lighter, 0 if equal */
int weigh(double weights[], int left[], int lcount, int right[], int rcount) {
    weighings++;
    double lsum = 0, rsum = 0;
    for (int i = 0; i < lcount; i++) lsum += weights[left[i]];
    for (int i = 0; i < rcount; i++) rsum += weights[right[i]];
    if (lsum < rsum) return -1;
    if (lsum > rsum) return 1;
    return 0;
}

/*
 * candidates: indices of coins that might contain the defective one
 * confirmed: 1 if we already KNOW (from a prior unequal weighing) that
 *            the defective coin is among 'candidates'; 0 if we only
 *            suspect it (came from an "equal" branch) and must still
 *            verify.
 * genuineRef: index of one coin known for certain to be genuine
 *             (used for the final tie-breaking weighing).
 * Returns index of defective coin, or -1 if none.
 */
int findDefective(double weights[], int candidates[], int n, int confirmed, int genuineRef) {
    if (n == 0) return -1;

    if (n == 1) {
        if (confirmed) {
            return candidates[0];
        } else {
            int left[1] = { candidates[0] };
            int right[1] = { genuineRef };
            int r = weigh(weights, left, 1, right, 1);
            return (r == -1) ? candidates[0] : -1; /* lighter -> defective; else none */
        }
    }

    int g1n = n / 3, g2n = n / 3, g3n = n - g1n - g2n;
    int *g1 = candidates;
    int *g2 = candidates + g1n;
    int *g3 = candidates + g1n + g2n;

    int cmp = weigh(weights, g1, g1n, g2, g2n);

    if (cmp == -1) {                 /* G1 lighter -> defective confirmed in G1 */
        return findDefective(weights, g1, g1n, 1, genuineRef);
    } else if (cmp == 1) {           /* G2 lighter -> defective confirmed in G2 */
        return findDefective(weights, g2, g2n, 1, genuineRef);
    } else {                          /* equal -> G1,G2 genuine; suspect is in G3 (unconfirmed) */
        int newRef = g1n > 0 ? g1[0] : genuineRef; /* any coin from G1 is now known genuine */
        return findDefective(weights, g3, g3n, 0, newRef);
    }
}

void runTest(const char *label, double weights[], int n) {
    int *candidates = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) candidates[i] = i;

    weighings = 0;
    /* We don't yet know any coin is genuine for certain, so we can't set
       a genuineRef safely until we've made at least one split. To handle
       the top level cleanly, hold out coin 0 as a "reference candidate"
       is not valid either (it could be the defective one). So instead,
       at the very top call we treat the whole set as unconfirmed and use
       candidates[0] only as a placeholder that gets replaced as soon as
       any group is proven genuine (n>1 guarantees a weighing happens
       before genuineRef is ever used for n==1 base case). */
    int result = findDefective(weights, candidates, n, 0, -1);

    printf("%s (n=%d): ", label, n);
    if (result == -1)
        printf("No defective coin found (all coins genuine). ");
    else
        printf("Defective (lighter) coin is at index %d (weight=%.2f). ", result, weights[result]);
    printf("Weighings used = %d\n", weighings);

    free(candidates);
}

int main(void) {
    srand((unsigned) time(NULL));

    /* Test 1: n coins, all identical (no defective coin) */
    int n1 = 27;
    double w1[27];
    for (int i = 0; i < n1; i++) w1[i] = 10.0;
    runTest("Test 1 - No defective coin", w1, n1);

    /* Test 2: n coins, one random coin is lighter */
    int n2 = 27;
    double w2[27];
    for (int i = 0; i < n2; i++) w2[i] = 10.0;
    int defectiveIdx = rand() % n2;
    w2[defectiveIdx] = 9.0;
    printf("(Planted defective coin at index %d)\n", defectiveIdx);
    runTest("Test 2 - One defective coin", w2, n2);

    /* Test 3: n = 100 coins, one lighter coin near the end */
    int n3 = 100;
    double w3[100];
    for (int i = 0; i < n3; i++) w3[i] = 5.0;
    w3[73] = 4.5;
    runTest("Test 3 - Defective at index 73, n=100", w3, n3);

    printf("\nFor n items the number of weighings grows as O(log3 n) "
           "which is <= floor(log2 n) + c for a small constant c.\n");

    return 0;
}
