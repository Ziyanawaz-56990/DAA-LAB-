/*
 * DAA Lab 11 - Question 2
 * Multi-Source BFS for Radius-Constrained Voronoi Cell Partitioning
 *
 * Idea:
 *   Put ALL hubs into the BFS queue at distance 0, in the order h1,h2,...,hK,
 *   and run ONE ordinary BFS. Every vertex is claimed by the first wave that
 *   reaches it. Because the queue keeps hubs in index order, ties at equal
 *   distance are automatically won by the smallest hub index.
 *   Vertices are not expanded beyond distance R, so anything farther than R
 *   stays unassigned (owner = -1).
 *
 * Input  (1-indexed vertices, undirected edges):
 *       n m K R
 *       h1 h2 ... hK
 *       u v          (m lines)
 * Output: for every vertex: owner hub index (or -1) and distance (or -1).
 *
 * Time  : O(V + E)      Space : O(V + E)
 */
#include <stdio.h>
#include <stdlib.h>

static int *head, *nxt, *to, edgeCount = 0;

static void addEdge(int u, int v) {
    to[edgeCount]  = v;
    nxt[edgeCount] = head[u];
    head[u]        = edgeCount++;
}

int main(void) {
    int n, m, K, R;
    if (scanf("%d %d %d %d", &n, &m, &K, &R) != 4) return 1;

    int *hub = malloc(K * sizeof(int));
    for (int i = 0; i < K; i++)
        if (scanf("%d", &hub[i]) != 1) return 1;

    head = malloc((n + 1) * sizeof(int));
    nxt  = malloc((2 * m + 1) * sizeof(int));
    to   = malloc((2 * m + 1) * sizeof(int));
    for (int i = 0; i <= n; i++) head[i] = -1;

    for (int i = 0; i < m; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) return 1;
        addEdge(u, v);
        addEdge(v, u);                    /* undirected: store both ways */
    }

    int *owner = malloc((n + 1) * sizeof(int));
    int *dist  = malloc((n + 1) * sizeof(int));
    int *queue = malloc((n + 1) * sizeof(int));
    for (int v = 0; v <= n; v++) owner[v] = dist[v] = -1;

    int qh = 0, qt = 0;
    for (int i = 0; i < K; i++) {         /* hubs enter in index order   */
        int h = hub[i];
        if (owner[h] == -1) {             /* duplicate hub: smaller index wins */
            owner[h] = i + 1;
            dist[h]  = 0;
            queue[qt++] = h;
        }
    }

    while (qh < qt) {
        int u = queue[qh++];
        if (dist[u] == R) continue;       /* radius reached: stop growing */
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            if (owner[v] == -1) {         /* first wave to arrive wins    */
                owner[v] = owner[u];
                dist[v]  = dist[u] + 1;
                queue[qt++] = v;
            }
        }
    }

    printf("Vertex  Owner  Distance\n");
    for (int v = 1; v <= n; v++)
        printf("%-7d %-6d %d\n", v, owner[v], dist[v]);

    free(hub); free(head); free(nxt); free(to);
    free(owner); free(dist); free(queue);
    return 0;
}
