/*
 * Q2 - Super Egg Testing Experiment (generalized Egg Drop problem)
 * ------------------------------------------------------------------
 * Given E eggs and F floors, compute the minimum number of droppings
 * that guarantee finding the highest safe floor.
 *
 * Implements:
 *   1) Direct DP recurrence            - O(E*F^2) time, O(E*F) space
 *   2) Optimised dual DP (max floors    - O(E*F)   time, O(E)   space
 *      resolvable with m trials)
 * and cross-checks that both agree.
 *
 * Compile : gcc -O2 -Wall -o solution solution.c
 * Run     : ./solution        (prompts for E and F)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

/* ---------- Method 1: direct DP, O(E*F^2) ---------- */
int eggDropDirect(int E, int F) {
    int **dp = malloc((E + 1) * sizeof(int *));
    for (int e = 0; e <= E; e++)
        dp[e] = calloc((F + 1), sizeof(int));

    for (int f = 1; f <= F; f++) dp[1][f] = f;      /* 1 egg: linear search */
    for (int e = 2; e <= E; e++) dp[e][0] = 0;

    for (int e = 2; e <= E; e++) {
        for (int f = 1; f <= F; f++) {
            int best = INT_MAX;
            for (int x = 1; x <= f; x++) {
                int worst = max(dp[e - 1][x - 1], dp[e][f - x]);
                best = min(best, 1 + worst);
            }
            dp[e][f] = best;
        }
    }

    int result = dp[E][F];
    for (int e = 0; e <= E; e++) free(dp[e]);
    free(dp);
    return result;
}

/* ---------- Method 2: dual DP, O(E*F) ---------- */
/* f(m, e) = max floors resolvable with m trials and e eggs
 * f(m, e) = f(m-1, e-1) + f(m-1, e) + 1
 * Find smallest m with f(m, E) >= F.
 */
int eggDropOptimised(int E, int F) {
    /* row[e] holds f(m, e) for the current m, rolling over m */
    int *row = calloc(E + 1, sizeof(int));   /* f(0, e) = 0 for all e */
    int m = 0;

    while (row[E] < F) {
        m++;
        /* update from high e to low e so row[e-1] is still the (m-1) value */
        for (int e = E; e >= 1; e--) {
            row[e] = row[e] + row[e - 1] + 1;
        }
    }

    free(row);
    return m;
}

int main(void) {
    int E, F;
    printf("=== Super Egg Testing Experiment ===\n");
    printf("Enter number of eggs (E): ");
    if (scanf("%d", &E) != 1 || E <= 0) { printf("Invalid input.\n"); return 1; }
    printf("Enter number of floors (F): ");
    if (scanf("%d", &F) != 1 || F <= 0) { printf("Invalid input.\n"); return 1; }

    int direct = -1;
    if ((long long)E * F * F <= 20000000LL) {  /* keep the O(E*F^2) demo fast */
        direct = eggDropDirect(E, F);
        printf("\nMinimum droppings (direct DP,   O(E*F^2)) : %d\n", direct);
    } else {
        printf("\n(direct O(E*F^2) DP skipped - E*F too large for this demo)\n");
    }

    int optimised = eggDropOptimised(E, F);
    printf("Minimum droppings (optimised DP, O(E*F))   : %d\n", optimised);

    if (direct != -1)
        printf("Both methods %s.\n", (direct == optimised) ? "AGREE" : "DISAGREE");

    /* Classic textbook instance check */
    if (E == 2 && F == 100)
        printf("(Classic instance: 2 eggs, 100 floors -> expected answer is 14)\n");

    return 0;
}
