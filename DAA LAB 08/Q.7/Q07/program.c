/*
 * DAA Lab 08 - Q7: Rod Cutting with Reconstruction
 *
 * INPUT FORMAT (standard input):
 *     n                -> length of the rod
 *     p1 p2 ... pn     -> p_i = price of a piece of length i (non-negative)
 * OUTPUT: (i) maximum revenue, (ii) the lengths of the pieces of an optimal cut.
 *
 * r[j] = best revenue for a rod of length j
 *   r[0] = 0
 *   r[j] = max over i = 1..j of ( p[i] + r[j - i] )
 *   firstCut[j] = the value of i that gave the maximum (for reconstruction)
 * Reconstruction: while j > 0 : output firstCut[j]; j = j - firstCut[j].
 * Time: O(n^2)   Space: O(n)
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, i, j;
    long long *price, *revenue;
    int *firstCut;

    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid rod length.\n");
        return 1;
    }
    price = (long long *)malloc((n + 1) * sizeof(long long));
    revenue = (long long *)malloc((n + 1) * sizeof(long long));
    firstCut = (int *)malloc((n + 1) * sizeof(int));
    if (price == NULL || revenue == NULL || firstCut == NULL) return 1;

    for (i = 1; i <= n; i++) {
        if (scanf("%lld", &price[i]) != 1 || price[i] < 0) {
            printf("Prices must be non-negative integers.\n");
            return 1;
        }
    }

    revenue[0] = 0;
    firstCut[0] = 0;
    for (j = 1; j <= n; j++) {
        revenue[j] = -1;
        for (i = 1; i <= j; i++) {
            if (price[i] + revenue[j - i] > revenue[j]) {
                revenue[j] = price[i] + revenue[j - i];
                firstCut[j] = i;
            }
        }
    }

    printf("Rod length n = %d\n", n);
    printf("(i)  Maximum revenue = %lld\n", revenue[n]);
    printf("(ii) Optimal piece lengths: ");
    j = n;
    while (j > 0) {
        printf("%d ", firstCut[j]);
        j -= firstCut[j];
    }
    printf("\n");

    free(price); free(revenue); free(firstCut);
    return 0;
}
