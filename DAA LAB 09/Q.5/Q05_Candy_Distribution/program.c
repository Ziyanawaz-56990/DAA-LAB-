/*
 * Q5: Candy distribution (bi-directional slope greedy)   (DAA Lab 09)
 * Input: array rating[0..n-1].  Left-to-right pass + right-to-left pass.  O(n) time, O(n) space.
 * Compile: gcc -O2 -Wall -o q05 program.c        Run: ./q05
 */
#include <stdio.h>
#include <stdlib.h>
#define MAXN 64

static int candies(const int r[], int n, int c[]) {
    for (int i = 0; i < n; i++) c[i] = 1;
    for (int i = 1; i < n; i++) if (r[i] > r[i-1]) c[i] = c[i-1] + 1;          /* left neighbour rule  */
    for (int i = n - 2; i >= 0; i--) if (r[i] > r[i+1] && c[i] <= c[i+1]) c[i] = c[i+1] + 1;   /* right neighbour rule */
    int s = 0; for (int i = 0; i < n; i++) s += c[i]; return s;
}
/* Independent check: start with all 1s and repeatedly fix violations until none remain (least fixed point) */
static int relax(const int r[], int n) {
    int c[MAXN]; for (int i = 0; i < n; i++) c[i] = 1;
    for (int changed = 1; changed; ) { changed = 0;
        for (int i = 0; i < n; i++) {
            if (i > 0 && r[i] > r[i-1] && c[i] <= c[i-1]) { c[i] = c[i-1] + 1; changed = 1; }
            if (i < n-1 && r[i] > r[i+1] && c[i] <= c[i+1]) { c[i] = c[i+1] + 1; changed = 1; } } }
    int s = 0; for (int i = 0; i < n; i++) s += c[i]; return s;
}
static int valid(const int r[], const int c[], int n) {
    for (int i = 0; i < n; i++) { if (c[i] < 1) return 0;
        if (i > 0 && r[i] > r[i-1] && c[i] <= c[i-1]) return 0;
        if (i < n-1 && r[i] > r[i+1] && c[i] <= c[i+1]) return 0; }
    return 1;
}
static void run(const char *name, int r[], int n) {
    int c[MAXN]; int tot = candies(r, n, c);
    printf("\n--- %s ---\nRatings: ", name); for (int i = 0; i < n; i++) printf("%d ", r[i]);
    printf("\nCandies: "); for (int i = 0; i < n; i++) printf("%d ", c[i]);
    printf("\nMinimum total = %d | valid distribution: %s | relaxation check = %d -> %s\n", tot, valid(r, c, n) ? "yes" : "NO", relax(r, n), relax(r, n) == tot ? "MATCH" : "MISMATCH");
}
int main(void) {
    int a[] = {1, 0, 2}; run("Test 1", a, 3);
    int b[] = {1, 2, 2}; run("Test 2: equal neighbours need no extra candy", b, 3);
    int c[] = {1, 2, 87, 87, 87, 2, 1}; run("Test 3: plateau in the middle", c, 7);
    int d[] = {5, 4, 3, 2, 1}; run("Test 4: strictly decreasing", d, 5);
    int e[] = {1, 3, 4, 5, 2}; run("Test 5: peak at the end side", e, 5);
    int f[] = {7}; run("Test 6: single child", f, 1);
    srand(2026); int ok = 0, tot = 1000;
    for (int t = 0; t < tot; t++) { int n = 1 + rand() % 12, r[MAXN], cc[MAXN]; for (int i = 0; i < n; i++) r[i] = rand() % 6;
        int g = candies(r, n, cc); if (g == relax(r, n) && valid(r, cc, n)) ok++; }
    printf("\n=== Random validation: %d/%d cases (two-pass == relaxation, and distribution valid) ===\n", ok, tot);
    return 0;
}
