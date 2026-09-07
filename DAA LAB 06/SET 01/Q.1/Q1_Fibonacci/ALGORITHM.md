# Question 1 — Nth Fibonacci Number using Dynamic Programming

## 1. Problem Statement
Write a program to find the **n-th Fibonacci number** using Dynamic Programming.

The Fibonacci sequence is defined by the recurrence:

```
F(0) = 0
F(1) = 1
F(n) = F(n-1) + F(n-2)    for n >= 2
```

## 2. Why Dynamic Programming?
A direct recursive implementation of the recurrence re-computes the same
sub-problems exponentially many times:

```
                F(5)
               /    \
            F(4)     F(3)
           /   \      /  \
        F(3)  F(2) F(2) F(1)
        / \    / \  / \
     F(2) F(1)...  ...
```

This gives a naive recursive time complexity of **O(2ⁿ)**. Since the
recurrence exhibits both:
- **Optimal Substructure** – F(n) is built directly from F(n-1) and F(n-2).
- **Overlapping Sub-problems** – F(2), F(3), etc. are recomputed many times.

...it is a textbook candidate for Dynamic Programming, where each unique
sub-problem is solved once and its answer is *stored* (memoized) or built
up iteratively (tabulated) for reuse.

## 3. Algorithm — Bottom-Up Tabulation
```
Algorithm FIB-TABULATION(n):
    Input : non-negative integer n
    Output: F(n)

    1. Create array dp[0 .. n]
    2. dp[0] <- 0
    3. if n >= 1: dp[1] <- 1
    4. for i <- 2 to n:
    5.      dp[i] <- dp[i-1] + dp[i-2]
    6. return dp[n]
```

### Space-Optimized Variant
Since F(i) only depends on the *previous two* values, we don't need to
store the whole table — only two rolling variables are required:

```
Algorithm FIB-OPTIMIZED(n):
    1. if n == 0: return 0
    2. if n == 1: return 1
    3. prev2 <- 0, prev1 <- 1
    4. for i <- 2 to n:
    5.      curr  <- prev1 + prev2
    6.      prev2 <- prev1
    7.      prev1 <- curr
    8. return curr
```

### Top-Down Memoization (alternative)
```
Algorithm FIB-MEMO(n, memo[]):
    1. if n == 0 or n == 1: return n
    2. if memo[n] is already computed: return memo[n]
    3. memo[n] <- FIB-MEMO(n-1, memo) + FIB-MEMO(n-2, memo)
    4. return memo[n]
```

## 4. Correctness
By induction on `n`:
- **Base case:** dp[0]=0=F(0), dp[1]=1=F(1) — correct by definition.
- **Inductive step:** Assume dp[i-1]=F(i-1) and dp[i-2]=F(i-2) hold.
  Then dp[i] = dp[i-1] + dp[i-2] = F(i-1) + F(i-2) = F(i) by the
  recurrence definition. Hence dp[n] = F(n) for all n ≥ 0. ∎

## 5. Complexity Analysis

| Version                     | Time  | Space  |
|------------------------------|-------|--------|
| Bottom-Up Tabulation          | O(n)  | O(n)   |
| Space-Optimized Bottom-Up      | O(n)  | O(1)   |
| Top-Down Memoization           | O(n)  | O(n) (table + recursion stack) |
| (Naive recursion, for contrast)| O(2ⁿ) | O(n) (stack only) |

The DP formulation reduces time from exponential to **linear**, at the
cost of (optionally) linear extra space — which itself can be optimized
away to **O(1)** using the rolling-variable trick.

## 6. Input Representation
- A single non-negative integer `n` (0 ≤ n ≤ 1000 in this implementation,
  easily extendable using arbitrary-precision arithmetic / `long long`
  overflow awareness for very large n).

## 7. Files
- `fibonacci.c` — implements all three variants (tabulation,
  space-optimized, memoization) and cross-verifies they agree.
