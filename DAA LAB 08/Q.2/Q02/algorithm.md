# Q2 — Coin Change: Total Number of Ways

## Algorithm Name
Bottom-up Dynamic Programming for counting combinations (unbounded knapsack counting).

## Objective
Count the number of **distinct combinations** of coins (infinite supply of each) whose sum is exactly `V`. Order does not matter: `1+2` and `2+1` are the *same* combination.

## Input representation
```
n              (number of distinct coin types)
c1 c2 ... cn   (distinct positive integers)
V              (target amount, V >= 0)
```
Stored in an array `coins[0..n-1]`.

## Output
One number: how many combinations make `V` (`0` if none).

## Key idea — why the loop order matters
We let `ways[v]` be the number of combinations that make `v` using **only the coins processed so far**. We process the coins **one at a time (outer loop)**, and for each coin update all amounts (inner loop). Because each coin is "introduced" only once, a combination such as {1,2} is built in a single canonical order (all 1s first, then 2s), so `1+2` and `2+1` are not counted twice.

(If the two loops were swapped, we would count *ordered* sequences — permutations — which is a different problem.)

## Definition of the table
`ways[v]` = number of combinations of the coins considered so far that sum to `v`.

## Algorithm Steps
1. Create `ways[0..V]`, set every entry to 0, then set `ways[0] = 1` (exactly one way to make 0: take no coin).
2. For each coin `c` in `coins` (outer loop):
   * For `v = c, c+1, …, V` (inner loop):
     * `ways[v] = ways[v] + ways[v − c]`  
       (every combination for `v − c` becomes a combination for `v` by adding one more coin `c`).
3. The answer is `ways[V]`.

Termination: both loops are bounded, so the algorithm stops. The program uses `unsigned long long` because the count can grow quickly.

## Example / Dry Run
Coins = {1, 2, 5}, V = 5.

| After processing | ways[0..5] |
|---|---|
| start | 1 0 0 0 0 0 |
| coin 1 | 1 1 1 1 1 1 |
| coin 2 | 1 1 2 2 3 3 |
| coin 5 | 1 1 2 2 3 **4** |

The 4 combinations are: 5; 2+2+1; 2+1+1+1; 1+1+1+1+1.

## Complexity Analysis
Derivation: for coin `c` the inner loop runs `V − c + 1` times (if `c <= V`), each time with O(1) work. The total is `Σ max(0, V − c_i + 1)`, which is at most `n·V`.

| Case | Time |
|------|------|
| Best  | O(n) after the O(V) initialisation — when every coin is larger than `V` the inner loops never run |
| Average | Θ(n·V) (typical case with small coins) |
| Worst | Θ(n·V) — e.g. coin 1 is present and others are small |

* **Space:** one array of `V + 1` counters → **Θ(V)**.
* The numeric result can be large (it can overflow 64 bits for huge `V`); the program uses `unsigned long long` and does not detect overflow — fine for lab-size inputs.
