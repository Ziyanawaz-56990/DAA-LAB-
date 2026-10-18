/*
 * DAA Lab-10 | Q4 : Reverse Time-Step DSU Application
 * -----------------------------------------------------------------------
 * Idea      : DSU supports UNION but not DELETE.  So run time BACKWARDS:
 *             start with every vertex deleted and ADD them in the order
 *             p_V, p_{V-1}, ..., p_1.  The answer "after i deletions" equals
 *             the best component sum when p_{i+1..V} are present.
 * Input rep : Adjacency list (forward-star / linked arrays) - we must
 *             enumerate the neighbours of the vertex being re-inserted.
 * Time      : O((V + E) * alpha(V))
 * Space     : O(V + E)
 * Assumption: vertex values are NON-NEGATIVE (so a merged component is never
 *             worse than its parts and a running maximum suffices).
 *
 * Input  : V E
 *          w1 ... wV
 *          u1 v1 ... (E lines)   (1-indexed)
 *          p1 ... pV             (deletion order, a permutation of 1..V)
 * Output : max component sum after each deletion.
 *
 * Compile: gcc -O2 -Wall -o q4 solution.c
 * Run    : ./q4 < sample_input.txt
 */
#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

static int *parent, *sz;
static ll  *sum;

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
    parent[b] = a; sz[a] += sz[b]; sum[a] += sum[b];
}

int main(void)
{
    int V, E;
    if (scanf("%d %d", &V, &E) != 2 || V < 1 || E < 0) {
        fprintf(stderr, "Invalid input. Expected: V E, weights, edges, deletion order\n");
        return 1;
    }
    ll *w = malloc((V + 1) * sizeof *w);
    int *head = malloc((V + 1) * sizeof *head);
    int *nxt  = malloc(2 * (size_t)(E + 1) * sizeof *nxt);
    int *to   = malloc(2 * (size_t)(E + 1) * sizeof *to);
    int *P    = malloc(V * sizeof *P);
    unsigned char *active = calloc(V + 1, 1), *seen = calloc(V + 1, 1);
    ll *ans = malloc(V * sizeof *ans);
    parent = malloc((V + 1) * sizeof *parent);
    sz = malloc((V + 1) * sizeof *sz);
    sum = malloc((V + 1) * sizeof *sum);
    if (!w || !head || !nxt || !to || !P || !active || !seen || !ans || !parent || !sz || !sum) {
        fprintf(stderr, "Out of memory\n"); return 1;
    }

    for (int i = 1; i <= V; i++) {
        if (scanf("%lld", &w[i]) != 1) { fprintf(stderr, "Weight %d missing\n", i); return 1; }
        if (w[i] < 0) { fprintf(stderr, "Error: this solution assumes non-negative vertex values\n"); return 1; }
        head[i] = -1;
    }
    int cnt = 0;                                     /* adjacency list build */
    for (int i = 0; i < E; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > V || v < 1 || v > V) {
            fprintf(stderr, "Bad edge %d\n", i + 1); return 1;
        }
        to[cnt] = v; nxt[cnt] = head[u]; head[u] = cnt++;
        to[cnt] = u; nxt[cnt] = head[v]; head[v] = cnt++;
    }
    for (int i = 0; i < V; i++) {
        if (scanf("%d", &P[i]) != 1 || P[i] < 1 || P[i] > V || seen[P[i]]) {
            fprintf(stderr, "Deletion order must be a permutation of 1..V\n"); return 1;
        }
        seen[P[i]] = 1;
    }

    ll best = 0;                                      /* empty graph => 0 */
    for (int i = V - 1; i >= 0; i--) {
        ans[i] = best;                                /* state after i+1 deletions */
        int v = P[i];
        active[v] = 1; parent[v] = v; sz[v] = 1; sum[v] = w[v];
        for (int e = head[v]; e != -1; e = nxt[e])
            if (active[to[e]]) unite(v, to[e]);
        ll s = sum[find(v)];
        if (s > best) best = s;
    }
    for (int i = 0; i < V; i++)
        printf("After deleting vertex %d: max component sum = %lld\n", P[i], ans[i]);

    free(w); free(head); free(nxt); free(to); free(P); free(active); free(seen);
    free(ans); free(parent); free(sz); free(sum);
    return 0;
}
