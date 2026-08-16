/*
 * Q4: Matrix Multiplication using Divide and Conquer (Strassen's method)
 * Multiplies two n x n matrices where n is a power of 2, using only
 * 7 recursive multiplications instead of 8.
 */
#include <stdio.h>
#include <stdlib.h>

typedef int **Matrix;

Matrix allocMatrix(int n) {
    Matrix m = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) m[i] = calloc(n, sizeof(int));
    return m;
}

void freeMatrix(Matrix m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

Matrix addMatrix(Matrix a, Matrix b, int n, int sign) {
    Matrix c = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            c[i][j] = a[i][j] + sign * b[i][j];
    return c;
}

/* Copy sub-block of size n starting at (rowOff,colOff) from src into dst */
void getBlock(Matrix src, Matrix dst, int n, int rowOff, int colOff) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dst[i][j] = src[rowOff + i][colOff + j];
}

void setBlock(Matrix dst, Matrix src, int n, int rowOff, int colOff) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dst[rowOff + i][colOff + j] = src[i][j];
}

Matrix strassen(Matrix A, Matrix B, int n) {
    Matrix C = allocMatrix(n);

    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int half = n / 2;
    Matrix A11 = allocMatrix(half), A12 = allocMatrix(half);
    Matrix A21 = allocMatrix(half), A22 = allocMatrix(half);
    Matrix B11 = allocMatrix(half), B12 = allocMatrix(half);
    Matrix B21 = allocMatrix(half), B22 = allocMatrix(half);

    getBlock(A, A11, half, 0, 0);
    getBlock(A, A12, half, 0, half);
    getBlock(A, A21, half, half, 0);
    getBlock(A, A22, half, half, half);

    getBlock(B, B11, half, 0, 0);
    getBlock(B, B12, half, 0, half);
    getBlock(B, B21, half, half, 0);
    getBlock(B, B22, half, half, half);

    /* Strassen's 7 products */
    Matrix t1 = addMatrix(A11, A22, half, 1);
    Matrix t2 = addMatrix(B11, B22, half, 1);
    Matrix M1 = strassen(t1, t2, half);
    freeMatrix(t1, half); freeMatrix(t2, half);

    Matrix t3 = addMatrix(A21, A22, half, 1);
    Matrix M2 = strassen(t3, B11, half);
    freeMatrix(t3, half);

    Matrix t4 = addMatrix(B12, B22, half, -1);
    Matrix M3 = strassen(A11, t4, half);
    freeMatrix(t4, half);

    Matrix t5 = addMatrix(B21, B11, half, -1);
    Matrix M4 = strassen(A22, t5, half);
    freeMatrix(t5, half);

    Matrix t6 = addMatrix(A11, A12, half, 1);
    Matrix M5 = strassen(t6, B22, half);
    freeMatrix(t6, half);

    Matrix t7 = addMatrix(A21, A11, half, -1);
    Matrix t8 = addMatrix(B11, B12, half, 1);
    Matrix M6 = strassen(t7, t8, half);
    freeMatrix(t7, half); freeMatrix(t8, half);

    Matrix t9 = addMatrix(A12, A22, half, -1);
    Matrix t10 = addMatrix(B21, B22, half, 1);
    Matrix M7 = strassen(t9, t10, half);
    freeMatrix(t9, half); freeMatrix(t10, half);

    /* C11 = M1 + M4 - M5 + M7 */
    Matrix x1 = addMatrix(M1, M4, half, 1);
    Matrix x2 = addMatrix(x1, M5, half, -1);
    Matrix C11 = addMatrix(x2, M7, half, 1);
    freeMatrix(x1, half); freeMatrix(x2, half);

    /* C12 = M3 + M5 */
    Matrix C12 = addMatrix(M3, M5, half, 1);

    /* C21 = M2 + M4 */
    Matrix C21 = addMatrix(M2, M4, half, 1);

    /* C22 = M1 - M2 + M3 + M6 */
    Matrix y1 = addMatrix(M1, M2, half, -1);
    Matrix y2 = addMatrix(y1, M3, half, 1);
    Matrix C22 = addMatrix(y2, M6, half, 1);
    freeMatrix(y1, half); freeMatrix(y2, half);

    setBlock(C, C11, half, 0, 0);
    setBlock(C, C12, half, 0, half);
    setBlock(C, C21, half, half, 0);
    setBlock(C, C22, half, half, half);

    Matrix blocks[] = {A11, A12, A21, A22, B11, B12, B21, B22,
                        M1, M2, M3, M4, M5, M6, M7, C11, C12, C21, C22};
    for (size_t i = 0; i < sizeof(blocks) / sizeof(blocks[0]); i++)
        freeMatrix(blocks[i], half);

    return C;
}

Matrix naiveMultiply(Matrix A, Matrix B, int n) {
    Matrix C = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    return C;
}

void printMatrix(Matrix m, int n, const char *label) {
    printf("%s:\n", label);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%6d ", m[i][j]);
        printf("\n");
    }
    printf("\n");
}

int main(void) {
    int n = 4; /* power of 2 */
    Matrix A = allocMatrix(n);
    Matrix B = allocMatrix(n);

    int valsA[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    int valsB[4][4] = {{16,15,14,13},{12,11,10,9},{8,7,6,5},{4,3,2,1}};
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            A[i][j] = valsA[i][j];
            B[i][j] = valsB[i][j];
        }

    printMatrix(A, n, "Matrix A");
    printMatrix(B, n, "Matrix B");

    Matrix C_strassen = strassen(A, B, n);
    Matrix C_naive = naiveMultiply(A, B, n);

    printMatrix(C_strassen, n, "A x B (Strassen's method)");
    printMatrix(C_naive, n, "A x B (naive method, for verification)");

    int match = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (C_strassen[i][j] != C_naive[i][j]) match = 0;

    printf(match ? "Verification: MATCH - Strassen result equals naive result.\n"
                 : "Verification: MISMATCH!\n");

    freeMatrix(A, n); freeMatrix(B, n);
    freeMatrix(C_strassen, n); freeMatrix(C_naive, n);
    return 0;
}
