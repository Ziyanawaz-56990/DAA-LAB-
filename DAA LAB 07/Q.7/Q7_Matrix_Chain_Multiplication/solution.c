/*
 * Q7 - Matrix Chain Multiplication (MCM)
 * -------------------------------------------------------------------
 * Given dimensions p[0..n] for a chain of n matrices (matrix i has
 * dimensions p[i-1] x p[i]), find the minimum number of scalar
 * multiplications needed to compute the full product, and the
 * corresponding optimal parenthesisation.
 *
 * Compile : gcc -O2 -Wall -o solution solution.c
 * Run     : ./solution        (prompts for n and the n+1 dimensions)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int **m, **s;

void printOptimalParens(int i, int j) {
    if (i == j) {
        printf("A%d", i);
    } else {
        printf("(");
        printOptimalParens(i, s[i][j]);
        printOptimalParens(s[i][j] + 1, j);
        printf(")");
    }
}

int main(void) {
    int n;
    printf("=== Matrix Chain Multiplication ===\n");
    printf("Enter number of matrices (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input.\n"); return 1; }

    int *p = malloc(sizeof(int) * (n + 1));
    printf("Enter %d dimensions p[0..%d]\n", n + 1, n);
    printf("(matrix i has dimensions p[i-1] x p[i]):\n");
    for (int i = 0; i <= n; i++) {
        printf("  p[%d]: ", i);
        if (scanf("%d", &p[i]) != 1 || p[i] <= 0) {
            printf("Invalid input.\n");
            free(p);
            return 1;
        }
    }

    m = malloc(sizeof(int *) * (n + 1));
    s = malloc(sizeof(int *) * (n + 1));
    for (int i = 0; i <= n; i++) {
        m[i] = calloc(n + 1, sizeof(int));
        s[i] = calloc(n + 1, sizeof(int));
    }

    for (int i = 1; i <= n; i++) m[i][i] = 0;

    for (int L = 2; L <= n; L++) {
        for (int i = 1; i <= n - L + 1; i++) {
            int j = i + L - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                long long cost = (long long)m[i][k] + m[k + 1][j]
                                  + (long long)p[i - 1] * p[k] * p[j];
                if (cost < m[i][j]) {
                    m[i][j] = (int)cost;
                    s[i][j] = k;
                }
            }
        }
    }

    printf("\nMinimum number of scalar multiplications: %d\n", m[1][n]);
    printf("Optimal parenthesisation: ");
    printOptimalParens(1, n);
    printf("\n");

    /* Classic textbook instance check: p = {30,35,15,5,10,20,25} -> 15125 */
    if (n == 6 && p[0]==30 && p[1]==35 && p[2]==15 && p[3]==5 &&
        p[4]==10 && p[5]==20 && p[6]==25) {
        printf("(Classic textbook instance: expected minimum is 15125)\n");
    }

    for (int i = 0; i <= n; i++) { free(m[i]); free(s[i]); }
    free(m); free(s); free(p);
    return 0;
}
