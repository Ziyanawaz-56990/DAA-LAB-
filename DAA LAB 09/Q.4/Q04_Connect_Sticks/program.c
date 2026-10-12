/*
 * Q4: Minimum cost to connect sticks   (DAA Lab 09)
 * Input: array L[0..n-1] of stick lengths.   Greedy: always join the two shortest (min-heap).
 * Compile: gcc -O2 -Wall -o q04 program.c        Run: ./q04
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 64
typedef long long ll;
static ll h[MAXN + 1]; static int hs;
static void push(ll x) { int i = hs++; h[i] = x; while (i > 0 && h[i] < h[(i-1)/2]) { int p = (i-1)/2; ll t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
static ll pop(void) { ll top = h[0]; h[0] = h[--hs]; int i = 0;
    for (;;) { int l = 2*i+1, r = l+1, m = i;
        if (l < hs && h[l] < h[m]) m = l;
        if (r < hs && h[r] < h[m]) m = r;
        if (m == i) break;
        ll t = h[i]; h[i] = h[m]; h[m] = t; i = m; }
    return top; }

/* O(n log n) */
static ll min_cost(const ll L[], int n) {
    hs = 0; for (int i = 0; i < n; i++) push(L[i]);
    ll cost = 0;
    while (hs > 1) { ll a = pop(), b = pop(); cost += a + b; push(a + b); }
    return cost;
}
/* Brute force: try EVERY pair at every step (exponential; n <= 7) */
static ll brute(ll a[], int n) {
    if (n == 1) return 0;
    ll best = -1;
    for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
        ll b[MAXN]; int m = 0; for (int k = 0; k < n; k++) if (k != i && k != j) b[m++] = a[k];
        b[m++] = a[i] + a[j];
        ll c = a[i] + a[j] + brute(b, m);
        if (best < 0 || c < best) best = c;
    }
    return best;
}
static void run(const char *name, ll L[], int n) {
    printf("\n--- %s ---\nSticks: ", name); for (int i = 0; i < n; i++) printf("%lld ", L[i]);
    ll g = min_cost(L, n), b = brute(L, n);
    printf("\nMinimum cost (greedy) = %lld\nBrute force           = %lld -> %s\n", g, b, g == b ? "MATCH" : "MISMATCH");
}
int main(void) {
    ll a[] = {2, 4, 3}; run("Test 1", a, 3);
    ll b[] = {1, 8, 3, 5}; run("Test 2", b, 4);
    ll c[] = {5}; run("Test 3: single stick (cost 0)", c, 1);
    ll d[] = {1, 1, 1, 1}; run("Test 4: equal sticks", d, 4);
    ll e[] = {10, 3, 7, 1, 20, 4}; run("Test 5: six sticks", e, 6);
    srand(2026); int ok = 0, tot = 500;
    for (int t = 0; t < tot; t++) { int n = 1 + rand() % 7; ll r[MAXN]; for (int i = 0; i < n; i++) r[i] = 1 + rand() % 30;
        if (min_cost(r, n) == brute(r, n)) ok++; }
    printf("\n=== Random validation: %d/%d cases greedy == brute force ===\n", ok, tot);
    return 0;
}
