/*
 * DAA Lab 08 - Q1: Minimum Coin Change (Dynamic Programming, bottom-up)
 *
 * INPUT FORMAT (from standard input):
 *     n            -> number of coin denominations
 *     c1 c2 ... cn -> the n coin values (positive integers)
 *     V            -> target amount (V >= 0)
 *
 * OUTPUT: minimum number of coins needed to make V, or -1 if impossible.
 *         (We also print one set of coins that achieves the minimum.)
 *
 * dp[v] = minimum number of coins needed to make amount v
 *   dp[0] = 0
 *   dp[v] = 1 + min( dp[v - c] )  over all coins c <= v with dp[v-c] reachable
 * Time: O(n * V)    Space: O(V)
 */
#include <stdio.h>
#include <stdlib.h>

#define UNREACHABLE (-1)

/* Returns minimum coins for amount V, or -1. 'lastCoin' (size V+1) records
 * the coin used to reach each amount so that we can rebuild the answer. */
int minCoins(const int coins[], int n, int V, int lastCoin[])
{
    int *dp = (int *)malloc((V + 1) * sizeof(int));
    int v, i, result;
    if (dp == NULL) return UNREACHABLE;

    dp[0] = 0;
    lastCoin[0] = 0;
    for (v = 1; v <= V; v++) {
        dp[v] = UNREACHABLE;
        lastCoin[v] = 0;
        for (i = 0; i < n; i++) {
            int c = coins[i];
            if (c <= v && dp[v - c] != UNREACHABLE) {
                if (dp[v] == UNREACHABLE || dp[v - c] + 1 < dp[v]) {
                    dp[v] = dp[v - c] + 1;
                    lastCoin[v] = c;
                }
            }
        }
    }
    result = dp[V];
    free(dp);
    return result;
}

int main(void)
{
    int n, V, i;
    int *coins, *lastCoin, answer;

    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of coins.\n");
        return 1;
    }
    coins = (int *)malloc(n * sizeof(int));
    if (coins == NULL) return 1;
    for (i = 0; i < n; i++) {
        if (scanf("%d", &coins[i]) != 1 || coins[i] <= 0) {
            printf("Coin values must be positive integers.\n");
            free(coins);
            return 1;
        }
    }
    if (scanf("%d", &V) != 1 || V < 0) {
        printf("Target amount must be a non-negative integer.\n");
        free(coins);
        return 1;
    }

    lastCoin = (int *)malloc((V + 1) * sizeof(int));
    if (lastCoin == NULL) { free(coins); return 1; }

    answer = minCoins(coins, n, V, lastCoin);
    printf("Minimum number of coins = %d\n", answer);

    if (answer > 0) {
        int v = V;
        printf("One optimal combination: ");
        while (v > 0) {
            printf("%d ", lastCoin[v]);
            v -= lastCoin[v];
        }
        printf("\n");
    }
    free(coins);
    free(lastCoin);
    return 0;
}
