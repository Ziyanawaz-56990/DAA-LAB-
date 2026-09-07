# Question 4 — Matrix Chain Multiplication using Dynamic Programming

## 1. Problem Statement
Given the dimensions of `N-1` matrices in an array `arr[0..N-1]` (matrix
`i` has dimensions `arr[i-1] x arr[i]`), determine the **minimum number
of scalar multiplications** required to compute the product of the
complete matrix chain `A1 x A2 x ... x A(N-1)` (matrix multiplication is
associative, so the *order* of multiplying — i.e. the parenthesization —
affects the total scalar multiplication count, but not the final result).

**Sample Input**  : `N = 4`, `arr[] = {10, 30, 5, 60}`
**Sample Output**  : `4500`
**Time Complexity**: `O(N³)`

## 2. Why Dynamic Programming?
There are `Catalan(N-2)` distinct ways to parenthesize a chain of `N-1`
matrices — an exponential number. The problem exhibits:
- **Optimal Substructure**: the optimal way to multiply matrices `i..j`
  contains, within it, optimal ways to multiply matrices `i..k` and
  `k+1..j` for the best split point `k`.
- **Overlapping Sub-problems**: sub-chains like `(2,3)` are required by
  many different larger splits.

## 3. State Definition
Let `dp[i][j]` = minimum number of scalar multiplications needed to
compute the product `Ai x A(i+1) x ... x Aj` (1-indexed matrices).

### Recurrence
```
dp[i][i] = 0                                              (single matrix, no multiplication needed)

dp[i][j] = min over k in [i, j-1] of:
                dp[i][k] + dp[k+1][j] + arr[i-1]*arr[k]*arr[j]
```
Here `arr[i-1] x arr[k]` is the dimension of the matrix produced by
`Ai..Ak`, and `arr[k] x arr[j]` is the dimension of the matrix produced
by `A(k+1)..Aj`; multiplying these two resulting matrices costs
`arr[i-1] * arr[k] * arr[j]` scalar multiplications.

Final answer: `dp[1][N-1]`.

## 4. Algorithm (Pseudocode)
```
Algorithm MATRIX-CHAIN-ORDER(arr[0..N-1]):
    let M = N - 1                        // number of matrices
    for i <- 1 to M: dp[i][i] <- 0

    for len <- 2 to M:                    // chain length being solved
        for i <- 1 to M-len+1:
            j <- i + len - 1
            dp[i][j] <- INFINITY
            for k <- i to j-1:
                cost <- dp[i][k] + dp[k+1][j] + arr[i-1]*arr[k]*arr[j]
                if cost < dp[i][j]:
                    dp[i][j] <- cost
                    split[i][j] <- k       // remember the best split for backtracking

    return dp[1][M]
```

### Reconstructing the optimal parenthesization
Using the `split[][]` table recursively:
```
PRINT-OPTIMAL-PARENS(split, i, j):
    if i == j: print "Ai"; return
    print "("
    PRINT-OPTIMAL-PARENS(split, i, split[i][j])
    PRINT-OPTIMAL-PARENS(split, split[i][j]+1, j)
    print ")"
```

## 5. Correctness
By induction on the chain length `len = j - i + 1`:
- **Base case (len = 1)**: a single matrix requires 0 multiplications —
  trivially true.
- **Inductive step**: assume `dp[i'][j']` is correct for all shorter
  chains. Any parenthesization of `Ai..Aj` must have *some* outermost
  split point `k` (the final multiplication combines `Ai..Ak` with
  `A(k+1)..Aj`). By the inductive hypothesis, `dp[i][k]` and `dp[k+1][j]`
  already hold the true minimum costs for those sub-chains. Trying every
  possible `k` and taking the minimum total cost therefore finds the true
  global minimum for `dp[i][j]`. ∎

## 6. Complexity Analysis

| Metric | Complexity |
|--------|------------|
| Time   | **O(N³)** — O(N²) `(i, j)` pairs, each considering O(N) split points `k` |
| Space  | **O(N²)** — the `dp` and `split` tables |

## 7. Verified Sample Run (matches the assignment's example exactly)
```
Input:  N = 4, arr[] = {10, 30, 5, 60}
Output: 4500
Optimal Parenthesization: ((A1 A2) A3)
Time Complexity: O(N^3)
```
A second classical textbook test (CLRS) with
`N = 6, arr[] = {30, 35, 15, 5, 10, 20}` yields **11875** with
parenthesization `((A1(A2A3))(A4A5))` — see `output.txt` in the parent
Set-1 folder for the exact program transcript.

## 8. Input Representation
- Integer `N` (size of the dimension array; number of matrices = N-1).
- Array `arr[0..N-1]` of matrix dimensions.
- Two dynamically allocated 2-D tables `dp` and `split` of size `N x N`.

## 9. Files
- `mcm.c` — builds the DP and split tables, prints the minimum scalar
  multiplication count, the optimal parenthesization, and the complexity.
