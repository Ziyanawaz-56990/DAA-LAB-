/*
 * Q9: Hu-Tucker algorithm (optimal alphabetic binary tree)   (DAA Lab 09)
 * Input: weights w[0..n-1] in FIXED left-to-right order (leaf order must be kept).
 * Minimises sum( w[i] * depth(i) ).
 *
 * Phase 1 (combination): leaves are "squares", merged nodes are "circles".
 *    Two nodes are COMPATIBLE if no square lies strictly between them.
 *    Repeatedly merge the compatible pair with the smallest weight sum
 *    (ties: leftmost).  The merged circle takes the position of the left node.
 * Phase 2 (levels): the depth of every leaf in that merge tree is its level.
 * Phase 3 (recombination): rebuild an ORDER-PRESERVING tree with the same levels (stack method)
 *    and read off alphabetic prefix codes.
 * Time O(n^3) as written (O(n^2) pair scans x n merges); O(n log n) is possible with fancier data structures.
 * Validation: compared with the classic O(n^3) dynamic programme for optimal alphabetic trees.
 * Compile: gcc -O2 -Wall -o q09 program.c        Run: ./q09
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 16
typedef long long ll;

static int lc[2 * MAXN], rc[2 * MAXN];

static void dfs(int v, int d, int n, int level[]) {
    if (v < n) { level[v] = d; return; }
    dfs(lc[v], d + 1, n, level); dfs(rc[v], d + 1, n, level);
}

/* Phase 1 + 2.  Returns levels[] */
static void hu_tucker_levels(const ll w[], int n, int level[]) {
    ll wt[MAXN]; int sq[MAXN], id[MAXN], m = n, next = n;
    for (int i = 0; i < n; i++) { wt[i] = w[i]; sq[i] = 1; id[i] = i; }
    if (n == 1) { level[0] = 0; return; }
    while (m > 1) {
        int bi = -1, bj = -1; ll bs = 0;
        for (int i = 0; i < m - 1; i++)
            for (int j = i + 1; j < m; j++) {
                if (bi < 0 || wt[i] + wt[j] < bs) { bi = i; bj = j; bs = wt[i] + wt[j]; }
                if (sq[j]) break;                      /* a square blocks every farther partner */
            }
        lc[next] = id[bi]; rc[next] = id[bj];
        wt[bi] = bs; sq[bi] = 0; id[bi] = next++;       /* circle at position of left node */
        for (int k = bj; k < m - 1; k++) { wt[k] = wt[k+1]; sq[k] = sq[k+1]; id[k] = id[k+1]; }
        m--;
    }
    dfs(id[0], 0, n, level);
}

/* Phase 3: stack check + alphabetic codes from the levels (code_{i+1} = (code_i + 1) adjusted to new length) */
static int build_codes(const int level[], int n, unsigned long code[]) {
    int st[MAXN], top = 0;                              /* stack of levels: merge equal tops */
    for (int i = 0; i < n; i++) { st[top++] = level[i];
        while (top >= 2 && st[top-1] == st[top-2]) { st[top-2]--; top--; } }
    int valid = (top == 1 && st[0] == 0);
    code[0] = 0;
    for (int i = 1; i < n; i++) {
        unsigned long c = code[i-1] + 1;
        if (level[i] >= level[i-1]) c <<= (level[i] - level[i-1]); else c >>= (level[i-1] - level[i]);
        code[i] = c;
    }
    return valid;
}

/* Reference optimum: dp[i][j] = W(i..j) + min_k dp[i][k] + dp[k+1][j] */
static ll optimal_dp(const ll w[], int n) {
    ll dp[MAXN][MAXN], pre[MAXN + 1] = {0};
    for (int i = 0; i < n; i++) pre[i+1] = pre[i] + w[i];
    for (int i = 0; i < n; i++) dp[i][i] = 0;
    for (int len = 2; len <= n; len++) for (int i = 0; i + len - 1 < n; i++) { int j = i + len - 1; ll b = -1;
        for (int k = i; k < j; k++) { ll c = dp[i][k] + dp[k+1][j]; if (b < 0 || c < b) b = c; }
        dp[i][j] = b + pre[j+1] - pre[i]; }
    return dp[0][n-1];
}
static ll cost_of(const ll w[], const int lv[], int n) { ll c = 0; for (int i = 0; i < n; i++) c += w[i] * lv[i]; return c; }

static void run(const char *name, ll w[], int n) {
    int lv[MAXN]; unsigned long code[MAXN];
    hu_tucker_levels(w, n, lv); int valid = build_codes(lv, n, code);
    printf("\n--- %s ---\nLeaf  weight  depth  alphabetic code\n", name);
    for (int i = 0; i < n; i++) { char b[40]; int L = lv[i];
        if (n == 1) { printf("  %d   %5lld   %3d    (root)\n", i + 1, w[i], 0); continue; }
        for (int k = 0; k < L; k++)
            b[k] = (code[i] >> (L - 1 - k) & 1) ? '1' : '0';
        b[L] = 0;
        printf("  %d   %5lld   %3d    %s\n", i + 1, w[i], lv[i], b); }
    ll c = cost_of(w, lv, n), o = optimal_dp(w, n);
    printf("Hu-Tucker cost = %lld | DP optimum = %lld -> %s | leaves form a valid ordered tree: %s\n",
           c, o, c == o ? "MATCH" : "MISMATCH", valid ? "yes" : "NO");
}
int main(void) {
    ll a[] = {3, 1, 2, 5, 4}; run("Test 1", a, 5);
    ll b[] = {10, 1, 1, 10}; run("Test 2: light leaves in the middle", b, 4);
    ll c[] = {1, 1, 1, 1, 1, 1, 1, 1}; run("Test 3: equal weights", c, 8);
    ll d[] = {40, 5, 3, 7, 30, 15}; run("Test 4", d, 6);
    ll e[] = {5}; run("Test 5: single leaf", e, 1);
    srand(2026); int ok = 0, tot = 3000;
    for (int t = 0; t < tot; t++) { int n = 1 + rand() % 10; ll r[MAXN]; int lv[MAXN]; unsigned long cd[MAXN];
        for (int i = 0; i < n; i++) r[i] = 1 + rand() % 12;
        hu_tucker_levels(r, n, lv);
        if (cost_of(r, lv, n) == optimal_dp(r, n) && build_codes(lv, n, cd)) ok++; }
    printf("\n=== Random validation: %d/%d cases Hu-Tucker cost == DP optimum (and valid tree) ===\n", ok, tot);
    return 0;
}
