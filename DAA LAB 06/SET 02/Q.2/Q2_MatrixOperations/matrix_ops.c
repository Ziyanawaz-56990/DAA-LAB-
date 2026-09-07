/* ============================================================================
   DAA LAB-6 (PDF Set) : Question 2
   2-D Square Matrix Operations and their worst-case complexities
   ============================================================================
   Given square matrices of size n x n, this program implements and times:
     (i)   Matrix Addition                     -> O(n^2)
     (ii)  Matrix Multiplication (naive)       -> O(n^3)
     (iii) Checking if the matrix is a zero matrix     -> O(n^2)
     (iv)  Checking if the matrix is symmetric         -> O(n^2)
     (v)   Determinant (cofactor expansion)            -> O(n!) worst case
                                                          (O(n^3) via LU / Gaussian elimination)
     (vi)  Transpose in-place (in situ)                -> O(n^2)
     (vii) Dominant Eigenvalue & Eigenvector via
           the Power Iteration method                  -> O(k * n^2), k = #iterations
   ============================================================================
   Input representation: a dynamically-allocated n x n array of doubles.
   NOTE on (vii): finding ALL eigenvalues/eigenvectors of a general matrix in
   closed form is only possible for n <= 4 (via the characteristic
   polynomial) and for larger n requires iterative numerical methods (QR
   algorithm, Power Iteration, Jacobi method for symmetric matrices, etc.).
   For pedagogical + practical purposes we implement the Power Iteration
   method, which converges to the dominant (largest magnitude) eigenvalue
   and its corresponding eigenvector for diagonalizable matrices.
   ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double **Matrix;

Matrix allocMatrix(int n)
{
    Matrix m = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) m[i] = calloc(n, sizeof(double));
    return m;
}

void freeMatrix(Matrix m, int n)
{
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

void readMatrix(Matrix m, int n, const char *label)
{
    printf("Enter %d x %d matrix %s (row by row):\n", n, n, label);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &m[i][j]);
}

void printMatrix(Matrix m, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("  [ ");
        for (int j = 0; j < n; j++) printf("%8.3f ", m[i][j]);
        printf("]\n");
    }
}

/* ---------------- (i) Matrix Addition : O(n^2) ---------------- */
Matrix matrixAdd(Matrix A, Matrix B, int n)
{
    Matrix C = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

/* ---------------- (ii) Matrix Multiplication : O(n^3) ---------------- */
Matrix matrixMultiply(Matrix A, Matrix B, int n)
{
    Matrix C = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            double sum = 0;
            for (int k = 0; k < n; k++)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    return C;
}

/* ---------------- (iii) Zero matrix check : O(n^2) ---------------- */
int isZeroMatrix(Matrix A, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (A[i][j] != 0.0) return 0;
    return 1;
}

/* ---------------- (iv) Symmetric matrix check : O(n^2) ---------------- */
int isSymmetric(Matrix A, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (A[i][j] != A[j][i]) return 0;
    return 1;
}

/* ---------------- (v) Determinant via cofactor expansion : O(n!) ----------
   (A production implementation would use LU/Gaussian elimination for O(n^3);
    cofactor expansion is shown here because it directly generalizes the
    2x2 / 3x3 formulas taught in class and needs no pivoting.)             */
double determinant(Matrix A, int n)
{
    if (n == 1) return A[0][0];
    if (n == 2) return A[0][0] * A[1][1] - A[0][1] * A[1][0];

    double det = 0;
    Matrix sub = allocMatrix(n - 1);
    for (int x = 0; x < n; x++)
    {
        int subi = 0;
        for (int i = 1; i < n; i++)
        {
            int subj = 0;
            for (int j = 0; j < n; j++)
            {
                if (j == x) continue;
                sub[subi][subj] = A[i][j];
                subj++;
            }
            subi++;
        }
        double sign = (x % 2 == 0) ? 1 : -1;
        det += sign * A[0][x] * determinant(sub, n - 1);
    }
    freeMatrix(sub, n - 1);
    return det;
}

/* ---------------- (vi) Transpose IN-PLACE (in situ) : O(n^2) ---------------- */
void transposeInPlace(Matrix A, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            double t = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = t;
        }
}

/* ---------------- (vii) Dominant eigenvalue/eigenvector : Power Iteration ---
   Time complexity: O(k * n^2) where k is the number of iterations.        */
