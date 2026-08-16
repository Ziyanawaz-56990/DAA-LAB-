/*
 * Q5: Multiply special-pattern square matrices using D&C in O(n^2)
 *
 * Each n x n matrix (n = 2^k) has the recursive block form
 *      M = [ M1  M2 ]
 *          [ M2  M1 ]
 * where M1, M2 (each n/2 x n/2) recursively have the SAME structure,
 * down to single scalars.
 *
 * KEY INSIGHT: such a matrix has only n independent values (not n^2!),
 * because f(n) = 2*f(n/2), f(1) = 1  =>  f(n) = n.
 * We store a matrix of this type compactly as a flat array of n values:
 *   repr(n) = [ repr(M1) (n/2 values) | repr(M2) (n/2 values) ]
 *
 * FACT (used below): if A, B both have this block pattern, then
 *   A*B = [ A1B1+A2B2   A1B2+A2B1 ]
 *         [ A1B2+A2B1   A1B1+A2B2 ]
 * i.e. the product ALSO has the same block-symmetric pattern! So we
 * only ever need to compute the top row of blocks; the compact
 * representation of the product is:
 *   ProdRepr(n) = [ Mul(A1,B1)+Mul(A2,B2)  |  Mul(A1,B2)+Mul(A2,B1) ]
 * where "+" here is an ELEMENT-WISE add of the two compact reprs
 * (size n/2), costing O(n/2), not a full matrix add.
 *
 * Recurrence: T(n) = 4*T(n/2) + O(n)   =>   T(n) = O(n^2)   [Master theorem]
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* elementwise add of two arrays of length len -> result in 'out' */
void addRepr(long a[], long b[], long out[], int len) {
    for (int i = 0; i < len; i++) out[i] = a[i] + b[i];
}

/*
 * Multiply two compact representations (each of length n, describing an
 * n x n special-pattern matrix), producing the compact representation
 * of the product (length n).
 */
void multiplyRepr(long A[], long B[], long out[], int n) {
    if (n == 1) {
        out[0] = A[0] * B[0];
        return;
    }
    int half = n / 2;
    long *A1 = A, *A2 = A + half;
    long *B1 = B, *B2 = B + half;

    long *P1 = malloc(sizeof(long) * half); /* A1*B1 */
    long *P2 = malloc(sizeof(long) * half); /* A2*B2 */
    long *P3 = malloc(sizeof(long) * half); /* A1*B2 */
    long *P4 = malloc(sizeof(long) * half); /* A2*B1 */

    multiplyRepr(A1, B1, P1, half);
    multiplyRepr(A2, B2, P2, half);
    multiplyRepr(A1, B2, P3, half);
    multiplyRepr(A2, B1, P4, half);

    addRepr(P1, P2, out, half);          /* C1 = A1B1 + A2B2 */
    addRepr(P3, P4, out + half, half);   /* C2 = A1B2 + A2B1 */

    free(P1); free(P2); free(P3); free(P4);
}

/* Expand a compact representation into a full n x n matrix (for display/verification) */
void expand(long repr[], int n, int fullN, long **full, int rowOff, int colOff) {
    if (n == 1) {
        full[rowOff][colOff] = repr[0];
        return;
    }
    int half = n / 2;
    /* Top-left = Bottom-right = M1 block; Top-right = Bottom-left = M2 block */
    expand(repr, half, fullN, full, rowOff, colOff);                 /* M1 -> top-left */
    expand(repr + half, half, fullN, full, rowOff, colOff + half);   /* M2 -> top-right */
    expand(repr + half, half, fullN, full, rowOff + half, colOff);   /* M2 -> bottom-left */
    expand(repr, half, fullN, full, rowOff + half, colOff + half);   /* M1 -> bottom-right */
}

long **allocFull(int n) {
    long **m = malloc(n * sizeof(long *));
    for (int i = 0; i < n; i++) m[i] = malloc(n * sizeof(long));
    return m;
}

void freeFull(long **m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

long **naiveMultiplyFull(long **A, long **B, int n) {
    long **C = allocFull(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            long sum = 0;
            for (int k = 0; k < n; k++) sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    return C;
}

void printFull(long **m, int n, const char *label) {
    printf("%s:\n", label);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%5ld ", m[i][j]);
        printf("\n");
    }
    printf("\n");
}

int main(void) {
    int k = 3;
    int n = 1 << k; /* n = 8 */

    /* n independent values define the special-pattern matrix */
    srand(7);
    long *A = malloc(sizeof(long) * n);
    long *B = malloc(sizeof(long) * n);
    for (int i = 0; i < n; i++) { A[i] = rand() % 10; B[i] = rand() % 10; }

    printf("Compact representation (n=%d independent values instead of n^2):\n", n);
    printf("A repr: "); for (int i = 0; i < n; i++) printf("%ld ", A[i]); printf("\n");
    printf("B repr: "); for (int i = 0; i < n; i++) printf("%ld ", B[i]); printf("\n\n");

    long *outRepr = malloc(sizeof(long) * n);
    multiplyRepr(A, B, outRepr, n);

    printf("Product compact repr: ");
    for (int i = 0; i < n; i++) printf("%ld ", outRepr[i]);
    printf("\n\n");

    /* Expand everything to full n x n matrices to display and verify */
    long **fullA = allocFull(n), **fullB = allocFull(n), **fullC = allocFull(n);
    expand(A, n, n, fullA, 0, 0);
    expand(B, n, n, fullB, 0, 0);
    expand(outRepr, n, n, fullC, 0, 0);

    printFull(fullA, n, "Full A (expanded)");
    printFull(fullB, n, "Full B (expanded)");
    printFull(fullC, n, "A x B via O(n^2) special D&C algorithm (expanded)");

    long **naiveC = naiveMultiplyFull(fullA, fullB, n);
    printFull(naiveC, n, "A x B via naive O(n^3) multiplication (verification)");

    int match = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (fullC[i][j] != naiveC[i][j]) match = 0;
    printf(match ? "Verification: MATCH.\n" : "Verification: MISMATCH!\n");

    /* Empirically show O(n^2) growth by timing increasing sizes */
    printf("\nTiming growth (should scale roughly as n^2):\n");
    for (int kk = 4; kk <= 14; kk++) {
        int sz = 1 << kk;
        long *X = malloc(sizeof(long) * sz);
        long *Y = malloc(sizeof(long) * sz);
        long *Z = malloc(sizeof(long) * sz);
        for (int i = 0; i < sz; i++) { X[i] = rand() % 5; Y[i] = rand() % 5; }

        clock_t start = clock();
        multiplyRepr(X, Y, Z, sz);
        clock_t end = clock();
        double secs = (double)(end - start) / CLOCKS_PER_SEC;
        printf("n=%-6d time=%.5f sec\n", sz, secs);

        free(X); free(Y); free(Z);
    }

    free(A); free(B); free(outRepr);
    freeFull(fullA, n); freeFull(fullB, n); freeFull(fullC, n); freeFull(naiveC, n);
    return 0;
}
