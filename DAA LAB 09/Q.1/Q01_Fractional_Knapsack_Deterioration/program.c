/*
 * Q1: Fractional Knapsack with Deterioration Rate  (DAA Lab 09)
 *
 * MODEL (see algorithm.md):
 *   - Item i: base value density rho_i = v_i / w_i, weight w_i (integer units), decay lambda_i > 0.
 *   - Time t = total weight consumed so far (1 unit of weight takes 1 unit of time).
 *   - One unit of item i consumed at time t is worth  rho_i - lambda_i * t.
 *   - "Fractional" = we may take any whole number of units x_i (0..w_i) of an item,
 *     i.e. the fraction x_i / w_i.  Use a smaller unit (grams instead of kg) for finer fractions.
 *
 * ALGORITHM: (1) sort by decay lambda DESCENDING (exchange argument),
 *            (2) dynamic programming over the sorted items to choose the amounts.
 * Also shown: the "obvious" greedy (best current density first) and why it can FAIL.
 *
 * Compile: gcc -O2 -Wall -o q01 program.c        Run: ./q01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 8
#define MAXW 64
#define NEG -1e18

typedef struct { double v; int w; double lam; double rho; } Item;

/* value of taking x units of item it, starting at time s */
static double block_value(const Item *it, int s, int x) {
    return x * it->rho - it->lam * ((double)x * s + (double)x * (x - 1) / 2.0);
}

static int cmp_lambda_desc(const void *a, const void *b) {
    const Item *p = a, *q = b;
    if (p->lam < q->lam) return 1;
    if (p->lam > q->lam) return -1;
    return 0;
}

/* ---------- Algorithm: sort by lambda desc + DP.  O(n log n + n*W*min(W,wmax)) ---------- */
static double solve_dp(Item items[], int n, int W, int amount[]) {
    Item s[MAXN]; memcpy(s, items, n * sizeof(Item));
    qsort(s, n, sizeof(Item), cmp_lambda_desc);
    static double dp[MAXN + 1][MAXW + 1];
    static int choice[MAXN + 1][MAXW + 1];
    for (int t = 0; t <= W; t++) dp[0][t] = NEG;
    dp[0][0] = 0;
    for (int i = 1; i <= n; i++) {
        for (int t = 0; t <= W; t++) {
            dp[i][t] = NEG; choice[i][t] = 0;
            for (int x = 0; x <= s[i-1].w && x <= t; x++) {
                if (dp[i-1][t-x] <= NEG / 2) continue;
                double val = dp[i-1][t-x] + block_value(&s[i-1], t - x, x);
                if (val > dp[i][t]) { dp[i][t] = val; choice[i][t] = x; }
            }
        }
    }
    int bt = 0; double best = NEG;
    for (int t = 0; t <= W; t++) if (dp[n][t] > best) { best = dp[n][t]; bt = t; }
    /* backtrack: print amounts per sorted item */
    int t = bt;
    for (int i = n; i >= 1; i--) { amount[i-1] = choice[i][t]; t -= choice[i][t]; }
    /* amount[] is in sorted (lambda desc) order; print order info through caller */
    memcpy(items, s, n * sizeof(Item));   /* give caller the sorted order */
    return best;
}

/* ---------- Naive greedy: at every unit of time take the best current density ---------- */
static double solve_naive_greedy(Item items[], int n, int W) {
    int left[MAXN]; for (int i = 0; i < n; i++) left[i] = items[i].w;
    double total = 0;
    for (int t = 0; t < W; t++) {
        int b = -1; double bd = 0;
        for (int i = 0; i < n; i++) if (left[i] > 0) {
            double d = items[i].rho - items[i].lam * t;
            if (d > bd) { bd = d; b = i; }
        }
        if (b < 0) break;
        total += bd; left[b]--;
    }
    return total;
}

