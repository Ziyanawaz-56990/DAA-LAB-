# Q5 — Maximum Sum Increasing Subsequence (MSIS)

## Algorithm Name
Dynamic Programming for Maximum Sum Increasing Subsequence (a weighted variant of LIS).

## Objective
Given `n` positive integers `A[0..n-1]`, find the largest possible **sum** of a strictly increasing subsequence. (Here we maximise the *sum*, not the number of elements.)

## Input representation
```
n
a0 a1 ... a(n-1)     (positive integers)
```
Stored in an `int` array; sums are stored in `long long` so that they do not overflow for reasonable inputs.

## Output
The maximum sum (the program also prints the subsequence that achieves it).

## Definition of the table
`msis[i]` = maximum sum of a strictly increasing subsequence that **ends at index i**.
`prev[i]` = index of the previous element in that subsequence (−1 if none).

## Recurrence
```
msis[i] = A[i] + max{ msis[j] : j < i and A[j] < A[i] }     (just A[i] if no such j)
answer  = max over i of msis[i]
```
*Optimal substructure:* if a best subsequence ending at `A[i]` has `A[j]` just before it, the part ending at `A[j]` must itself be a best (maximum-sum) subsequence ending at `A[j]`.

## Algorithm Steps
1. For `i = 0 … n-1`:
   1. `msis[i] = A[i]`, `prev[i] = -1`.
   2. For `j = 0 … i-1`: if `A[j] < A[i]` and `msis[j] + A[i] > msis[i]`, set `msis[i] = msis[j] + A[i]` and `prev[i] = j`.
   3. Track `bestEnd`, the index with the largest `msis`.
2. The answer is `msis[bestEnd]`.
3. Reconstruction: follow `prev[]` from `bestEnd` to −1 and print the elements in reverse order.

Because all numbers are positive, extending a subsequence never decreases its sum, but the best choice is not always the longest one (see the example).

## Example / Dry Run
`A = [1, 101, 2, 3, 100, 4, 5]`

| i | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| A[i] | 1 | 101 | 2 | 3 | 100 | 4 | 5 |
| msis[i] | 1 | 102 | 3 | 6 | 106 | 10 | 15 |

Maximum = 106, from 1 + 2 + 3 + 100 (the longest increasing subsequence 1,2,3,4,5 only gives 15).

## Complexity Analysis
Derivation: the inner loop runs `i` times for each `i`, giving `n(n−1)/2` comparisons.

| Case | Time |
|------|------|
| Best  | Θ(n²) (the loops do not depend on the data) |
| Average | Θ(n²) |
| Worst | Θ(n²) |

* **Space:** `msis`, `prev` and the copy used for printing → Θ(n).
