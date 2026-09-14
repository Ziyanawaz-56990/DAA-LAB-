/*
 * Q5 - Hitting a Moving Target
 * -------------------------------------------------------------------
 * n hiding spots on a line (1..n). Target moves to an adjacent spot
 * between shots; shooter gets no feedback and must fire a fixed
 * sequence guaranteed to hit the target regardless of its (unknown)
 * start and moves. Solved exactly via BFS over "belief sets" (the set
 * of positions the target could still be at).
 *
 * Compile : gcc -O2 -Wall -o solution solution.c
 * Run     : ./solution        (prompts for n)
 */

#include <stdio.h>
#include <stdlib.h>

int expandToNeighbors(int set, int n) {
    int result = 0;
    for (int s = 1; s <= n; s++) {
        if (!((set >> (s - 1)) & 1)) continue;
        if (s - 1 >= 1) result |= (1 << (s - 2));
        if (s + 1 <= n) result |= (1 << s);
    }
    return result;
}

int main(void) {
    int n;
    printf("=== Hitting a Moving Target ===\n");
    printf("Enter number of hiding spots (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input.\n"); return 1; }

    if (n == 1) {
        printf("\nOnly 1 spot: shoot it once. Minimum shots = 1\n");
        return 0;
    }
    if (n > 20) {
        printf("n too large for this BFS demo (state space 2^n). Try n <= 20.\n");
        return 1;
    }

    int size = 1 << n;
    int full = size - 1;

    int *dist = malloc(sizeof(int) * size);
    int *parentState = malloc(sizeof(int) * size);
    int *parentShot  = malloc(sizeof(int) * size);
    int *queue = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) dist[i] = -1;

    int head = 0, tail = 0;
    dist[full] = 0;
    queue[tail++] = full;

    int finalShot = -1, finalFromState = -1, found = 0;
    int answerLen = -1;

    while (head < tail && !found) {
        int S = queue[head++];
        for (int p = 1; p <= n && !found; p++) {
            int Sprime = S & ~(1 << (p - 1));
            if (Sprime == 0) {
                finalShot = p;
                finalFromState = S;
                answerLen = dist[S] + 1;
                found = 1;
                break;
            }
            int Snext = expandToNeighbors(Sprime, n);
            if (dist[Snext] == -1) {
                dist[Snext] = dist[S] + 1;
                parentState[Snext] = S;
                parentShot[Snext]  = p;
                queue[tail++] = Snext;
            }
        }
    }

    if (!found) {
        printf("\nNo guaranteed hitting sequence found within reachable states "
               "(unexpected for n >= 2).\n");
        free(dist); free(parentState); free(parentShot); free(queue);
        return 1;
    }

    printf("\nMinimum guaranteed shots : %d\n", answerLen);

    /* reconstruct the shot sequence */
    int *seq = malloc(sizeof(int) * answerLen);
    seq[answerLen - 1] = finalShot;
    int cur = finalFromState;
    int idx = answerLen - 2;
    while (idx >= 0) {
        seq[idx] = parentShot[cur];
        cur = parentState[cur];
        idx--;
    }

    printf("Shot sequence (guaranteed to hit the target): ");
    for (int i = 0; i < answerLen; i++) {
        printf("%d", seq[i]);
        printf(i == answerLen - 1 ? "\n" : ", ");
    }

    /* Show the known closed-form pattern for comparison, n >= 3 */
    if (n >= 3) {
        printf("\nKnown closed-form pattern (sweep in, then out), for reference:\n");
        printf("  ");
        for (int p = 2; p <= n - 1; p++) printf("%d, ", p);
        for (int p = n - 1; p >= 2; p--) printf("%d%s", p, (p == 2) ? "\n" : ", ");
        printf("  (length = 2n - 4 = %d)\n", 2 * n - 4);
    }

    free(seq); free(dist); free(parentState); free(parentShot); free(queue);
    return 0;
}
