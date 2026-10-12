/*
 * Q8: Minimum number of meeting rooms   (DAA Lab 09)
 * Input: two arrays start[i], end[i] (meeting i occupies [start, end), so end==start does NOT clash).
 * Greedy: sort starts and ends separately; sweep with two pointers.   O(n log n) time, O(n) space.
 * Compile: gcc -O2 -Wall -o q08 program.c        Run: ./q08
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 64
static int cmpi(const void *a, const void *b) { return *(const int*)a - *(const int*)b; }

static int min_rooms(const int s[], const int e[], int n) {
    int S[MAXN], E[MAXN]; memcpy(S, s, n * sizeof(int)); memcpy(E, e, n * sizeof(int));
    qsort(S, n, sizeof(int), cmpi); qsort(E, n, sizeof(int), cmpi);
    int rooms = 0, best = 0, j = 0;
    for (int i = 0; i < n; i++) {
        while (E[j] <= S[i]) { rooms--; j++; }          /* some meeting already finished: room freed */
        rooms++; if (rooms > best) best = rooms;
    }
    return best;
}
/* Independent O(n^2) check: at the start time of each meeting, count meetings in progress */
static int brute(const int s[], const int e[], int n) {
    int best = 0;
    for (int i = 0; i < n; i++) { int c = 0; for (int j = 0; j < n; j++) if (s[j] <= s[i] && s[i] < e[j]) c++; if (c > best) best = c; }
    return best;
}
static void run(const char *name, int s[], int e[], int n) {
    printf("\n--- %s ---\nIntervals: ", name); for (int i = 0; i < n; i++) printf("[%d,%d] ", s[i], e[i]);
    int g = min_rooms(s, e, n), b = brute(s, e, n);
    printf("\nMinimum rooms (greedy) = %d | O(n^2) check = %d -> %s\n", g, b, g == b ? "MATCH" : "MISMATCH");
}
int main(void) {
    int s1[] = {0, 5, 15}, e1[] = {30, 10, 20}; run("Test 1", s1, e1, 3);
    int s2[] = {7, 2}, e2[] = {10, 4}; run("Test 2: no overlap", s2, e2, 2);
    int s3[] = {1, 5}, e3[] = {5, 10}; run("Test 3: back-to-back meetings share a room", s3, e3, 2);
    int s4[] = {1, 2, 3, 4}, e4[] = {10, 10, 10, 10}; run("Test 4: all overlap", s4, e4, 4);
    int s5[] = {9, 9, 11, 13, 10}, e5[] = {12, 10, 14, 15, 13}; run("Test 5: mixed", s5, e5, 5);
    srand(2026); int ok = 0, tot = 2000;
    for (int t = 0; t < tot; t++) { int n = 1 + rand() % 12, s[MAXN], e[MAXN];
        for (int i = 0; i < n; i++) { s[i] = rand() % 30; e[i] = s[i] + 1 + rand() % 10; }
        if (min_rooms(s, e, n) == brute(s, e, n)) ok++; }
    printf("\n=== Random validation: %d/%d cases sweep == O(n^2) check ===\n", ok, tot);
    return 0;
}
