/* ============================================================================
   DAA LAB-6 (PDF Set) : Question 3
   Convolution of two vectors in O(n log n) using Divide & Conquer (FFT)
   ============================================================================
   Definition:  C[k] = sum_{j=0}^{m-1} A[j] * B[k - j]      (linear convolution)
   Given |A| = m, |B| = n, n >= m, result C has length (m + n - 1).

   Naive convolution costs O(n*m) = O(n^2) in the worst case.

   The Fast Fourier Transform (FFT) is a Divide & Conquer algorithm
   (Cooley-Tukey, radix-2) that evaluates a degree-(L-1) polynomial at all
   L-th roots of unity in O(L log L) time by recursively splitting the
   polynomial into its even- and odd-indexed coefficients:

        P(x) = P_even(x^2) + x * P_odd(x^2)

   Convolution <=> Polynomial multiplication, and by the Convolution Theorem:

        C = IFFT( FFT(A_padded) (pointwise*) FFT(B_padded) )

   Steps:
     1. Pick L = smallest power of two >= (m + n - 1).
     2. Zero-pad A and B to length L.
     3. FFT(A), FFT(B)                       -> O(L log L)  [divide & conquer]
     4. Pointwise multiply the transforms    -> O(L)
     5. Inverse FFT of the product            -> O(L log L)  [divide & conquer]
     6. Round the (real parts of the) first (m+n-1) results to get C[].

   Overall Time Complexity : O(L log L) = O(n log n)   (since L = O(n) when n >= m)
   Overall Space Complexity: O(L) = O(n)
   ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

typedef double complex cplx;

/* ---------------- Recursive radix-2 FFT : Divide & Conquer, O(L log L) ----
   invert = +1 for forward FFT, -1 for inverse FFT (caller divides by L)   */
void fft(cplx *a, int L, int invert)
{
    if (L == 1) return;

    /* Divide: split into even-indexed and odd-indexed coefficients */
    cplx *even = malloc((L / 2) * sizeof(cplx));
    cplx *odd  = malloc((L / 2) * sizeof(cplx));
    for (int i = 0; i < L / 2; i++)
    {
        even[i] = a[2 * i];
        odd[i]  = a[2 * i + 1];
    }

    /* Conquer: recursively transform each half */
    fft(even, L / 2, invert);
    fft(odd,  L / 2, invert);

    /* Combine: butterfly step using the roots of unity */
    double angleSign = invert ? 1.0 : -1.0;
    for (int k = 0; k < L / 2; k++)
    {
        double angle = angleSign * 2.0 * M_PI * k / L;
        cplx w = cos(angle) + I * sin(angle);
        cplx t = w * odd[k];
        a[k]         = even[k] + t;
        a[k + L / 2] = even[k] - t;
    }

    free(even);
    free(odd);
}

int nextPowerOfTwo(int x)
{
    int p = 1;
    while (p < x) p <<= 1;
    return p;
}

/* Convolution via FFT: returns array of length (m+n-1) */
double *fftConvolution(double A[], int m, double B[], int n, int *resultLen)
{
    int convLen = m + n - 1;
    int L = nextPowerOfTwo(convLen);
    *resultLen = convLen;

    cplx *fa = calloc(L, sizeof(cplx));
    cplx *fb = calloc(L, sizeof(cplx));
    for (int i = 0; i < m; i++) fa[i] = A[i];
    for (int i = 0; i < n; i++) fb[i] = B[i];

    fft(fa, L, 0);   /* forward FFT of A */
    fft(fb, L, 0);   /* forward FFT of B */

    for (int i = 0; i < L; i++) fa[i] = fa[i] * fb[i]; /* pointwise multiply */

    fft(fa, L, 1);   /* inverse FFT */

    double *C = malloc(convLen * sizeof(double));
    for (int i = 0; i < convLen; i++)
        C[i] = creal(fa[i]) / L;   /* IFFT requires dividing by L */

    free(fa); free(fb);
    return C;
}

/* Naive O(n*m) convolution used ONLY to cross-verify the FFT result */
double *naiveConvolution(double A[], int m, double B[], int n, int *resultLen)
{
    int convLen = m + n - 1;
    *resultLen = convLen;
    double *C = calloc(convLen, sizeof(double));
    for (int k = 0; k < convLen; k++)
        for (int j = 0; j < m; j++)
            if (k - j >= 0 && k - j < n)
                C[k] += A[j] * B[k - j];
    return C;
}

int main(void)
{
    int m, n;
    printf("=========================================================\n");
    printf(" DAA LAB-6 (PDF) : Q3 - Convolution via FFT (Divide&Conquer)\n");
    printf("=========================================================\n");
    printf("Enter length of vector A (m): ");
    scanf("%d", &m);
    double *A = malloc(m * sizeof(double));
    printf("Enter %d elements of A: ", m);
    for (int i = 0; i < m; i++) scanf("%lf", &A[i]);

    printf("Enter length of vector B (n), where n >= m: ");
    scanf("%d", &n);
    if (n < m) { printf("Error: this implementation assumes n >= m.\n"); return 1; }
    double *B = malloc(n * sizeof(double));
    printf("Enter %d elements of B: ", n);
    for (int i = 0; i < n; i++) scanf("%lf", &B[i]);

    int lenFFT, lenNaive;
    double *cFFT   = fftConvolution(A, m, B, n, &lenFFT);
    double *cNaive = naiveConvolution(A, m, B, n, &lenNaive);

    printf("\n--- Result: C = A convolved with B  (length %d) ---\n", lenFFT);
    printf("FFT Divide&Conquer result : [ ");
    for (int i = 0; i < lenFFT; i++) printf("%.4f ", cFFT[i]);
    printf("]\n");

    printf("Naive O(n*m) cross-check  : [ ");
    for (int i = 0; i < lenNaive; i++) printf("%.4f ", cNaive[i]);
    printf("]\n");

    double maxDiff = 0;
    for (int i = 0; i < lenFFT; i++)
    {
        double diff = fabs(cFFT[i] - cNaive[i]);
        if (diff > maxDiff) maxDiff = diff;
    }
    printf("Max absolute difference between FFT and naive result: %.10f %s\n",
           maxDiff, (maxDiff < 1e-6) ? "(MATCH)" : "(MISMATCH!)");

    printf("\nComplexity Analysis:\n");
    printf("  Naive convolution : O(n*m)\n");
    printf("  FFT-based D&C     : O(L log L) where L = smallest power of 2 >= (m+n-1)\n");
    printf("                      Since n >= m, L = O(n)  =>  Overall = O(n log n)\n");
    printf("  Space             : O(L) = O(n)\n");

    free(A); free(B); free(cFFT); free(cNaive);
    return 0;
}
