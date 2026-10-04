/*
 * DAA Lab 08 - Q2: Coin Change - Total number of ways (combinations)
 *
 * INPUT FORMAT (standard input):
 *     n            -> number of distinct coin denominations
 *     c1 c2 ... cn -> coin values (distinct positive integers)
 *     V            -> target amount (V >= 0)
 *
 * OUTPUT: number of distinct combinations (order does not matter).
 *
 * ways[v] = number of combinations that make amount v using the coins
 * processed so far.
 *   ways[0] = 1   (the empty combination)
 *   for each coin c (OUTER loop), for v = c..V (INNER loop):
 *         ways[v] += ways[v - c]
 * Coins are the OUTER loop so that 1+2 and 2+1 are counted only once.
 * Time: O(n * V)    Space: O(V)
 */
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull;

ull countWays(const int coins[], int n, int V)
{
    ull *ways = (ull *)calloc(V + 1, sizeof(ull));
    ull result;
    int i, v;
    if (ways == NULL) return 0;

    ways[0] = 1;
    for (i = 0; i < n; i++) {
        for (v = coins[i]; v <= V; v++) {
            ways[v] += ways[v - coins[i]];
        }
    }
    result = ways[V];
    free(ways);
    return result;
}

int main(void)
{
    int n, V, i, j;
    int *coins;

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
        for (j = 0; j < i; j++) {
            if (coins[j] == coins[i]) {
                printf("Coin denominations must be distinct.\n");
                free(coins);
                return 1;
            }
        }
    }
    if (scanf("%d", &V) != 1 || V < 0) {
        printf("Target amount must be a non-negative integer.\n");
        free(coins);
        return 1;
    }
    printf("Number of distinct combinations = %llu\n", countWays(coins, n, V));
    free(coins);
    return 0;
}
