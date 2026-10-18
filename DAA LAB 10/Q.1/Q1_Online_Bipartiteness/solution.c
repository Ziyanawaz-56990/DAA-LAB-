/*
 * DAA Lab-10 | Q1 : Online Graph-Bipartiteness Testing
 * -----------------------------------------------------------------------
 * Technique : Disjoint Set Union (DSU) with PARITY (a.k.a. "DSU with
 *             bipartite / 2-colouring information").
 * Input rep : Edge stream (u, v) read one at a time. No adjacency matrix
 *             or list is stored - the DSU forest is the only structure.
 * Time      : O(alpha(V)) amortised per edge  ->  O(V + E*alpha(V)) total
 * Space     : O(V)
 *
 * Input  : V E
 *          u1 v1
 *          ...            (vertices are 1-indexed)
 * Output : verdict for every arriving edge + final 2-colouring.
 *
 * Compile: gcc -O2 -Wall -o q1 solution.c
 * Run    : ./q1 < sample_input.txt
 */
#include <stdio.h>
#include <stdlib.h>

static int *parent;            /* DSU parent                               */
static int *sz;                /* size of tree rooted at index (for union) */
static unsigned char *parity;  /* parity[x] = colour(x) XOR colour(parent) */

/* find with path compression; also compresses the parity so that after
 * the call parity[x] is the colour of x RELATIVE TO ITS ROOT. */
static int find(int x)
{
    if (parent[x] == x) return x;
    int p = parent[x];
    int r = find(p);          /* afterwards parity[p] is relative to r     */
    parity[x] ^= parity[p];   /* x relative to p  XOR  p relative to r     */
    parent[x] = r;
    return r;
}

/* Try to add edge (u,v). Returns 1 if accepted, 0 if it would create an
 * odd cycle (graph is left untouched in that case). */
static int add_edge(int u, int v)
{
    int ru = find(u), rv = find(v);
    if (ru == rv)                         /* same component              */
        return parity[u] != parity[v];    /* equal colours => odd cycle  */

    /* different components: merge so that colour(u) != colour(v).        */
    if (sz[ru] < sz[rv]) { int t = ru; ru = rv; rv = t; }   /* ru bigger */
    parent[rv] = ru;
    parity[rv] = parity[u] ^ parity[v] ^ 1;
    sz[ru] += sz[rv];
    return 1;
}

int main(void)
{
    int V, E;
    if (scanf("%d %d", &V, &E) != 2 || V < 1 || E < 0) {
        fprintf(stderr, "Invalid input. Expected: V E followed by E edges\n");
        return 1;
    }
    parent = malloc((V + 1) * sizeof *parent);
    sz     = malloc((V + 1) * sizeof *sz);
    parity = calloc(V + 1, sizeof *parity);
    if (!parent || !sz || !parity) { fprintf(stderr, "Out of memory\n"); return 1; }
    for (int i = 1; i <= V; i++) { parent[i] = i; sz[i] = 1; }

    int accepted = 0, rejected = 0;
    for (int i = 1; i <= E; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) { fprintf(stderr, "Edge %d missing\n", i); return 1; }
        if (u < 1 || u > V || v < 1 || v > V) {
            printf("Edge %d: (%d, %d) -> IGNORED (vertex out of range)\n", i, u, v);
            continue;
        }
        if (add_edge(u, v)) { accepted++; printf("Edge %d: (%d, %d) -> ACCEPTED\n", i, u, v); }
        else { rejected++; printf("Edge %d: (%d, %d) -> REJECTED (odd cycle)\n", i, u, v); }
    }
    printf("Summary: accepted = %d, rejected = %d\n", accepted, rejected);

    if (V <= 100) {
        printf("Colouring (0/1 relative to component root):");
        for (int i = 1; i <= V; i++) { find(i); printf(" %d:%d", i, parity[i]); }
        printf("\n");
    }
    free(parent); free(sz); free(parity);
    return 0;
}
