# Question 2 — 0/1 Knapsack Problem using Dynamic Programming

## 1. Problem Statement
Given `n` items, each with a `weight[i]` and a `profit[i]`, and a knapsack
of capacity `W`, determine the **maximum total profit** obtainable by
selecting a subset of items such that:
- the sum of the selected items' weights does not exceed `W`, and
- each item is either fully included (1) or fully excluded (0) — it
  cannot be split (hence "0/1").

Also analyze the time and space complexity of the algorithm.

## 2. Why Dynamic Programming?
Brute-force enumeration of all subsets of `n` items costs **O(2ⁿ)**.
The problem has:
- **Optimal Substructure**: the best solution using items `1..i` with
  capacity `w` depends only on the best solutions for items `1..i-1`.
- **Overlapping Sub-problems**: the same `(i, w)` pair is required by many
  different decision paths.

This makes it solvable via DP over the state `(item index, remaining capacity)`.

## 3. State Definition
Let `dp[i][w]` = maximum profit achievable using the **first i items**
with knapsack capacity **w**.

### Recurrence
```
dp[0][w] = 0                                  for all w        (no items)
dp[i][0] = 0                                  for all i        (no capacity)

if weight[i-1] > w:
    dp[i][w] = dp[i-1][w]                                     (can't include item i)
else:
    dp[i][w] = max( dp[i-1][w],                                (exclude item i)
                     profit[i-1] + dp[i-1][w - weight[i-1]] )  (include item i)
```

Final answer: `dp[n][W]`.

## 4. Algorithm (Pseudocode)
```
Algorithm KNAPSACK-01(weight[1..n], profit[1..n], W):
    for i <- 0 to n:
        for w <- 0 to W:
            if i == 0 or w == 0:
                dp[i][w] <- 0
            else if weight[i-1] <= w:
                dp[i][w] <- max(profit[i-1] + dp[i-1][w-weight[i-1]], dp[i-1][w])
            else:
                dp[i][w] <- dp[i-1][w]
    return dp[n][W]
```

### Reconstructing which items were chosen
Starting from `dp[n][W]`, walk backward: if `dp[i][w] != dp[i-1][w]` then
item `i` was included in the optimal solution; subtract its weight from
`w` and move to `dp[i-1][w-weight[i-1]]`; otherwise move to `dp[i-1][w]`
unchanged. This backtracking costs an additional O(n) time.

## 5. Correctness
By induction on `i`:
- **Base case (i=0)**: with zero items, the best profit for any capacity
  is 0 — trivially true.
- **Inductive step**: assume `dp[i-1][*]` correctly stores the optimal
  profit using the first `(i-1)` items for every capacity. For item `i`,
  either it is excluded (profit = `dp[i-1][w]`) or, if it fits
  (`weight[i-1] <= w`), included (profit = `profit[i-1] + dp[i-1][w-weight[i-1]]`).
  Since these are the *only* two choices for item `i`, and both sub-cases
  rely on already-correct smaller sub-problems, taking the max of the two
  gives the true optimal profit for `dp[i][w]`. ∎

## 6. Complexity Analysis

| Metric | Complexity |
|--------|------------|
| Time   | **O(n · W)** — a table of size (n+1)×(W+1), each cell filled in O(1) |
| Space  | **O(n · W)** for the full table (needed to reconstruct chosen items); optimizable to **O(W)** using a single rolling 1-D array (processed right-to-left) if only the maximum profit value is required, not the item list |

This is a **pseudo-polynomial** time algorithm: it is polynomial in the
*numeric value* of W, not in the number of bits needed to represent W —
for very large W the DP table becomes impractically large, at which
point approximation (FPTAS) or branch-and-bound techniques are preferred.

## 7. Input Representation
- Integers `n` (number of items) and `W` (capacity).
- Two arrays `weight[]` and `profit[]` of size `n`.
- A dynamically allocated 2-D DP table of size `(n+1) x (W+1)`.

## 8. Files
- `knapsack.c` — builds the full DP table, prints the maximum profit,
  reconstructs and prints the selected items, and reports the complexity.
