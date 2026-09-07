/* ============================================================
   DAA LAB-6 : Question 2
   0/1 Knapsack Problem using Dynamic Programming
   ============================================================
   Given n items each with a weight[i] and profit[i], and a
   knapsack of capacity W, determine the maximum profit such
   that the total weight of chosen items <= W and every item
   is either fully taken (1) or left out (0).

   Time Complexity  : O(n * W)
   Space Complexity : O(n * W)   (can be optimized to O(W))
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) { return (a > b) ? a : b; }

/* Builds the full DP table so that the chosen items can be traced back */
int knapsack01(int n, int W, int weight[], int profit[], int **dp, int chosen[])
{
    for (int i = 0; i <= n; i++)
    {
        for (int w = 0; w <= W; w++)
        {
            if (i == 0 || w == 0)
            {
                dp[i][w] = 0;
            }
            else if (weight[i - 1] <= w)
            {
                int includeItem = profit[i - 1] + dp[i - 1][w - weight[i - 1]];
                int excludeItem = dp[i - 1][w];
                dp[i][w] = max(includeItem, excludeItem);
            }
            else
            {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    /* Backtrack through the table to find which items were selected */
    int w = W;
    for (int i = n; i > 0 && w >= 0; i--)
    {
        if (dp[i][w] != dp[i - 1][w])
        {
            chosen[i - 1] = 1;   /* item (i-1) was included */
            w -= weight[i - 1];
        }
    }

    return dp[n][W];
}

int main(void)
{
    int n, W;

    printf("=================================================\n");
    printf(" DAA LAB-6 : Q2 - 0/1 Knapsack using DP\n");
    printf("=================================================\n");
    printf("Enter number of items (n): ");
    scanf("%d", &n);
    printf("Enter knapsack capacity (W): ");
    scanf("%d", &W);

    int *weight = malloc(n * sizeof(int));
    int *profit = malloc(n * sizeof(int));
    int *chosen = calloc(n, sizeof(int));

    printf("Enter weight and profit for each item:\n");
    for (int i = 0; i < n; i++)
    {
        printf("  Item %d - weight profit: ", i + 1);
        scanf("%d %d", &weight[i], &profit[i]);
    }

    /* Allocate 2D DP table dynamically: (n+1) x (W+1) */
    int **dp = malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
        dp[i] = malloc((W + 1) * sizeof(int));

    int best = knapsack01(n, W, weight, profit, dp, chosen);

    printf("\n--- Result ---\n");
    printf("Maximum profit achievable within capacity %d = %d\n", W, best);
    printf("Items included (1-indexed): ");
    int any = 0;
    for (int i = 0; i < n; i++)
        if (chosen[i]) { printf("Item%d(w=%d,p=%d) ", i + 1, weight[i], profit[i]); any = 1; }
    if (!any) printf("(none)");
    printf("\n");

    printf("\nComplexity Analysis:\n");
    printf("  Time  complexity : O(n * W) = O(%d * %d) = O(%d)\n", n, W, n * W);
    printf("  Space complexity : O(n * W) for the table (optimizable to O(W) using a 1-D rolling array)\n");

    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    free(weight);
    free(profit);
    free(chosen);
    return 0;
}
