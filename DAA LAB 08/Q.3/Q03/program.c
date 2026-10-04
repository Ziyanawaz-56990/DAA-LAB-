/*
 * DAA Lab 08 - Q3: Longest Common Subsequence (LCS)
 *
 * INPUT FORMAT (standard input): two strings, each on its own line,
 * containing no spaces (read with %s):
 *     X
 *     Y
 * OUTPUT: length of the LCS and the LCS string itself.
 *
 * L[i][j] = length of LCS of X[0..i-1] and Y[0..j-1]
 *   L[i][0] = L[0][j] = 0
 *   if X[i-1] == Y[j-1]: L[i][j] = L[i-1][j-1] + 1
 *   else                : L[i][j] = max(L[i-1][j], L[i][j-1])
 * Reconstruction: walk back from L[m][n] to L[0][0].
 * Time: O(m * n)    Space: O(m * n)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000

static int maxOf(int a, int b) { return (a > b) ? a : b; }

int main(void)
{
    char X[MAX_LEN + 1], Y[MAX_LEN + 1];
    int m, n, i, j, len, k;
    int **L;
    char *lcs;

    if (scanf("%1000s", X) != 1 || scanf("%1000s", Y) != 1) {
        printf("Please enter two strings.\n");
        return 1;
    }
    m = (int)strlen(X);
    n = (int)strlen(Y);

    L = (int **)malloc((m + 1) * sizeof(int *));
    if (L == NULL) return 1;
    for (i = 0; i <= m; i++) {
        L[i] = (int *)calloc(n + 1, sizeof(int)); /* row 0 / column 0 stay 0 */
        if (L[i] == NULL) return 1;
    }

    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1])
                L[i][j] = L[i - 1][j - 1] + 1;
            else
                L[i][j] = maxOf(L[i - 1][j], L[i][j - 1]);
        }
    }

    len = L[m][n];
    lcs = (char *)malloc(len + 1);
    if (lcs == NULL) return 1;
    lcs[len] = '\0';

    i = m; j = n; k = len - 1;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {          /* part of the LCS */
            lcs[k--] = X[i - 1];
            i--; j--;
        } else if (L[i - 1][j] >= L[i][j - 1]) {
            i--;                              /* came from above */
        } else {
            j--;                              /* came from the left */
        }
    }

    printf("X = %s\nY = %s\n", X, Y);
    printf("Length of LCS = %d\n", len);
    printf("LCS string    = \"%s\"\n", lcs);

    for (i = 0; i <= m; i++) free(L[i]);
    free(L);
    free(lcs);
    return 0;
}
