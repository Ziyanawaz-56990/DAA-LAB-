/*
 * Q7: Minimise deviation in array (two-way greedy with max-heap)   (DAA Lab 09)
 * Operations: odd -> x2, even -> /2.   deviation = max - min.
 * Trick: multiply every odd number by 2 once (after that we only ever need "divide even by 2").
 * Then repeatedly shrink the current maximum while it is even.  O(n log n * log M).
 * Compile: gcc -O2 -Wall -o q07 program.c        Run: ./q07
 */
#include <stdio.h>
#include <stdlib.h>
#define MAXN 16
typedef long long ll;
static ll h[4096]; static int hs;                      /* max-heap */
static void push(ll x) { int i = hs++; h[i] = x;
    while (i > 0 && h[i] > h[(i-1)/2]) { int p = (i-1)/2; ll t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
static ll pop(void) { ll top = h[0]; h[0] = h[--hs]; int i = 0;
    for (;;) { int l = 2*i+1, r = l+1, m = i;
        if (l < hs && h[l] > h[m]) m = l;
        if (r < hs && h[r] > h[m]) m = r;
        if (m == i) break;
        ll t = h[i]; h[i] = h[m]; h[m] = t; i = m; }
    return top; }

static ll min_deviation(const ll a[], int n) {
    hs = 0; ll mn = -1;
    for (int i = 0; i < n; i++) { ll x = (a[i] % 2) ? a[i] * 2 : a[i]; push(x); if (mn < 0 || x < mn) mn = x; }
    ll best = h[0] - mn;
    while (h[0] % 2 == 0) {                            /* the max can still be reduced */
        ll mx = pop();
        push(mx / 2); if (mx / 2 < mn) mn = mx / 2;
        if (h[0] - mn < best) best = h[0] - mn;
    }
    return best;
}
/* Brute force: element o*2^e can become o*2^k for k in 0..max(e,1); try every combination */
static ll bf_best;
static void rec(ll opts[][40], int cnt[], int n, int i, ll mx, ll mn) {
    if (i == n) { if (mx - mn < bf_best) bf_best = mx - mn; return; }
    for (int k = 0; k < cnt[i]; k++) { ll v = opts[i][k]; rec(opts, cnt, n, i + 1, v > mx ? v : mx, mn < 0 || v < mn ? v : mn); }
}
static ll brute(const ll a[], int n) {
    static ll opts[MAXN][40]; int cnt[MAXN];
    for (int i = 0; i < n; i++) { ll o = a[i]; int e = 0; while (o % 2 == 0) { o /= 2; e++; }
        int top = e > 1 ? e : 1; cnt[i] = 0; for (int k = 0; k <= top; k++) opts[i][cnt[i]++] = o << k; }
    bf_best = (ll)1 << 60; rec(opts, cnt, n, 0, 0, -1); return bf_best;
}
static void run(const char *name, ll a[], int n) {
    printf("\n--- %s ---\nArray: ", name); for (int i = 0; i < n; i++) printf("%lld ", a[i]);
    ll g = min_deviation(a, n), b = brute(a, n);
    printf("\nMinimum deviation (greedy) = %lld\nBrute force                = %lld -> %s\n", g, b, g == b ? "MATCH" : "MISMATCH");
}
int main(void) {
    ll a[] = {1, 2, 3, 4}; run("Test 1", a, 4);
    ll b[] = {4, 1, 5, 20, 3}; run("Test 2", b, 5);
    ll c[] = {2, 10, 8}; run("Test 3", c, 3);
    ll d[] = {7}; run("Test 4: single element", d, 1);
    ll e[] = {3, 3, 3}; run("Test 5: all equal", e, 3);
    srand(2026); int ok = 0, tot = 1000;
    for (int t = 0; t < tot; t++) { int n = 1 + rand() % 5; ll r[MAXN]; for (int i = 0; i < n; i++) r[i] = 1 + rand() % 40;
        if (min_deviation(r, n) == brute(r, n)) ok++; }
    printf("\n=== Random validation: %d/%d cases greedy == brute force ===\n", ok, tot);
    return 0;
}
