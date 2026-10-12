/*
 * Q2: Huffman Coding + Canonical Huffman Codebook   (DAA Lab 09)
 * Input representation: parallel arrays  sym[i] (a character) and freq[i] (a positive integer).
 * Compile: gcc -O2 -Wall -o q02 program.c        Run: ./q02
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXS 64

/* Tree nodes: 0..n-1 are leaves, n..2n-2 are internal nodes */
static long nfreq[2 * MAXS]; static int nleft[2 * MAXS], nright[2 * MAXS];
static int heap[2 * MAXS], hsize;

/* min-heap ordered by (frequency, node index): the tie-break makes output deterministic */
static int less(int a, int b) { return nfreq[a] < nfreq[b] || (nfreq[a] == nfreq[b] && a < b); }
static void hpush(int x) {
    int i = hsize++; heap[i] = x;
    while (i > 0 && less(heap[i], heap[(i - 1) / 2])) { int p = (i - 1) / 2, t = heap[i]; heap[i] = heap[p]; heap[p] = t; i = p; }
}
static int hpop(void) {
    int top = heap[0]; heap[0] = heap[--hsize]; int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hsize && less(heap[l], heap[m])) m = l;
        if (r < hsize && less(heap[r], heap[m])) m = r;
        if (m == i) break;
        int t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m;
    }
    return top;
}
static void depth_dfs(int v, int d, int n, int len[]) {
    if (v < n) { len[v] = d ? d : 1; return; }     /* single-symbol alphabet gets length 1 */
    depth_dfs(nleft[v], d + 1, n, len); depth_dfs(nright[v], d + 1, n, len);
}

/* Step 1: Huffman code LENGTHS via the greedy merge of two smallest.  O(n log n) */
static void huffman_lengths(int n, const long freq[], int len[]) {
    hsize = 0; int next = n;
    for (int i = 0; i < n; i++) { nfreq[i] = freq[i]; hpush(i); }
    while (hsize > 1) {
        int a = hpop(), b = hpop();
        nfreq[next] = nfreq[a] + nfreq[b]; nleft[next] = a; nright[next] = b; hpush(next++);
    }
    depth_dfs(heap[0], 0, n, len);
}

/* Independent check: total cost = sum of all merged weights (O(n^2) linear scan version) */
static long cost_by_merging(int n, const long freq[]) {
    long w[MAXS]; int m = n; long cost = 0;
    memcpy(w, freq, n * sizeof(long));
    while (m > 1) {
        int a = 0; for (int i = 1; i < m; i++) if (w[i] < w[a]) a = i;
        long x = w[a]; w[a] = w[--m];
        int b = 0; for (int i = 1; i < m; i++) if (w[i] < w[b]) b = i;
        long y = w[b]; w[b] = x + y; cost += x + y;
    }
    return cost;
}

typedef struct { char sym; long freq; int len; unsigned long code; } Entry;
static int cmp_canon(const void *a, const void *b) {
    const Entry *p = a, *q = b;
    if (p->len != q->len) return p->len - q->len;
    return (unsigned char)p->sym - (unsigned char)q->sym;    /* lexicographic by symbol */
}

/* Step 2: canonical codes.  O(n log n) for the sort */
static void canonical(Entry e[], int n) {
    qsort(e, n, sizeof(Entry), cmp_canon);
    unsigned long code = 0; e[0].code = 0;
    for (int i = 1; i < n; i++) { code = (code + 1) << (e[i].len - e[i - 1].len); e[i].code = code; }
}
static void bits(unsigned long c, int len, char *out) { for (int i = 0; i < len; i++) out[i] = (c >> (len - 1 - i) & 1) ? '1' : '0'; out[len] = 0; }

static void run(const char *title, const char sym[], const long freq[], int n) {
    printf("\n--- %s ---\n", title);
    int len[MAXS]; huffman_lengths(n, freq, len);
    Entry e[MAXS]; long total = 0;
    for (int i = 0; i < n; i++) { e[i].sym = sym[i]; e[i].freq = freq[i]; e[i].len = len[i]; total += freq[i]; }
    canonical(e, n);
    printf("Symbol  Freq  Len  Canonical code\n");
    long cost = 0; int maxlen = 0;
    for (int i = 0; i < n; i++) {
        char b[80]; bits(e[i].code, e[i].len, b);
        printf("  '%c'   %4ld  %3d  %s\n", e[i].sym, e[i].freq, e[i].len, b);
        cost += e[i].freq * e[i].len; if (e[i].len > maxlen) maxlen = e[i].len;
    }
    printf("Total encoded bits = %ld, expected length = %.4f bits/symbol\n", cost, (double)cost / total);
    /* validations */
    long kraft = 0; for (int i = 0; i < n; i++) kraft += 1L << (maxlen - e[i].len);
    int prefix_ok = 1;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (i != j && e[i].len <= e[j].len)
        if ((e[j].code >> (e[j].len - e[i].len)) == e[i].code) prefix_ok = 0;
    printf("Check 1 (Kraft sum == 1): %s\n", (n == 1 || kraft == (1L << maxlen)) ? "PASS" : "FAIL");
    printf("Check 2 (prefix-free)   : %s\n", prefix_ok ? "PASS" : "FAIL");
    printf("Check 3 (cost equals independent O(n^2) merge cost): %s\n", (n == 1 || cost == cost_by_merging(n, freq)) ? "PASS" : "FAIL");
}

int main(void) {
    char s1[] = {'a', 'b', 'c', 'd', 'e', 'f'}; long f1[] = {45, 13, 12, 16, 9, 5};
    run("Test 1: classic textbook example (CLRS)", s1, f1, 6);

    /* Test 2: frequencies counted from a text string */
    const char *text = "abracadabra"; long cnt[256] = {0}; for (const char *p = text; *p; p++) cnt[(unsigned char)*p]++;
    char s2[MAXS]; long f2[MAXS]; int n2 = 0;
    for (int c = 0; c < 256; c++) if (cnt[c]) { s2[n2] = (char)c; f2[n2++] = cnt[c]; }
    printf("\nText used for Test 2: \"%s\"", text);
    run("Test 2: frequencies counted from text", s2, f2, n2);

    char s3[] = {'w', 'x', 'y', 'z'}; long f3[] = {1, 1, 1, 1};
    run("Test 3: equal frequencies (balanced tree)", s3, f3, 4);

    char s4[] = {'p', 'q'}; long f4[] = {7, 3};
    run("Test 4: two symbols", s4, f4, 2);

    char s5[] = {'z'}; long f5[] = {10};
    run("Test 5: single symbol (edge case, gets 1-bit code)", s5, f5, 1);

    /* Random validation */
    srand(2026); int pass = 0, tot = 500;
    for (int c = 0; c < tot; c++) {
        int n = 2 + rand() % 12; long f[MAXS]; int len[MAXS];
        for (int i = 0; i < n; i++) { f[i] = 1 + rand() % 100; }
        huffman_lengths(n, f, len);
        long cost = 0, kraft = 0; int mx = 0;
        for (int i = 0; i < n; i++) { cost += f[i] * len[i]; if (len[i] > mx) mx = len[i]; }
        for (int i = 0; i < n; i++) kraft += 1L << (mx - len[i]);
        if (cost == cost_by_merging(n, f) && kraft == (1L << mx)) pass++;
    }
    printf("\n=== Random validation: %d/%d cases passed (cost match + Kraft equality) ===\n", pass, tot);
    return 0;
}
