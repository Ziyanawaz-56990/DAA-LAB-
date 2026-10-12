/*
 * Q10: Greedy Superstring (research-oriented experiment)   (DAA Lab 09)
 *
 * What this program DOES:
 *   - implements the classic greedy algorithm (merge the pair with maximum overlap),
 *   - computes the TRUE shortest superstring for small inputs (Held-Karp bitmask DP),
 *   - reports the ratio greedy/optimal on examples and on a random search.
 * What it does NOT do: it does not prove or disprove the conjecture. The 2026 counter-example
 * family claims ratio >= (9k+2)/(4k+4) for even k >= 10 with specially constructed instances; searching tiny random
 * inputs is not expected to find ratios above 2 (it is a sanity/insight tool only).
 *
 * Input representation: array of C strings (n <= 12, each length <= 40).
 * Compile: gcc -O2 -Wall -o q10 program.c        Run: ./q10
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 12
#define MAXL 400

/* longest suffix of a that is a prefix of b (a != b, proper overlap) */
static int overlap(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b), m = la < lb ? la : lb;
    for (int k = m; k > 0; k--) if (strncmp(a + la - k, b, k) == 0) return k;
    return 0;
}
/* drop duplicates and strings contained in another string */
static int reduce(char in[][MAXL], int n, char out[][MAXL]) {
    int m = 0;
    for (int i = 0; i < n; i++) {
        int drop = 0;
        for (int j = 0; j < n && !drop; j++) {
            if (i == j) continue;
            if (strstr(in[j], in[i]) && (strlen(in[j]) > strlen(in[i]) || j < i)) drop = 1;
        }
        if (!drop) strcpy(out[m++], in[i]);
    }
    return m;
}
/* Greedy: repeatedly merge the pair (i != j) with maximum overlap (ties: smallest i, then j). O(n^2 * L * n) naive */
static int greedy(char s[][MAXL], int n, char *result) {
    char cur[MAXN][MAXL] = {{0}}; for (int i = 0; i < n; i++) strcpy(cur[i], s[i]);
    int m = n;
    while (m > 1) {
        int bi = 0, bj = 1, bo = -1;
        for (int i = 0; i < m; i++) for (int j = 0; j < m; j++) if (i != j) {
            int o = overlap(cur[i], cur[j]); if (o > bo) { bo = o; bi = i; bj = j; } }
        char merged[MAXL]; strcpy(merged, cur[bi]); strcat(merged, cur[bj] + bo);
        strcpy(cur[bi], merged);
        if (bj != m - 1) strcpy(cur[bj], cur[m-1]);   /* move last string into the freed slot */
        m--;
    }
    strcpy(result, cur[0]); return strlen(cur[0]);
}
/* Optimal length via Held-Karp: dp[mask][last] = max total overlap of an ordering of 'mask' ending in 'last'. O(2^n n^2) */
static int optimal(char s[][MAXL], int n) {
    static int ov[MAXN][MAXN], dp[1 << MAXN][MAXN]; int total = 0;
    for (int i = 0; i < n; i++) { total += strlen(s[i]); for (int j = 0; j < n; j++) ov[i][j] = i == j ? 0 : overlap(s[i], s[j]); }
    for (int m = 0; m < (1 << n); m++) for (int i = 0; i < n; i++) dp[m][i] = -1;
    for (int i = 0; i < n; i++) dp[1 << i][i] = 0;
    for (int m = 1; m < (1 << n); m++) for (int i = 0; i < n; i++) if (dp[m][i] >= 0)
        for (int j = 0; j < n; j++) if (!(m >> j & 1)) { int v = dp[m][i] + ov[i][j]; if (v > dp[m | 1 << j][j]) dp[m | 1 << j][j] = v; }
    int best = 0; for (int i = 0; i < n; i++) if (dp[(1 << n) - 1][i] > best) best = dp[(1 << n) - 1][i];
    return total - best;
}
static double run(const char *name, const char *in[], int n0, int verbose) {
    char raw[MAXN][MAXL], s[MAXN][MAXL], res[MAXL];
    for (int i = 0; i < n0; i++) strcpy(raw[i], in[i]);
    int n = reduce(raw, n0, s), g = greedy(s, n, res), o = optimal(s, n);
    if (verbose) {
        printf("\n--- %s ---\nStrings:", name); for (int i = 0; i < n0; i++) printf(" %s", in[i]);
        printf("\nGreedy superstring : %s (length %d)\nOptimal length     : %d\nRatio greedy/optimal = %.4f\n", res, g, o, (double)g / o);
        int ok = 1; for (int i = 0; i < n0; i++) if (!strstr(res, in[i])) ok = 0;
        printf("Greedy result contains every input string: %s\n", ok ? "yes" : "NO");
    }
    return (double)g / o;
}
int main(void) {
    const char *t1[] = {"abc", "bcd", "cde"}; run("Test 1: chain (greedy optimal)", t1, 3, 1);
    const char *t2[] = {"cabab", "ababc", "baba"}; run("Test 2: classic c(ab)^k, (ab)^k c, (ba)^k with k=2", t2, 3, 1);
    const char *t3[] = {"cababab", "abababc", "bababa"}; run("Test 3: same family, k=3", t3, 3, 1);
    const char *t4[] = {"ate", "half", "lethal", "alpha", "alfalfa"}; run("Test 4: words (some overlap, some not)", t4, 5, 1);
    const char *t5[] = {"abc", "b", "xbcy"}; run("Test 5: substring removal ('b' and 'abc' handled)", t5, 3, 1);

    /* random search for the worst ratio on tiny inputs */
    srand(2026); double worst = 1; char w[MAXN][MAXL]; int wn = 0; char *ptrs[MAXN]; (void)ptrs;
    const char *pool[MAXN]; static char store[MAXN][MAXL];
    for (int it = 0; it < 20000; it++) {
        int n = 3 + rand() % 4;
        for (int i = 0; i < n; i++) { int L = 2 + rand() % 5; for (int k = 0; k < L; k++) store[i][k] = 'a' + rand() % 2; store[i][L] = 0; pool[i] = store[i]; }
        double r = run("", pool, n, 0);
        if (r > worst + 1e-12) { worst = r; wn = n; for (int i = 0; i < n; i++) strcpy(w[i], store[i]); }
    }
    printf("\n=== Random search (20000 tiny instances over {a,b}) ===\nWorst ratio found = %.4f on n=%d strings:", worst, wn);
    for (int i = 0; i < wn; i++) printf(" %s", w[i]);
    printf("\n(The conjecture said <= 2; tiny instances stay far below 2 and below 9/4.)\n");

    printf("\n=== Ratio lower bound claimed by the Sept-2026 preprint: (9k+2)/(4k+4), even k >= 10 (UNVERIFIED claim) ===\n");
    for (int k = 10; k <= 1000; k = (k < 20 ? k + 2 : k * 5)) printf("k = %4d  ->  %.4f\n", k, (9.0 * k + 2) / (4.0 * k + 4));
    printf("limit k -> infinity = 9/4 = 2.2500\n");
    return 0;
}
