/* ============================================================
   DAA LAB-6 : Question 1
   Nth Fibonacci Number using Dynamic Programming
   ============================================================
   Two DP versions are implemented:
     1. Tabulation (Bottom-Up)      -> O(n) time, O(n) space
     2. Space-Optimized Bottom-Up   -> O(n) time, O(1) space
   A memoized (Top-Down) version is also shown for completeness.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

#define MAXN 1000

/* ---------- 1. Bottom-Up Tabulation : O(n) time, O(n) space ---------- */
long long fibTabulation(int n, long long dp[])
{
    dp[0] = 0;
    if (n >= 1) dp[1] = 1;

    for (int i = 2; i <= n; i++)
        dp[i] = dp[i - 1] + dp[i - 2];

    return dp[n];
}

/* ---------- 2. Space-Optimized Bottom-Up : O(n) time, O(1) space ---------- */
long long fibOptimized(int n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    long long prev2 = 0, prev1 = 1, curr = 0;
    for (int i = 2; i <= n; i++)
    {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

/* ---------- 3. Top-Down Memoization : O(n) time, O(n) space ---------- */
long long fibMemo(int n, long long memo[], int computed[])
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (computed[n]) return memo[n];

    memo[n] = fibMemo(n - 1, memo, computed) + fibMemo(n - 2, memo, computed);
    computed[n] = 1;
    return memo[n];
}

int main(void)
{
    int n;
    long long dp[MAXN + 1];
    long long memo[MAXN + 1];
    int computed[MAXN + 1] = {0};

    printf("=========================================\n");
    printf(" DAA LAB-6 : Q1 - Nth Fibonacci using DP \n");
    printf("=========================================\n");
    printf("Enter n (0 <= n <= %d): ", MAXN);
    if (scanf("%d", &n) != 1 || n < 0 || n > MAXN)
    {
        printf("Invalid input.\n");
        return 1;
    }

    long long resultTab  = fibTabulation(n, dp);
    long long resultOpt  = fibOptimized(n);
    long long resultMemo = fibMemo(n, memo, computed);

    printf("\n--- Results for n = %d ---\n", n);
    printf("Bottom-Up Tabulation   (O(n) time, O(n) space) : %lld\n", resultTab);
    printf("Space-Optimized DP     (O(n) time, O(1) space) : %lld\n", resultOpt);
    printf("Top-Down Memoization   (O(n) time, O(n) space) : %lld\n", resultMemo);

    printf("\nFirst min(n+1,15) Fibonacci numbers: ");
    for (int i = 0; i <= n && i < 15; i++)
        printf("%lld ", dp[i]);
    printf("\n");

    return 0;
}
