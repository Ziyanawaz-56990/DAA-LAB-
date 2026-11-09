/*
 * DAA Lab 11 - Question 1
 * Shortest Path on Parity-Constrained Graphs via 0-1 BFS
 *
 * Idea:
 *   Cost(P) = W(P) + (C if k1(P) is odd else 0).
 *   Because every weight is 0 or 1, W(P) = k1(P). So the only thing that
 *   matters about a path is (its weight, parity of its weight).
 *   We therefore run 0-1 BFS on a "state graph" whose states are (v, p),
 *   p = parity (0 even / 1 odd) of the number of weight-1 edges used so far.
 *
 *       edge (u,v,0):  (u,p) -> (v,p)     cost 0
 *       edge (u,v,1):  (u,p) -> (v,1-p)   cost 1
 *
 *   Final answer for v:  min( dist[v][0] , dist[v][1] + C ).
 *
 * Input  (1-indexed vertices):
 *       n m S C
 *       u v w      (m lines, w is 0 or 1, directed edge u -> v)
 * Output: one line per vertex: its minimum Cost, or UNREACHABLE.
 *
 * Time  : O(V + E)      Space : O(V + E)
 */
#include <stdio.h>
#include <stdlib.h>

#define INF 4000000000000000000LL   /* "infinity" that never overflows */

/* ---------- graph stored as an adjacency list (forward-star) ---------- */
static int *head, *nxt, *to, *wt;   /* head[u] = first edge of u          */
static int edgeCount = 0;

static void addEdge(int u, int v, int w) {
    to[edgeCount]  = v;
    wt[edgeCount]  = w;
    nxt[edgeCount] = head[u];
    head[u]        = edgeCount++;
}

/* ---------- deque (array with the start placed in the middle) ---------- */
static int *buf, dqFront, dqBack;

static void pushFront(int x) { buf[--dqFront] = x; }
static void pushBack (int x) { buf[dqBack++]  = x; }
static int  popFront (void)  { return buf[dqFront++]; }
static int  dqEmpty  (void)  { return dqFront == dqBack; }

int main(void) {
    int n, m, S;
    long long C;
    if (scanf("%d %d %d %lld", &n, &m, &S, &C) != 4) return 1;

    head = malloc((n + 1) * sizeof(int));
    nxt  = malloc((m + 1) * sizeof(int));
    to   = malloc((m + 1) * sizeof(int));
    wt   = malloc((m + 1) * sizeof(int));
    for (int i = 0; i <= n; i++) head[i] = -1;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) return 1;
        addEdge(u, v, w);
    }

    /* state id of (v, p) is 2*v + p */
    int states = 2 * (n + 1);
    long long *dist = malloc(states * sizeof(long long));
    char *done = calloc(states, 1);
    for (int i = 0; i < states; i++) dist[i] = INF;

    /* each state is processed once, so at most 2m+1 pushes at each end */
    int cap = 2 * m + 2;
    buf = malloc(2 * cap * sizeof(int));
    dqFront = dqBack = cap;

    dist[2 * S + 0] = 0;                 /* empty path: weight 0, even    */
    pushBack(2 * S + 0);

    while (!dqEmpty()) {
        int s = popFront();
        if (done[s]) continue;           /* skip outdated duplicates      */
        done[s] = 1;
        int u = s / 2, p = s % 2;

        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e], w = wt[e];
            int t = 2 * v + (p ^ w);     /* weight-1 edge flips the parity */
            if (dist[s] + w < dist[t]) {
                dist[t] = dist[s] + w;
                if (w == 0) pushFront(t);
                else        pushBack(t);
            }
        }
    }

    printf("Vertex  MinCost\n");
    for (int v = 1; v <= n; v++) {
        long long even = dist[2 * v];
        long long odd  = (dist[2 * v + 1] >= INF) ? INF : dist[2 * v + 1] + C;
        long long best = even < odd ? even : odd;
        if (best >= INF) printf("%-7d UNREACHABLE\n", v);
        else             printf("%-7d %lld\n", v, best);
    }

    free(head); free(nxt); free(to); free(wt);
    free(dist); free(done); free(buf);
    return 0;
}
