# Q7 — Rod Cutting with Reconstruction

## Algorithm Name
Bottom-up Dynamic Programming for Rod Cutting (with a "first cut" array for reconstruction).

## Objective
A rod has length `n`. A piece of length `i` sells for `p_i`. Cut the rod into integer-length pieces (or not at all) so that the **total revenue is maximum**, and also output **the lengths of the pieces** in an optimal cutting.

## Input representation
```
n
p1 p2 ... pn      (p_i = price of a piece of length i, non-negative integers)
```
Stored in arrays `price[1..n]`.

## Output
(i) the maximum revenue, (ii) the piece lengths of one optimal decomposition.

## Definition of the table
`r[j]` = maximum revenue for a rod of length `j`.
`firstCut[j]` = length of the first piece cut off in an optimal solution for length `j`.

## Recurrence
```
r[0] = 0
r[j] = max over i = 1..j of ( price[i] + r[j - i] )
```
*Optimal substructure:* after cutting the first piece of length `i`, the remaining rod of length `j − i` must be cut optimally.

## Algorithm Steps
1. Set `r[0] = 0`.
2. For `j = 1 … n`:
   1. `r[j] = −1` (smaller than any possible revenue).
   2. For `i = 1 … j`: if `price[i] + r[j − i] > r[j]`, set `r[j] = price[i] + r[j − i]` and `firstCut[j] = i`.
3. The maximum revenue is `r[n]`.
4. **Reconstruction:** `j = n`; while `j > 0`: output `firstCut[j]` and set `j = j − firstCut[j]`. The printed numbers add up to `n`.

(Leaving the rod uncut is the case `i = j` with `r[0] = 0`, so it is automatically considered.)

## Example / Dry Run
`n = 4`, prices `1 5 8 9`.

| j | 0 | 1 | 2 | 3 | 4 |
|---|---|---|---|---|---|
| r[j] | 0 | 1 | 5 | 8 | 10 |
| firstCut[j] | – | 1 | 2 | 3 | 2 |

`r[4] = max(1+r[3], 5+r[2], 8+r[1], 9+r[0]) = max(9, 10, 9, 9) = 10`, first cut 2, remaining rod 2 → first cut 2. Pieces: **2, 2**, revenue 10.

## Complexity Analysis
Derivation: the inner loop runs `j` times for each `j = 1 … n`, so the total is `1 + 2 + … + n = n(n+1)/2` iterations.

| Case | Time |
|------|------|
| Best  | Θ(n²) |
| Average | Θ(n²) |
| Worst | Θ(n²) |

* **Space:** arrays `r` and `firstCut` (and the prices) → Θ(n).
* (A naive recursive solution without storing results takes Θ(2ⁿ) time; that is what the DP avoids.)
