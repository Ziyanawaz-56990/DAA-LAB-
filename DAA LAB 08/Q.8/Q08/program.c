/*
 * DAA Lab 08 - Q8: Optimal Binary Search Tree (OBST)
 *
 * INPUT FORMAT (standard input):
 *     n                     -> number of keys
 *     p1 p2 ... pn          -> probability of searching key k_i
 *     q0 q1 ... qn          -> probability for dummy key d_i (unsuccessful search)
 * The n + (n+1) probabilities should add up to 1.
 * OUTPUT: minimum expected search cost, the root table and the tree shape.
 *
 * Notation (same as CLRS):
 *   e[i][j] = expected cost of an optimal BST for keys k_i..k_j (with dummies d_{i-1}..d_j)
 *   w[i][j] = p_i + ... + p_j + q_{i-1} + ... + q_j   (sum of probabilities)
 *   root[i][j] = index r of the key chosen as root of that subtree
 * Base : e[i][i-1] = q_{i-1},  w[i][i-1] = q_{i-1}      (empty subtree = dummy key)
 * Rule : w[i][j] = w[i][j-1] + p_j + q_j
 *        e[i][j] = min over r = i..j of ( e[i][r-1] + e[r+1][j] + w[i][j] )
 * Tables are filled by increasing chain length l = 1..n.
 * Time: O(n^3)   Space: O(n^2)
 */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

/* allocate a (rows x cols) table of doubles filled with 0 */
static double **newDoubleTable(int rows, int cols)
{
    int i;
    double **t = (double **)malloc(rows * sizeof(double *));
    if (t == NULL) return NULL;
    for (i = 0; i < rows; i++) {
        t[i] = (double *)calloc(cols, sizeof(double));
        if (t[i] == NULL) return NULL;
    }
    return t;
}

/* print the tree in pre-order using the root table */
static void printTree(int **root, int i, int j, int parent, const char *side)
{
    int r;
    if (i > j) {
        printf("  d%d is the %s child of k%d\n", j, side, parent);
        return;
    }
    r = root[i][j];
    if (parent == 0) printf("  k%d is the root\n", r);
    else             printf("  k%d is the %s child of k%d\n", r, side, parent);
    printTree(root, i, r - 1, r, "left");
    printTree(root, r + 1, j, r, "right");
}

int main(void)
{
    int n, i, j, r, l;
    double *p, *q, **e, **w;
    int **root;

    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid n.\n");
        return 1;
    }
    p = (double *)calloc(n + 2, sizeof(double));
    q = (double *)calloc(n + 2, sizeof(double));
    if (p == NULL || q == NULL) return 1;
    for (i = 1; i <= n; i++) {
        if (scanf("%lf", &p[i]) != 1) { printf("Invalid input.\n"); return 1; }
    }
    for (i = 0; i <= n; i++) {
        if (scanf("%lf", &q[i]) != 1) { printf("Invalid input.\n"); return 1; }
    }

    e = newDoubleTable(n + 2, n + 2);
    w = newDoubleTable(n + 2, n + 2);
    root = (int **)malloc((n + 2) * sizeof(int *));
    if (e == NULL || w == NULL || root == NULL) return 1;
    for (i = 0; i < n + 2; i++) {
        root[i] = (int *)calloc(n + 2, sizeof(int));
        if (root[i] == NULL) return 1;
    }

    for (i = 1; i <= n + 1; i++) {          /* empty subtrees */
        e[i][i - 1] = q[i - 1];
        w[i][i - 1] = q[i - 1];
    }

    for (l = 1; l <= n; l++) {              /* chain length */
        for (i = 1; i <= n - l + 1; i++) {
            j = i + l - 1;
            e[i][j] = DBL_MAX;
            w[i][j] = w[i][j - 1] + p[j] + q[j];
            for (r = i; r <= j; r++) {
                double cost = e[i][r - 1] + e[r + 1][j] + w[i][j];
                if (cost < e[i][j]) {
                    e[i][j] = cost;
                    root[i][j] = r;
                }
            }
        }
    }

    printf("Number of keys n = %d\n", n);
    printf("Minimum expected search cost = %.4f\n", e[1][n]);
    printf("Root table (row i, column j gives root of keys k_i..k_j):\n");
    for (i = 1; i <= n; i++) {
        printf("  i=%d:", i);
        for (j = 1; j <= n; j++) {
            if (j < i) printf("   -");
            else       printf("  k%d", root[i][j]);
        }
        printf("\n");
    }
    printf("Structure of the optimal tree:\n");
    printTree(root, 1, n, 0, "");

    return 0;
}
