/*
 * DAA Lab 08 - Q6: Edit Distance with Traceback (Levenshtein distance)
 *
 * INPUT FORMAT (standard input): two strings A and B, one per line,
 * without spaces (read with %s):
 *     A
 *     B
 * OUTPUT: minimum number of insertions / deletions / substitutions to
 *         transform A into B, plus the traceback (the list of operations).
 *
 * D[i][j] = edit distance between A[0..i-1] and B[0..j-1]
 *   D[i][0] = i  (delete i characters),   D[0][j] = j  (insert j characters)
 *   if A[i-1] == B[j-1]: D[i][j] = D[i-1][j-1]
 *   else D[i][j] = 1 + min( D[i-1][j]   (delete A[i-1]),
 *                           D[i][j-1]   (insert B[j-1]),
 *                           D[i-1][j-1] (substitute A[i-1] -> B[j-1]) )
 * Traceback: start at (m, n) and walk back to (0, 0), recording operations.
 * Time: O(m * n)   Space: O(m * n)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000

static int min3(int a, int b, int c)
{
    int m = (a < b) ? a : b;
    return (m < c) ? m : c;
}

int main(void)
{
    char A[MAX_LEN + 1], B[MAX_LEN + 1];
    int m, n, i, j, step;
    int **D;
    char **ops;           /* ops[k] = text of k-th operation (stored backwards) */
    int opCount = 0;

    if (scanf("%1000s", A) != 1 || scanf("%1000s", B) != 1) {
        printf("Please enter two strings.\n");
        return 1;
    }
    m = (int)strlen(A);
    n = (int)strlen(B);

    D = (int **)malloc((m + 1) * sizeof(int *));
    if (D == NULL) return 1;
    for (i = 0; i <= m; i++) {
        D[i] = (int *)malloc((n + 1) * sizeof(int));
        if (D[i] == NULL) return 1;
    }
    for (i = 0; i <= m; i++) D[i][0] = i;
    for (j = 0; j <= n; j++) D[0][j] = j;

    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (A[i - 1] == B[j - 1])
                D[i][j] = D[i - 1][j - 1];
            else
                D[i][j] = 1 + min3(D[i - 1][j], D[i][j - 1], D[i - 1][j - 1]);
        }
    }

    printf("A = %s (length %d)\nB = %s (length %d)\n", A, m, B, n);
    printf("Minimum edit distance = %d\n", D[m][n]);

    /* ---- traceback ---- */
    ops = (char **)malloc((m + n + 1) * sizeof(char *));
    if (ops == NULL) return 1;
    i = m; j = n;
    while (i > 0 || j > 0) {
        char buf[100];
        if (i > 0 && j > 0 && A[i - 1] == B[j - 1] && D[i][j] == D[i - 1][j - 1]) {
            sprintf(buf, "Match   '%c' (A[%d] = B[%d])", A[i - 1], i, j);
            i--; j--;
        } else if (i > 0 && j > 0 && D[i][j] == D[i - 1][j - 1] + 1) {
            sprintf(buf, "Replace '%c' with '%c' (position %d of A)", A[i - 1], B[j - 1], i);
            i--; j--;
        } else if (i > 0 && D[i][j] == D[i - 1][j] + 1) {
            sprintf(buf, "Delete  '%c' (position %d of A)", A[i - 1], i);
            i--;
        } else {
            sprintf(buf, "Insert  '%c' (after position %d of A)", B[j - 1], i);
            j--;
        }
        ops[opCount] = (char *)malloc(strlen(buf) + 1);
        if (ops[opCount] == NULL) return 1;
        strcpy(ops[opCount], buf);
        opCount++;
    }

    printf("Traceback (from the start of A to the end):\n");
    step = 0;
    for (i = opCount - 1; i >= 0; i--) {
        printf("  %d. %s\n", ++step, ops[i]);
        free(ops[i]);
    }
    free(ops);
    for (i = 0; i <= m; i++) free(D[i]);
    free(D);
    return 0;
}
