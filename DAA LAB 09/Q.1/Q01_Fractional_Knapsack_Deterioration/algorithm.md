# Q1 - Fractional Knapsack with Deterioration Rate

## 1. Understanding the problem (and the model we fix)
The statement leaves some details open, so we state them explicitly:
* Consuming 1 unit of weight takes 1 unit of time, so **time t = total weight consumed so far**.
* One unit of item i consumed at time t is worth **rho_i - lambda_i * t**, where rho_i = v_i / w_i.
* We choose the **order** of items and the **amount x_i** (0 <= x_i <= w_i) of each; total amount <= W.
* **Input representation:** integer weights and capacity (arrays `v[]`, `w[]`, `lam[]`, integer `W`). A fraction x_i / w_i is any whole number of units. For finer fractions, use a smaller unit (grams instead of kg).

Value of taking x units of item i starting at time s:
`x*rho_i - lambda_i * ( x*s + x(x-1)/2 )`

## 2. Why the "obvious" greedy fails
The obvious idea - "always take the item with the highest *current* density" - is **not optimal**.
Example: X (rho=10, lambda~0), Y (rho=9.9, lambda=5), 1 unit each, W=2.
Greedy takes X first (10) then Y at t=1 (4.9): total 14.9. Better: Y first (9.9) then X at t=1 (10): total 19.9.
The program prints this counter-example (Test 2) and counts how often the naive greedy loses on random data.

## 3. The correct idea
**Step 1 - ordering by exchange argument (the greedy part).**
Take two units, item i at time t and item j at time t' > t. Swapping them changes total value by (lambda_j - lambda_i)(t' - t) (positive when lambda_j > lambda_i).
So in an optimal schedule, **items with larger decay lambda must be consumed earlier**. Equal-lambda items can be in any order. Also, unused capacity is only ever left at the *end* (starting later never helps because lambda > 0). So the schedule is: items sorted by lambda **descending**, each consumed as one contiguous block.

**Step 2 - choose the amounts (dynamic programming).**
Sort items by lambda descending. Let `dp[i][t]` = best value using the first i items with exactly t units consumed.
`dp[i][t] = max over x in [0, min(w_i, t)] of dp[i-1][t-x] + blockValue(i, start=t-x, x)`
Answer = max over t <= W of dp[n][t]; back-track the choices to get the amounts.

## 4. Pseudocode
```
sort items by lambda descending
dp[0][0] = 0, dp[0][t>0] = -infinity
for i = 1..n:
    for t = 0..W:
        for x = 0..min(w_i, t):
            dp[i][t] = max(dp[i][t], dp[i-1][t-x] + x*rho_i - lambda_i*(x*(t-x) + x(x-1)/2))
best = max_t dp[n][t]
backtrack to read x_i for each item
```

## 5. Dry run (Test 2 of the program)
Items: X(rho 10, lam ~0, w 1), Y(rho 9.9, lam 5, w 1), W = 2. Sorted by lambda: Y, X.
Y at t=0: 9.9, then X at t=1: 10 - 0 = 10. Total = **19.9**.

## 6. Complexity
* Sorting: O(n log n).
* DP: n items x (W+1) states x up to min(w_i, W)+1 choices = **O(n * W * w_max) <= O(n W^2)** time, O(n W) space.
* This is *pseudo-polynomial* (depends on the numeric value of W), which is natural because fractions are measured in units.

## 7. Validation done by the program
* Brute force over **every order and every amount vector** for small inputs: DP matches in all tests.
* Random test (300 cases, fixed seed): DP == brute force in all, naive greedy was worse in many.

## 8. Edge cases
* If every remaining density is <= 0, taking more only loses value, so DP naturally leaves capacity unused.
* W = 0 gives value 0. One item: a single block.

## 9. Possible viva questions
* Why is sorting by density alone wrong here? (Because decay differs per item; delaying a fast-decaying item costs more.)
* Which part is greedy and which is DP? (Ordering is greedy via exchange argument; amounts via DP.)
* What if lambda_i = 0 for all items? (Then it reduces to the classic fractional knapsack: sort by density, O(n log n).)
