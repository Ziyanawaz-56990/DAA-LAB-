/* ============================================================
   DAA LAB-6 : Question 4
   Matrix Chain Multiplication (MCM) using Dynamic Programming
   ============================================================
   Given the dimensions of N-1 matrices in an array arr[0..N-1]
   (matrix i has dimensions arr[i-1] x arr[i]), find the minimum
   number of scalar multiplications needed to multiply the
   complete chain, and also print an optimal parenthesization.

   Sample Input  : N = 4, arr[] = {10, 30, 5, 60}
   Sample Output : 4500          (Time Complexity : O(N^3))

   Time Complexity  : O(N^3)
   Space Complexity : O(N^2)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Recursively print the optimal parenthesization using the split table */
void printParenthesization(int **s, int i, int j, char *label)
{
    if (i == j)
    {
        printf("%c%d", *label, i);
        (*label)++;
        return;
    }
    printf("(");
    printParenthesization(s, i, s[i][j], label);
    printParenthesization(s, s[i][j] + 1, j, label);
    printf(")");
}

int matrixChainOrder(int arr[], int n, int **s)
{
    /* n = number of matrices + 1 = size of arr[] */
    int N = n - 1; /* number of matrices */
    int **dp = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        dp[i] = calloc(n, sizeof(int));

    /* dp[i][j] = min scalar mults to compute product of matrices i..j (1-indexed) */
    for (int len = 2; len <= N; len++)
    {
        for (int i = 1; i <= N - len + 1; i++)
        {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; k++)
            {
                int cost = dp[i][k] + dp[k + 1][j] + arr[i - 1] * arr[k] * arr[j];
                if (cost < dp[i][j])
                {
                    dp[i][j] = cost;
                    s[i][j] = k;
                }
            }
        }
    }

    int result = dp[1][N];
    for (int i = 0; i < n; i++) free(dp[i]);
    free(dp);
    return result;
}

int main(void)
{
    int N;

    printf("=================================================\n");
    printf(" DAA LAB-6 : Q4 - Matrix Chain Multiplication (DP)\n");
    printf("=================================================\n");
    printf("Enter N (number of dimension entries, matrices = N-1): ");
    scanf("%d", &N);

    int *arr = malloc(N * sizeof(int));
    printf("Enter the %d dimension values: ", N);
    for (int i = 0; i < N; i++)
        scanf("%d", &arr[i]);

    int **s = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        s[i] = calloc(N, sizeof(int));

    int minMultiplications = matrixChainOrder(arr, N, s);

    printf("\n--- Result ---\n");
    printf("Number of matrices        : %d\n", N - 1);
    printf("Dimension array arr[]     : {");
    for (int i = 0; i < N; i++) printf("%d%s", arr[i], (i < N - 1) ? ", " : "");
    printf("}\n");
    printf("Minimum scalar multiplications = %d\n", minMultiplications);

    printf("Optimal Parenthesization  : ");
    char label = 'A';
    printParenthesization(s, 1, N - 1, &label);
    printf("\n");

    printf("\nComplexity Analysis:\n");
    printf("  Time  complexity : O(N^3)   (three nested loops: len, i, k)\n");
    printf("  Space complexity : O(N^2)   (dp and split tables)\n");

    for (int i = 0; i < N; i++) free(s[i]);
    free(s);
    free(arr);
    return 0;
}
