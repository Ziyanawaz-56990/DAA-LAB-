# Q4 — Longest (Strictly) Increasing Subsequence (LIS)

## Algorithm Name
Dynamic Programming for LIS — O(n²) method (main), with an O(n log n) "tails + binary search" method used as an independent check.

## Objective
Given an integer array `A[0..n-1]`, find the length of the longest subsequence whose elements are **strictly increasing** (`a_i1 < a_i2 < …` with `i1 < i2 < …`).

## Input representation
```
n
a0 a1 ... a(n-1)
```
Stored in an `int` array of size `n`.

## Output
The length of the LIS (the program also prints one LIS).

## Definition of the table
`lis[i]` = length of the longest strictly increasing subsequence that **ends exactly at index i**.
`prev[i]` = the index before `i` in that subsequence (or −1) — used for reconstruction.

## Recurrence
```
lis[i] = 1 + max{ lis[j] : j < i and A[j] < A[i] }      (just 1 if no such j)
answer = max over i of lis[i]
```

## Algorithm Steps (main O(n²) algorithm)
1. For `i = 0 … n-1`:
   1. Set `lis[i] = 1`, `prev[i] = -1` (the element alone).
   2. For `j = 0 … i-1`: if `A[j] < A[i]` and `lis[j] + 1 > lis[i]`, set `lis[i] = lis[j] + 1` and `prev[i] = j`.
   3. Remember the index `bestEnd` with the largest `lis` value.
2. The answer is `lis[bestEnd]`.
3. Reconstruction: follow `prev[]` from `bestEnd` back to −1, filling the answer from the back.

## Check algorithm (O(n log n))
Keep an array `tails` where `tails[k]` = the smallest possible last element of a strictly increasing subsequence of length `k+1`. For each element `x`, binary-search the first position with `tails[pos] >= x`; put `x` there; if `pos` equals the current size, the size grows by one. The final size is the LIS length. The program prints both results and checks that they agree.

## Example / Dry Run
`A = [10, 9, 2, 5, 3, 7, 101, 18]`

| i | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| A[i] | 10 | 9 | 2 | 5 | 3 | 7 | 101 | 18 |
| lis[i] | 1 | 1 | 1 | 2 | 2 | 3 | 4 | 4 |

Maximum = 4. One LIS: 2, 5, 7, 101.

## Complexity Analysis
Derivation (O(n²) method): for each `i` the inner loop runs `i` times, so the total number of comparisons is `0 + 1 + … + (n−1) = n(n−1)/2`, which is Θ(n²) **in every case** (the loops do not depend on the data).

| Case | Time |
|------|------|
| Best  | Θ(n²) |
| Average | Θ(n²) |
| Worst | Θ(n²) |

* **Space:** `lis`, `prev` → Θ(n).
* O(n log n) check method: each of the `n` elements needs one binary search over at most `n` entries → O(n log n) time, O(n) space.
