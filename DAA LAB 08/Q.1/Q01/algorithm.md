# Q1 — Minimum Coin Change

## Algorithm Name
Bottom-up Dynamic Programming for Minimum Coin Change (unbounded coins).

## Objective
Given coin values `c1 … cn` (each can be used any number of times) and a target amount `V`, find the **smallest number of coins** whose values add up to exactly `V`. If no combination works, answer `-1`.

## Input representation (chosen by us)
Standard input, in this order:
```
n              (number of coin types)
c1 c2 ... cn   (coin values, positive integers)
V              (target amount, V >= 0)
```
Inside the program the coins are stored in a 1-D array `coins[0..n-1]`.

## Output
A single integer: the minimum number of coins, or `-1` if impossible. (The program also prints one optimal set of coins.)

## Why Dynamic Programming works
* **Optimal substructure:** if the last coin used for amount `v` is `c`, the remaining `v − c` must itself be paid with the fewest coins possible. Otherwise we could improve the answer for `v`.
* **Overlapping subproblems:** the amount `v − c` is needed again and again by many different larger amounts, so we compute each amount only once and store it.

(A greedy method — "always take the biggest coin" — is **wrong** in general. Example: coins {1,3,4}, V = 6: greedy gives 4+1+1 = 3 coins, but the optimum is 3+3 = 2 coins.)

## Definition of the table
`dp[v]` = minimum number of coins needed to make amount `v` (`-1` means "not possible").

## Algorithm Steps
1. Create arrays `dp[0..V]` and `lastCoin[0..V]`.
2. **Base case:** `dp[0] = 0` (zero coins make amount 0).
3. For every amount `v = 1, 2, …, V` (in increasing order):
   1. Set `dp[v] = -1` (unreachable so far).
   2. For every coin `c = coins[i]`, `i = 0 … n-1`:
      * If `c <= v` **and** `dp[v − c] != -1` (the smaller amount is reachable):
        * candidate = `dp[v − c] + 1`.
        * If `dp[v]` is still `-1` or candidate `< dp[v]`, set `dp[v] = candidate` and remember `lastCoin[v] = c`.
4. The answer is `dp[V]` (it is `-1` if `V` cannot be formed).
5. **Reconstruction (optional):** while `v > 0`, output `lastCoin[v]` and set `v = v − lastCoin[v]`.

Termination: the outer loop runs a fixed `V` times and the inner loop a fixed `n` times, so the algorithm always stops.

## Example / Dry Run
Coins = {1, 2, 5}, V = 11.

| v | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
|---|---|---|---|---|---|---|---|---|---|---|----|----|
| dp[v] | 0 | 1 | 1 | 2 | 2 | 1 | 2 | 2 | 3 | 3 | 2 | 3 |

For example `dp[11] = min(dp[10]+1, dp[9]+1, dp[6]+1) = min(3, 4, 3) = 3`, i.e. 5 + 5 + 1.
Edge cases: V = 0 → answer 0; coins {2}, V = 3 → answer −1.

## Complexity Analysis
Derivation: the outer loop runs `V` times; each time the inner loop checks all `n` coins and does O(1) work. The total number of basic steps is exactly `V × n`, whatever the coin values are.

| Case | Time |
|------|------|
| Best  | Θ(n·V) — the loops always run completely (even if V is impossible) |
| Average | Θ(n·V) |
| Worst | Θ(n·V) |

* **Space:** `dp` and `lastCoin` each need `V + 1` integers → **Θ(V)**.
* Note: this is a *pseudo-polynomial* algorithm — it is polynomial in the **value** of `V`, not in the number of digits of `V`.
