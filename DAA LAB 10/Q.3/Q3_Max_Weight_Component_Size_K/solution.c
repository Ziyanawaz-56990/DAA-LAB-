/*
 * DAA Lab-10 | Q3 : Maximum Weight Component with Size Constraint K
 * -----------------------------------------------------------------------
 * Technique : DSU storing (size, total vertex weight) per component
 *             + MAX-HEAP WITH LAZY DELETION of candidate components.
 * Input rep : Weight array w[1..V] + edge stream (u, v, wt). The edge weight
 *             never changes connectivity or vertex sums, so it is read and
 *             carried along but not needed by the algorithm.
 * Time      : O((V + E) * alpha(V) + V log V)
 * Space     : O(V)
 *
 * Input  : V K E
 *          w1 w2 ... wV
 *          u1 v1 wt1
 *          ...                (vertices are 1-indexed)
 * Output : max weight of a component of size <= K initially and after
 *          every edge ("NONE" if no component qualifies).
 *
 * Compile: gcc -O2 -Wall -o q3 solution.c
 * Run    : ./q3 < sample_input.txt
 */
#include <stdio.h>
#include <stdlib.h>

typedef long long ll;
typedef struct { ll sum; int root; int size; } Entry;

static int *parent, *sz;
static ll  *sum;
static int  K;
static Entry *heap; static int hn = 0;

static int find(int x)
{
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}

/* ---------- binary max-heap keyed on Entry.sum ---------- */
static void push(Entry e)
{
    int i = hn++;
    while (i > 0 && heap[(i - 1) / 2].sum < e.sum) { heap[i] = heap[(i - 1) / 2]; i = (i - 1) / 2; }
    heap[i] = e;
}
static void pop(void)
{
    Entry last = heap[--hn];
    int i = 0;
    while (2 * i + 1 < hn) {
        int c = 2 * i + 1;
        if (c + 1 < hn && heap[c + 1].sum > heap[c].sum) c++;
        if (heap[c].sum <= last.sum) break;
        heap[i] = heap[c]; i = c;
    }
    if (hn > 0) heap[i] = last;
}

/* an entry is still current iff its root is still a root and has not grown */
static int valid(Entry e)
{
    return parent[e.root] == e.root && sz[e.root] == e.size && e.size <= K;
}

/* returns 1 and sets *out if some component of size <= K exists */
static int query(ll *out)
{
    while (hn > 0 && !valid(heap[0])) pop();         /* lazy deletion */
    if (hn == 0) return 0;
    *out = heap[0].sum;
    return 1;
}

int main(void)
{
    int V, E;
    if (scanf("%d %d %d", &V, &K, &E) != 3 || V < 1 || E < 0) {
        fprintf(stderr, "Invalid input. Expected: V K E, weights, then E edges\n");
        return 1;
    }
    parent = malloc((V + 1) * sizeof *parent);
    sz     = malloc((V + 1) * sizeof *sz);
    sum    = malloc((V + 1) * sizeof *sum);
    heap   = malloc((2 * (size_t)V + 2) * sizeof *heap);   /* <= V + (V-1) pushes */
    if (!parent || !sz || !sum || !heap) { fprintf(stderr, "Out of memory\n"); return 1; }

    for (int i = 1; i <= V; i++) {
        if (scanf("%lld", &sum[i]) != 1) { fprintf(stderr, "Weight %d missing\n", i); return 1; }
        parent[i] = i; sz[i] = 1;
        if (1 <= K) push((Entry){sum[i], i, 1});
    }

    ll best;
    if (query(&best)) printf("Initially: max = %lld\n", best);
    else              printf("Initially: max = NONE\n");

    for (int i = 1; i <= E; i++) {
        int u, v; ll wt;
        if (scanf("%d %d %lld", &u, &v, &wt) != 3) { fprintf(stderr, "Edge %d missing\n", i); return 1; }
        if (u >= 1 && u <= V && v >= 1 && v <= V) {
            int a = find(u), b = find(v);
            if (a != b) {
                if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
                parent[b] = a;
                sz[a]  += sz[b];
                sum[a] += sum[b];
                if (sz[a] <= K) push((Entry){sum[a], a, sz[a]});
            }
        }
        if (query(&best)) printf("After edge %d (%d,%d,wt=%lld): max = %lld\n", i, u, v, wt, best);
        else              printf("After edge %d (%d,%d,wt=%lld): max = NONE\n", i, u, v, wt);
    }
    free(parent); free(sz); free(sum); free(heap);
    return 0;
}
