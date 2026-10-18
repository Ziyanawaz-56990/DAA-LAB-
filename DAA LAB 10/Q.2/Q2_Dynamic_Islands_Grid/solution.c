/*
 * DAA Lab-10 | Q2 : Dynamic Connected Components in a Grid (Number of Islands II)
 * -----------------------------------------------------------------------
 * Technique : DSU over FLATTENED coordinates  id = r*C + c
 * Input rep : Implicit grid graph (no adjacency list needed - neighbours are
 *             the 4 cells (r+-1,c),(r,c+-1)); parent[] array of size R*C.
 * Time      : O(R*C) initialisation + O(K * alpha(R*C)) for K operations
 * Space     : O(R*C)
 *
 * Input  : R C K
 *          r1 c1
 *          ...      (0-indexed cell coordinates)
 * Output : number of islands after each operation.
 *
 * Compile: gcc -O2 -Wall -o q2 solution.c
 * Run    : ./q2 < sample_input.txt
 */
#include <stdio.h>
#include <stdlib.h>

static int R, C;
static int *parent;   /* -1 => water, otherwise DSU parent (land)  */
static int *sz;

/* iterative find with path halving (no recursion => safe for big grids) */
static int find(int x)
{
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

/* returns 1 if two different components were merged */
static int unite(int a, int b)
{
    a = find(a); b = find(b);
    if (a == b) return 0;
    if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
    parent[b] = a;
    sz[a] += sz[b];
    return 1;
}

int main(void)
{
    int K;
    if (scanf("%d %d %d", &R, &C, &K) != 3 || R < 1 || C < 1 || K < 0) {
        fprintf(stderr, "Invalid input. Expected: R C K followed by K positions\n");
        return 1;
    }
    long long cells = (long long)R * C;
    parent = malloc(cells * sizeof *parent);
    sz     = calloc(cells, sizeof *sz);
    if (!parent || !sz) { fprintf(stderr, "Out of memory\n"); return 1; }
    for (long long i = 0; i < cells; i++) parent[i] = -1;       /* all water */

    const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    int islands = 0;

    for (int i = 1; i <= K; i++) {
        int r, c;
        if (scanf("%d %d", &r, &c) != 2) { fprintf(stderr, "Operation %d missing\n", i); return 1; }
        const char *note = "";
        if (r < 0 || r >= R || c < 0 || c >= C) {
            note = " (ignored: out of range)";
        } else {
            int id = r * C + c;
            if (parent[id] != -1) {
                note = " (already land)";
            } else {
                parent[id] = id; sz[id] = 1; islands++;          /* new island */
                for (int d = 0; d < 4; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
                    int nid = nr * C + nc;
                    if (parent[nid] != -1 && unite(id, nid)) islands--;  /* merged */
                }
            }
        }
        printf("After op %d: land (%d,%d) -> islands = %d%s\n", i, r, c, islands, note);
    }

    if (cells <= 400) {
        printf("Final grid ('#' land, '.' water):\n");
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) putchar(parent[r * C + c] != -1 ? '#' : '.');
            putchar('\n');
        }
    }
    free(parent); free(sz);
    return 0;
}
