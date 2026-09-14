/*
 * Q4 - Security Switches
 * -------------------------------------------------------------------
 * n switches (bit i-1 = switch i, 1=ON), switch 1 always toggleable;
 * switch k>1 toggleable only if switch k-1 is ON and switches 1..k-2
 * are all OFF. Start: all ON. Goal: all OFF. Minimum moves?
 *
 * Implements:
 *   1) Exact BFS over the 2^n-state graph (also reconstructs the move
 *      sequence)                                    - O(n*2^n)
 *   2) Closed-form Jacobsthal-style formula          - O(log n)
 * and cross-checks them.
 *
 * Compile : gcc -O2 -Wall -o solution solution.c
 * Run     : ./solution        (prompts for n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int legalMove(int state, int k) {
    if (k == 1) return 1;
    int rightBits = state & ((1 << (k - 2)) - 1);   /* switches 1..k-2 */
    int neighborOn = (state >> (k - 2)) & 1;         /* switch k-1     */
    return neighborOn && (rightBits == 0);
}

/* Exact BFS solver. Returns move count, and if moves!=NULL fills it
 * with the sequence of switch indices toggled (1-indexed), returning
 * the count via *outLen. */
long long solveBFS(int n, int *moves, int *outLen) {
    int size = 1 << n;
    int *dist = malloc(sizeof(int) * size);
    int *parentState = malloc(sizeof(int) * size);
    int *parentMove  = malloc(sizeof(int) * size);
    int *queue = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) dist[i] = -1;

    int start = size - 1;   /* all ON  */
    int goal  = 0;           /* all OFF */

    int head = 0, tail = 0;
    dist[start] = 0;
    queue[tail++] = start;

    while (head < tail) {
        int s = queue[head++];
        if (s == goal) break;
        for (int k = 1; k <= n; k++) {
            if (!legalMove(s, k)) continue;
            int s2 = s ^ (1 << (k - 1));
            if (dist[s2] == -1) {
                dist[s2] = dist[s] + 1;
                parentState[s2] = s;
                parentMove[s2]  = k;
                queue[tail++] = s2;
            }
        }
    }

    long long result = dist[goal];

    if (moves != NULL) {
        int len = 0;
        int cur = goal;
        int *rev = malloc(sizeof(int) * (result + 1));
        while (cur != start) {
            rev[len++] = parentMove[cur];
            cur = parentState[cur];
        }
        for (int i = 0; i < len; i++) moves[i] = rev[len - 1 - i];
        *outLen = len;
        free(rev);
    }

    free(dist); free(parentState); free(parentMove); free(queue);
    return result;
}

/* Closed-form formula : O(log n) via fast exponentiation */
long long minMovesFormula(int n) {
    long long p = 1;
    for (int i = 0; i < n + 1; i++) p *= 2;   /* 2^(n+1) */
    if (n % 2 == 1) return (p - 1) / 3;
    else            return (p - 2) / 3;
}

int main(void) {
    int n;
    printf("=== Security Switches ===\n");
    printf("Enter number of switches (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input.\n"); return 1; }

    long long formula = minMovesFormula(n);
    printf("\nMinimum moves (closed-form formula) : %lld\n", formula);

    if (n <= 16) {
        int *moves = malloc(sizeof(int) * (1 << n));
        int len = 0;
        long long bfs = solveBFS(n, moves, &len);
        printf("Minimum moves (exact BFS search)    : %lld\n", bfs);
        printf("Formula %s BFS result.\n", (bfs == formula) ? "MATCHES" : "DOES NOT MATCH");

        printf("\nExplicit legal move sequence (switch toggled each step):\n");
        for (int i = 0; i < len; i++) {
            printf("%d", moves[i]);
            printf(i == len - 1 ? "\n" : ", ");
        }
        free(moves);
    } else {
        printf("(exact BFS skipped for n > 16 - state space 2^n too large for this demo)\n");
    }

    return 0;
}
