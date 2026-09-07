/* ============================================================
   DAA LAB-6 : Question 3
   Longest Common Subsequence (LCS) using Dynamic Programming
   ============================================================
   Given two strings X (length m) and Y (length n), find the
   length of their longest common subsequence and print the
   subsequence itself.

   Time Complexity  : O(m * n)
   Space Complexity : O(m * n)   (can be optimized to O(min(m,n)) if
                                  only the length, not the sequence, is needed)
   ============================================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int max(int a, int b) { return (a > b) ? a : b; }

int lcsLength(char *X, char *Y, int m, int n, int **dp)
{
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[m][n];
}

/* Reconstruct the actual LCS string by walking the DP table backwards */
void printLCS(char *X, char *Y, int m, int n, int **dp)
{
    int len = dp[m][n];
    char *lcs = malloc((len + 1) * sizeof(char));
    lcs[len] = '\0';

    int i = m, j = n, index = len - 1;
    while (i > 0 && j > 0)
    {
        if (X[i - 1] == Y[j - 1])
        {
            lcs[index--] = X[i - 1];
            i--; j--;
        }
        else if (dp[i - 1][j] >= dp[i][j - 1])
            i--;
        else
            j--;
    }

    printf("LCS string        : %s\n", lcs);
    free(lcs);
}

int main(void)
{
    char X[1000], Y[1000];

    printf("=================================================\n");
    printf(" DAA LAB-6 : Q3 - Longest Common Subsequence (LCS)\n");
    printf("=================================================\n");
    printf("Enter first string  X: ");
    scanf("%s", X);
    printf("Enter second string Y: ");
    scanf("%s", Y);

    int m = strlen(X), n = strlen(Y);

    int **dp = malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++)
        dp[i] = malloc((n + 1) * sizeof(int));

    int length = lcsLength(X, Y, m, n, dp);

    printf("\n--- Result ---\n");
    printf("Length of LCS(%s, %s) = %d\n", X, Y, length);
    printLCS(X, Y, m, n, dp);

    printf("\nComplexity Analysis:\n");
    printf("  Time  complexity : O(m * n) = O(%d * %d) = O(%d)\n", m, n, m * n);
    printf("  Space complexity : O(m * n) for the DP table (optimizable to O(min(m,n)) for length-only)\n");

    for (int i = 0; i <= m; i++) free(dp[i]);
    free(dp);
    return 0;
}
