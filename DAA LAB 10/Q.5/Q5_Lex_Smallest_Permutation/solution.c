/*
 * DAA Lab-10 | Q5 : Lexicographically Smallest Permutation via Allowed Swaps
 * -----------------------------------------------------------------------
 * Key fact  : positions joined by a chain of allowed swaps form a connected
 *             component, and inside a component ANY rearrangement of its
 *             values is reachable.  Smallest result = sort the values of each
 *             component and place them on its sorted positions.
 * Technique : DSU for components + counting/bucket placement (no comparison
 *             sort needed because A is a permutation of 1..n).
 * Input rep : Edge list (u, v) of POSITIONS; only used to build the DSU.
 * Time      : O((n + m) * alpha(n))  (linear apart from the alpha factor)
 * Space     : O(n)
 *
 * Input  : n m
 *          A1 ... An          (permutation of 1..n)
 *          u1 v1 ... (m lines, positions, 1-indexed)
 * Output : lexicographically smallest reachable permutation.
 *
 * Compile: gcc -O2 -Wall -o q5 solution.c
 * Run    : ./q5 < sample_input.txt
 */
#include <stdio.h>
#include <stdlib.h>

static int *parent, *sz;

static int find(int x)
{
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}
static void unite(int a, int b)
{
    a = find(a); b = find(b);
    if (a == b) return;
    if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
    parent[b] = a; sz[a] += sz[b];
}

int main(void)
{
    int n, m;
    if (scanf("%d %d", &n, &m) != 2 || n < 1 || m < 0) {
        fprintf(stderr, "Invalid input. Expected: n m, permutation, then m swaps\n");
        return 1;
    }
    int *A = malloc((n + 1) * sizeof *A), *posOf = malloc((n + 1) * sizeof *posOf);
    int *fillP = malloc((n + 1) * sizeof *fillP);
    int *fillV = malloc((n + 1) * sizeof *fillV);
    int *posList = malloc(n * sizeof *posList), *valList = malloc(n * sizeof *valList);
    int *res = malloc((n + 1) * sizeof *res);
    unsigned char *seen = calloc(n + 1, 1);
    parent = malloc((n + 1) * sizeof *parent); sz = malloc((n + 1) * sizeof *sz);
    if (!A || !posOf || !fillP || !fillV || !posList || !valList || !res || !seen || !parent || !sz) {
        fprintf(stderr, "Out of memory\n"); return 1;
    }

    for (int i = 1; i <= n; i++) {
        if (scanf("%d", &A[i]) != 1 || A[i] < 1 || A[i] > n || seen[A[i]]) {
            fprintf(stderr, "A must be a permutation of 1..n\n"); return 1;
        }
        seen[A[i]] = 1; posOf[A[i]] = i;
        parent[i] = i; sz[i] = 1;
    }
    for (int i = 0; i < m; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n || v < 1 || v > n) {
            fprintf(stderr, "Bad swap %d\n", i + 1); return 1;
        }
        unite(u, v);
    }

    /* Bucket layout: component with root r owns slots [off, off+sz[r]) */
    int off = 0;
    for (int i = 1; i <= n; i++)
        if (find(i) == i) { fillP[i] = fillV[i] = off; off += sz[i]; }

    for (int i = 1; i <= n; i++)                 /* positions, ascending  */
        { int r = find(i); posList[fillP[r]++] = i; }
    for (int v = 1; v <= n; v++)                 /* values, ascending     */
        { int r = find(posOf[v]); valList[fillV[r]++] = v; }

    for (int k = 0; k < n; k++) res[posList[k]] = valList[k];

    printf("Smallest permutation:");
    for (int i = 1; i <= n; i++) printf(" %d", res[i]);
    printf("\n");

    free(A); free(posOf); free(fillP); free(fillV); free(posList);
    free(valList); free(res); free(seen); free(parent); free(sz);
    return 0;
}
