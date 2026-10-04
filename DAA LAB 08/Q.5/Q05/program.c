/*
 * DAA Lab 08 - Q5: Maximum Sum Increasing Subsequence (strictly increasing)
 *
 * INPUT FORMAT (standard input):
 *     n                 -> number of elements (n >= 1)
 *     a0 a1 ... an-1    -> positive integers
 * OUTPUT: maximum sum of a strictly increasing subsequence (and that subsequence).
 *
 * msis[i] = maximum sum of a strictly increasing subsequence ENDING at a[i]
 *   msis[i] = a[i] + max( msis[j] ) over j < i with a[j] < a[i]   (or just a[i])
 *   answer  = max over i of msis[i]
 * Time: O(n^2)   Space: O(n)
 */
#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

int main(void)
{
    int n, i, j, bestEnd = 0;
    int *a, *prev;
    ll *msis;

    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid n.\n");
        return 1;
    }
    a = (int *)malloc(n * sizeof(int));
    prev = (int *)malloc(n * sizeof(int));
    msis = (ll *)malloc(n * sizeof(ll));
    if (a == NULL || prev == NULL || msis == NULL) return 1;

    for (i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1 || a[i] <= 0) {
            printf("Elements must be positive integers.\n");
            return 1;
        }
    }

    for (i = 0; i < n; i++) {
        msis[i] = a[i];
        prev[i] = -1;
        for (j = 0; j < i; j++) {
            if (a[j] < a[i] && msis[j] + a[i] > msis[i]) {
                msis[i] = msis[j] + a[i];
                prev[i] = j;
            }
        }
        if (msis[i] > msis[bestEnd]) bestEnd = i;
    }

    printf("Maximum sum of a strictly increasing subsequence = %lld\n", msis[bestEnd]);

    /* rebuild the subsequence by following prev[] backwards (print reversed) */
    {
        int *seq = (int *)malloc(n * sizeof(int));
        int count = 0, idx = bestEnd;
        if (seq != NULL) {
            while (idx != -1) { seq[count++] = a[idx]; idx = prev[idx]; }
            printf("Subsequence: ");
            for (i = count - 1; i >= 0; i--) printf("%d ", seq[i]);
            printf("\n");
            free(seq);
        }
    }
    free(a); free(prev); free(msis);
    return 0;
}