void powerIteration(Matrix A, int n, int maxIter, double tol,
                     double *eigenvalue, double eigenvector[])
{
    double *v = malloc(n * sizeof(double));
    double *w = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) v[i] = 1.0; /* initial guess vector */

    double lambdaOld = 0.0, lambdaNew = 0.0;

    for (int iter = 0; iter < maxIter; iter++)
    {
        /* w = A * v */
        for (int i = 0; i < n; i++)
        {
            w[i] = 0;
            for (int j = 0; j < n; j++)
                w[i] += A[i][j] * v[j];
        }

        /* normalize w by its largest-magnitude component (infinity norm) */
        double norm = 0;
        for (int i = 0; i < n; i++)
            if (fabs(w[i]) > fabs(norm)) norm = w[i];

        if (norm == 0) { lambdaNew = 0; break; }

        for (int i = 0; i < n; i++) v[i] = w[i] / norm;

        lambdaNew = norm;
        if (fabs(lambdaNew - lambdaOld) < tol) { lambdaOld = lambdaNew; break; }
        lambdaOld = lambdaNew;
    }

    *eigenvalue = lambdaNew;
    for (int i = 0; i < n; i++) eigenvector[i] = v[i];

    free(v); free(w);
}

int main(void)
{
    int n;
    printf("=========================================================\n");
    printf(" DAA LAB-6 (PDF) : Q2 - 2D Square Matrix Ops & Complexity\n");
    printf("=========================================================\n");
    printf("Enter matrix size n (n x n): ");
    scanf("%d", &n);

    Matrix A = allocMatrix(n);
    Matrix B = allocMatrix(n);
    readMatrix(A, n, "A");
    readMatrix(B, n, "B");

    printf("\nMatrix A:\n"); printMatrix(A, n);
    printf("Matrix B:\n"); printMatrix(B, n);

    /* (i) Addition */
    Matrix sum = matrixAdd(A, B, n);
    printf("\n(i)   A + B  [O(n^2)] :\n"); printMatrix(sum, n);

    /* (ii) Multiplication */
    Matrix prod = matrixMultiply(A, B, n);
    printf("\n(ii)  A x B  [O(n^3)] :\n"); printMatrix(prod, n);

    /* (iii) Zero matrix check */
    printf("\n(iii) Is A a zero matrix? [O(n^2)] : %s\n", isZeroMatrix(A, n) ? "YES" : "NO");

    /* (iv) Symmetric check */
    printf("(iv)  Is A symmetric?     [O(n^2)] : %s\n", isSymmetric(A, n) ? "YES" : "NO");

    /* (v) Determinant */
    Matrix Acopy = allocMatrix(n);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) Acopy[i][j] = A[i][j];
    double det = determinant(Acopy, n);
    printf("(v)   Determinant of A    [O(n!) cofactor expansion] : %.4f\n", det);
    freeMatrix(Acopy, n);

    /* (vi) In-place transpose (work on a copy so A stays intact for later use) */
    Matrix T = allocMatrix(n);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) T[i][j] = A[i][j];
    transposeInPlace(T, n);
    printf("(vi)  Transpose of A (in-place)  [O(n^2)] :\n"); printMatrix(T, n);

    /* (vii) Dominant eigenvalue / eigenvector via power iteration */
    double eigenvalue;
    double *eigenvector = malloc(n * sizeof(double));
    powerIteration(A, n, 1000, 1e-9, &eigenvalue, eigenvector);
    printf("(vii) Dominant eigenvalue of A  [O(k*n^2) power iteration] : %.6f\n", eigenvalue);
    printf("      Corresponding eigenvector (normalized, largest component = 1): [ ");
    for (int i = 0; i < n; i++) printf("%.6f ", eigenvector[i]);
    printf("]\n");

    printf("\nComplexity Summary (n = %d):\n", n);
    printf("  Addition        : O(n^2) = O(%d)\n", n * n);
    printf("  Multiplication  : O(n^3) = O(%d)\n", n * n * n);
    printf("  Zero-check      : O(n^2)\n");
    printf("  Symmetric-check : O(n^2)\n");
    printf("  Determinant     : O(n!) via cofactor expansion (O(n^3) via LU decomposition)\n");
    printf("  Transpose       : O(n^2), O(1) extra space (in-place)\n");
    printf("  Eigen (power it): O(k*n^2), k = number of iterations to converge\n");

    freeMatrix(A, n); freeMatrix(B, n); freeMatrix(sum, n); freeMatrix(prod, n); freeMatrix(T, n);
    free(eigenvector);
    return 0;
}
