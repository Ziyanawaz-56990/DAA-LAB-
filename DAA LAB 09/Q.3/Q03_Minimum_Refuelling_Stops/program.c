/*
 * Q3: Minimum number of refuelling stops (reverse / "look-back" greedy)   (DAA Lab 09)
 * Input: target D, starting fuel F (1 fuel = 1 distance), stations as arrays dist[i], fuel[i].
 * Compile: gcc -O2 -Wall -o q03 program.c        Run: ./q03
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXN 64
typedef long long ll;
typedef struct { ll d, f; } Station;

static ll heap[MAXN + 1]; static int hs;           /* max-heap of fuel amounts */
static void push(ll x) { int i = hs++; heap[i] = x;
    while (i > 0 && heap[i] > heap[(i-1)/2]) { int p = (i-1)/2; ll t = heap[i]; heap[i] = heap[p]; heap[p] = t; i = p; } }
static ll pop(void) { ll top = heap[0]; heap[0] = heap[--hs]; int i = 0;
    for (;;) { int l = 2*i+1, r = l+1, m = i;
        if (l < hs && heap[l] > heap[m]) m = l;
        if (r < hs && heap[r] > heap[m]) m = r;
        if (m == i) break;
        ll t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m; }
    return top; }
static int cmp(const void *a, const void *b) { ll x = ((Station*)a)->d, y = ((Station*)b)->d; return (x > y) - (x < y); }

/* Greedy: drive as far as possible; when stuck, retroactively refuel at the biggest station passed.  O(n log n) */
static int greedy(ll D, ll F, Station s[], int n) {
    qsort(s, n, sizeof(Station), cmp);
    hs = 0; ll reach = F; int stops = 0, i = 0;
    while (reach < D) {
        while (i < n && s[i].d <= reach) push(s[i++].f);
        if (hs == 0) return -1;
        reach += pop(); stops++;
    }
    return stops;
}
/* Independent check: DP, dp[j] = farthest distance reachable using exactly/at most j stops.  O(n^2) */
static int dp_check(ll D, ll F, Station s[], int n) {      /* s[] already sorted by greedy() */
    ll dp[MAXN + 1]; for (int j = 0; j <= n; j++) dp[j] = -1; dp[0] = F;
    for (int i = 0; i < n; i++) for (int j = i; j >= 0; j--) if (dp[j] >= s[i].d && dp[j] + s[i].f > dp[j+1]) dp[j+1] = dp[j] + s[i].f;
    for (int j = 0; j <= n; j++) if (dp[j] >= D) return j;
    return -1;
}
static void run(const char *name, ll D, ll F, Station s[], int n) {
    printf("\n--- %s ---\nTarget D=%lld, start fuel F=%lld, stations (distance,fuel): ", name, D, F);
    for (int i = 0; i < n; i++) printf("(%lld,%lld) ", s[i].d, s[i].f);
    int g = greedy(D, F, s, n), d = dp_check(D, F, s, n);
    if (g < 0) printf("\nMinimum stops (greedy) = -1 (cannot reach)\n"); else printf("\nMinimum stops (greedy) = %d\n", g);
    printf("DP cross-check          = %d  -> %s\n", d, g == d ? "MATCH" : "MISMATCH");
}
int main(void) {
    Station a[] = {{10,60},{20,30},{30,30},{60,40}}; run("Test 1: classic (D=100,F=10)", 100, 10, a, 4);
    Station b[] = {{10,100}}; run("Test 2: no stations needed (F>=D)", 1, 1, b, 0);
    Station c[] = {{5,10}}; run("Test 3: impossible (first station out of reach)", 100, 1, c, 1);
    Station e[] = {{4,4},{5,2},{9,6},{3,1}}; run("Test 4: unsorted stations (D=15,F=3)", 15, 3, e, 4);
    srand(2026); int ok = 0, tot = 1000;
    for (int t = 0; t < tot; t++) {
        int n = rand() % 9; Station r[MAXN]; for (int i = 0; i < n; i++) { r[i].d = 1 + rand() % 40; r[i].f = 1 + rand() % 15; }
        ll D = 1 + rand() % 60, F = 1 + rand() % 12;
        int g = greedy(D, F, r, n), d = dp_check(D, F, r, n); if (g == d) ok++;
    }
    printf("\n=== Random validation: %d/%d cases greedy == DP ===\n", ok, tot);
    return 0;
}
