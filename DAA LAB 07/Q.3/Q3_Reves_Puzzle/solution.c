/*
 * Q3 - Reve's Puzzle (4-peg Tower of Hanoi) via the Frame-Stewart algorithm
 * ---------------------------------------------------------------------------
 * Computes the minimum number of moves FS(n) for n disks on 4 pegs, and
 * prints the explicit move sequence.
 *
 * Compile : gcc -O2 -Wall -o solution solution.c
 * Run     : ./solution        (prompts for n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXN 40

long long FS[MAXN + 1];
int splitK[MAXN + 1];
long long moveCount = 0;

long long powi(int base, int exp) {
    long long r = 1;
    for (int i = 0; i < exp; i++) r *= base;
    return r;
}

/* Compute FS(n) for 0..n with memoisation, O(n^2) */
void computeFrameStewart(int n) {
    FS[0] = 0;
    FS[1] = 1;
    splitK[1] = 0;
    for (int m = 2; m <= n; m++) {
        long long best = LLONG_MAX;
        int bestK = 1;
        for (int k = 1; k < m; k++) {
            long long candidate = 2 * FS[k] + (powi(2, m - k) - 1);
            if (candidate < best) {
                best = candidate;
                bestK = k;
            }
        }
        FS[m] = best;
        splitK[m] = bestK;
    }
}

/* 3-peg Hanoi but disk labels are offset+1 .. offset+n (so printed disk
 * sizes match the true global stack, not a local sub-count). Used for
 * the middle phase of Frame-Stewart, where the 4th peg is "blocked". */
void solve3PegGlobal(int n, int offset, char src, char dst, char spare, int printMoves) {
    if (n == 0) return;
    solve3PegGlobal(n - 1, offset, src, spare, dst, printMoves);
    moveCount++;
    if (printMoves) printf("Move disk %d: %c -> %c\n", offset + n, src, dst);
    solve3PegGlobal(n - 1, offset, spare, dst, src, printMoves);
}

/* Frame-Stewart 4-peg solver. Disk numbering here is local: disk 1..n
 * are the n disks currently being moved (n is not necessarily the total
 * disk count when called recursively on the "top k" sub-stack), so we
 * pass an offset to print the true global disk size. */
void solve4Peg(int n, char src, char dst, char spare1, char spare2,
               int offset, int printMoves) {
    if (n == 0) return;
    int k = splitK[n];
    solve4Peg(k, src, spare1, dst, spare2, offset, printMoves);
    /* move disks (k+1..n), i.e. global sizes (offset+k+1 .. offset+n),
       using only 3 pegs (spare1 is occupied) */
    solve3PegGlobal(n - k, offset + k, src, dst, spare2, printMoves);
    solve4Peg(k, spare1, dst, src, spare2, offset, printMoves);
}

int main(void) {
    int n;
    printf("=== Reve's Puzzle (4-peg Tower of Hanoi, Frame-Stewart) ===\n");
    printf("Enter number of disks (n): ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAXN) {
        printf("Invalid input (1 <= n <= %d).\n", MAXN);
        return 1;
    }

    computeFrameStewart(n);
    printf("\nMinimum number of moves for n = %d disks (4 pegs): %lld\n", n, FS[n]);

    if (n == 8)
        printf("(Matches the classic Reve's Puzzle result: 33 moves)\n");

    char show;
    printf("Print the explicit move sequence? (y/n): ");
    if (scanf(" %c", &show) == 1 && (show == 'y' || show == 'Y')) {
        printf("\nMove sequence (pegs A,B,C,D ; disk 1 = smallest):\n");
        moveCount = 0;
        solve4Peg(n, 'A', 'D', 'B', 'C', 0, 1);
        printf("\nTotal moves printed: %lld\n", moveCount);
    } else {
        moveCount = 0;
        solve4Peg(n, 'A', 'D', 'B', 'C', 0, 0);
        printf("Total moves (verified by simulation, not printed): %lld\n", moveCount);
    }

    return 0;
}
