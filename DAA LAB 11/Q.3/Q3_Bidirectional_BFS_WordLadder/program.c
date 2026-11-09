/*
 * DAA Lab 11 - Question 3
 * Bidirectional BFS for Shortest Transformation Sequence in a Word Graph
 *
 * Idea:
 *   Words are vertices; two words are joined if they differ in exactly one
 *   letter. The graph is never built explicitly. Instead, the neighbours of a
 *   word are found by changing each position to each of the 25 other letters
 *   and looking the result up in a hash table of the dictionary.
 *   BFS is run from A and from B at the same time, always expanding the
 *   smaller frontier one full level; the first time the two searches touch,
 *   the answer is (depth on one side) + 1 + (depth on the other side).
 *
 * Input:
 *       N L
 *       A B
 *       w1 w2 ... wN       (N dictionary words)
 * Output: the minimum number of mutations k, or -1 if impossible.
 *
 * Time  : O(N * L^2 * 26)  worst case     Space : O(N * L)
 */
#include <stdio.h>
#include <string.h>

#define MAXN   100000
#define MAXL   10
#define TSIZE  (1 << 18)                 /* hash table size, > 2 * MAXN */

static char words[MAXN + 2][MAXL + 2];   /* dictionary (+ A if missing) */
static int  table[TSIZE];                /* stores word index + 1, 0 = empty */
static int  total = 0;                   /* number of words stored      */
static int  L;

/* ---------- hash table of words ---------- */
static unsigned hashWord(const char *s) {
    unsigned h = 2166136261u;            /* FNV-1a */
    for (int i = 0; i < L; i++) { h ^= (unsigned char)s[i]; h *= 16777619u; }
    return h & (TSIZE - 1);
}

static int lookup(const char *s) {       /* index of s, or -1           */
    unsigned h = hashWord(s);
    while (table[h]) {
        if (strncmp(words[table[h] - 1], s, L) == 0) return table[h] - 1;
        h = (h + 1) & (TSIZE - 1);
    }
    return -1;
}

static int insertWord(const char *s) {   /* returns index of s          */
    int id = lookup(s);
    if (id != -1) return id;
    strncpy(words[total], s, L);
    words[total][L] = '\0';
    unsigned h = hashWord(s);
    while (table[h]) h = (h + 1) & (TSIZE - 1);
    table[h] = total + 1;
    return total++;
}

/* ---------- bidirectional BFS ---------- */
static int dist[2][MAXN + 2];            /* dist[0]: from A, dist[1]: from B */
static int front[2][MAXN + 2], fsz[2];
static int nextLevel[MAXN + 2];

static int bidirectionalBFS(int src, int dst) {
    if (src == dst) return 0;
    for (int s = 0; s < 2; s++)
        for (int i = 0; i < total; i++) dist[s][i] = -1;

    dist[0][src] = 0; front[0][0] = src; fsz[0] = 1;
    dist[1][dst] = 0; front[1][0] = dst; fsz[1] = 1;

    while (fsz[0] > 0 && fsz[1] > 0) {
        int side  = (fsz[0] <= fsz[1]) ? 0 : 1;   /* expand smaller side */
        int other = 1 - side;
        int nsz = 0;

        for (int k = 0; k < fsz[side]; k++) {
            int u = front[side][k];
            char cand[MAXL + 2];
            strncpy(cand, words[u], L);
            cand[L] = '\0';

            for (int i = 0; i < L; i++) {
                char orig = cand[i];
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == orig) continue;
                    cand[i] = c;
                    int v = lookup(cand);
                    if (v == -1 || dist[side][v] != -1) continue;
                    if (dist[other][v] != -1)          /* the searches met */
                        return dist[side][u] + 1 + dist[other][v];
                    dist[side][v] = dist[side][u] + 1;
                    nextLevel[nsz++] = v;
                }
                cand[i] = orig;
            }
        }
        memcpy(front[side], nextLevel, nsz * sizeof(int));
        fsz[side] = nsz;
    }
    return -1;                            /* one side ran out of words   */
}

int main(void) {
    int N;
    char A[MAXL + 2], B[MAXL + 2], w[MAXL + 2];
    if (scanf("%d %d", &N, &L) != 2) return 1;
    if (scanf("%10s %10s", A, B) != 2) return 1;

    for (int i = 0; i < N; i++) {
        if (scanf("%10s", w) != 1) return 1;
        insertWord(w);
    }

    int dst = lookup(B);
    if (dst == -1) {                      /* B must be in the dictionary */
        printf("-1\n");
        return 0;
    }
    int src = insertWord(A);              /* A need not be in D          */
    printf("%d\n", bidirectionalBFS(src, dst));
    return 0;
}
