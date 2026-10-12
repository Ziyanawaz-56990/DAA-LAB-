/*
 * Q6: Reorganise string so equal characters are at least K apart   (DAA Lab 09)
 * Input: string S (chars as unsigned bytes, alphabet size 256) and integer K.
 * Greedy: at each position pick, among characters whose last use was >= K positions ago,
 *         the one with the largest remaining count (ties: smaller character).
 * Same as the "max-heap + waiting queue" version; the scan over a fixed alphabet is O(256) = O(1).
 * Time O(n * sigma) = O(n) for a fixed alphabet; space O(sigma + n).
 * Compile: gcc -O2 -Wall -o q06 program.c        Run: ./q06
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIGMA 256
#define MAXN 64

/* returns 1 and fills out[] on success, 0 if impossible */
static int reorganise(const char *s, int K, char *out) {
    int n = strlen(s), cnt[SIGMA] = {0}, last[SIGMA];
    for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
    for (int c = 0; c < SIGMA; c++) last[c] = -1000000;
    if (K <= 1) { strcpy(out, s); return 1; }           /* no restriction */
    for (int i = 0; i < n; i++) {
        int best = -1;
        for (int c = 0; c < SIGMA; c++)
            if (cnt[c] > 0 && i - last[c] >= K && (best < 0 || cnt[c] > cnt[best])) best = c;
        if (best < 0) { out[0] = 0; return 0; }
        out[i] = (char)best; cnt[best]--; last[best] = i;
    }
    out[n] = 0; return 1;
}
static int check(const char *orig, const char *res, int K) {      /* is res a valid arrangement of orig? */
    int a[SIGMA] = {0}, b[SIGMA] = {0}, n = strlen(orig);
    if ((int)strlen(res) != n) return 0;
    for (int i = 0; i < n; i++) { a[(unsigned char)orig[i]]++; b[(unsigned char)res[i]]++; }
    if (memcmp(a, b, sizeof a)) return 0;
    for (int i = 0; i < n; i++) for (int j = i + 1; j < n && j < i + K; j++) if (res[i] == res[j]) return 0;
    return 1;
}
/* Brute force: backtracking over all arrangements (exact feasibility) */
static int bt(int cnt[], int n, int pos, int K, int last[]) {
    if (pos == n) return 1;
    for (int c = 0; c < SIGMA; c++) if (cnt[c] > 0 && pos - last[c] >= K) {
        int old = last[c]; cnt[c]--; last[c] = pos;
        int r = bt(cnt, n, pos + 1, K, last);
        cnt[c]++; last[c] = old; if (r) return 1;
    }
    return 0;
}
static int feasible_brute(const char *s, int K) {
    int cnt[SIGMA] = {0}, last[SIGMA]; for (int c = 0; c < SIGMA; c++) last[c] = -1000000;
    for (const char *p = s; *p; p++) cnt[(unsigned char)*p]++;
    return bt(cnt, strlen(s), 0, K < 1 ? 1 : K, last);
}
static void run(const char *s, int K) {
    char out[MAXN]; int ok = reorganise(s, K, out);
    printf("S=\"%s\", K=%d  ->  ", s, K);
    if (ok) printf("\"%s\"  (valid: %s)", out, check(s, out, K) ? "yes" : "NO"); else printf("\"\" (impossible)");
    printf("   brute-force says %s -> %s\n", feasible_brute(s, K) ? "possible" : "impossible", (feasible_brute(s, K) == ok) ? "MATCH" : "MISMATCH");
}
int main(void) {
    printf("--- Sample tests ---\n");
    run("aabbcc", 3); run("aaabc", 3); run("aaadbbcc", 2); run("aab", 2); run("aa", 2);
    run("abc", 1); run("a", 5); run("aaaa", 1); run("aabbccdd", 4);
    srand(2026); int ok = 0, tot = 2000;
    for (int t = 0; t < tot; t++) {
        int n = 1 + rand() % 9, K = 1 + rand() % 5; char s[MAXN], out[MAXN];
        int sig = 1 + rand() % 4; for (int i = 0; i < n; i++) s[i] = 'a' + rand() % sig; s[n] = 0;
        int g = reorganise(s, K, out);
        if (g == feasible_brute(s, K) && (!g || check(s, out, K))) ok++;
    }
    printf("\n=== Random validation: %d/%d cases (greedy feasibility == brute force, output valid) ===\n", ok, tot);
    return 0;
}