/* ---------- Brute force: every order x every amount vector (tiny inputs only) ---------- */
static double best_brute;
static void brute_rec(Item it[], int n, int used, int order[], int k, int t, int W, double val) {
    if (val > best_brute) best_brute = val;
    if (k == n) return;
    for (int i = 0; i < n; i++) if (!(used >> i & 1))
        for (int x = 0; x <= it[i].w && t + x <= W; x++)
            brute_rec(it, n, used | (1 << i), order, k + 1, t + x, W, val + block_value(&it[i], t, x));
}
static double solve_brute(Item it[], int n, int W) {
    best_brute = 0; int order[MAXN];
    brute_rec(it, n, 0, order, 0, 0, W, 0);
    return best_brute;
}

static void make(Item *it, double v, int w, double lam) { it->v = v; it->w = w; it->lam = lam; it->rho = v / w; }

static void run_case(const char *name, Item in[], int n, int W) {
    printf("\n--- %s ---\n", name);
    printf("Capacity W = %d\n", W);
    printf("Item   v    w   rho=v/w  lambda\n");
    for (int i = 0; i < n; i++) printf("  %d  %5.1f %3d  %6.2f  %6.2f\n", i + 1, in[i].v, in[i].w, in[i].rho, in[i].lam);
    Item work[MAXN]; memcpy(work, in, n * sizeof(Item));
    int amount[MAXN];
    double dp = solve_dp(work, n, W, amount);
    printf("DP schedule (decay-descending order): ");
    int t = 0;
    for (int i = 0; i < n; i++) if (amount[i] > 0) {
        int idx = 0; for (int j = 0; j < n; j++) if (in[j].v == work[i].v && in[j].w == work[i].w && in[j].lam == work[i].lam) { idx = j; break; }
        printf("[item %d: %d/%d units, starts t=%d] ", idx + 1, amount[i], work[i].w, t);
        t += amount[i];
    }
    printf("\nOptimal total value (DP)      = %.3f\n", dp);
    printf("Naive density-greedy value    = %.3f %s\n", solve_naive_greedy(in, n, W),
           solve_naive_greedy(in, n, W) + 1e-9 < dp ? "  <-- naive greedy is WORSE" : "");
    double br = solve_brute(in, n, W);
    printf("Brute force (all orders/amounts) = %.3f  -> %s\n", br, (br - dp < 1e-9 && dp - br < 1e-9) ? "MATCH" : "MISMATCH");
}

int main(void) {
    Item a[MAXN];
    make(&a[0], 60, 6, 0.5); make(&a[1], 50, 5, 1.0); make(&a[2], 40, 4, 0.2);
    run_case("Test 1: mixed items", a, 3, 10);

    make(&a[0], 10, 1, 0.0 + 1e-9); make(&a[1], 9.9, 1, 5.0);   /* decay-sensitive counter-example */
    run_case("Test 2: counter-example for naive greedy (X: rho=10,lam~0 ; Y: rho=9.9,lam=5)", a, 2, 2);

    make(&a[0], 30, 3, 4.0); make(&a[1], 28, 4, 0.1); make(&a[2], 20, 2, 2.0); make(&a[3], 15, 5, 0.0 + 1e-9);
    run_case("Test 3: four items, tight capacity", a, 4, 6);

    /* Random validation against brute force (fixed seed, reproducible) */
    srand(2026);
    int ok = 0, tot = 300, naive_worse = 0;
    for (int c = 0; c < tot; c++) {
        int n = 2 + rand() % 3, W = 2 + rand() % 7; Item r[MAXN];
        for (int i = 0; i < n; i++) { int w = 1 + rand() % 4; make(&r[i], (1 + rand() % 90) / 5.0 * w, w, (1 + rand() % 40) / 10.0); }
        Item work[MAXN]; memcpy(work, r, n * sizeof(Item)); int am[MAXN];
        double d = solve_dp(work, n, W, am), b = solve_brute(r, n, W), g = solve_naive_greedy(r, n, W);
        if (d - b < 1e-6 && b - d < 1e-6) ok++;
        if (g + 1e-6 < d) naive_worse++;
    }
    printf("\n=== Random validation: %d/%d cases DP == brute force ===\n", ok, tot);
    printf("=== Naive density-greedy was strictly worse in %d of %d cases ===\n", naive_worse, tot);
    return 0;
}
