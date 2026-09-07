# Question 3 — Longest Common Subsequence (LCS) using Dynamic Programming

## 1. Problem Statement
Given two strings `X` (length `m`) and `Y` (length `n`), find the length of
their **Longest Common Subsequence** (a subsequence need not be
contiguous, but must preserve relative order) and display the
subsequence itself. Analyze the algorithm's complexity.

## 2. Why Dynamic Programming?
A naive approach checks all 2ᵐ subsequences of `X` against `Y`, giving
**O(2ᵐ · n)** time. The LCS problem has:
- **Optimal Substructure**: LCS(X[1..i], Y[1..j]) can be expressed in
  terms of LCS on smaller prefixes.
- **Overlapping Sub-problems**: the same prefix pair `(i, j)` is needed
  repeatedly by different recursive paths.

## 3. State Definition
Let `dp[i][j]` = length of the LCS of `X[1..i]` and `Y[1..j]`
(the first `i` characters of X and the first `j` characters of Y).

### Recurrence
```
dp[i][0] = dp[0][j] = 0                       for all i, j   (empty string has LCS 0)

if X[i] == Y[j]:
    dp[i][j] = dp[i-1][j-1] + 1               (the matching char extends the LCS)
else:
    dp[i][j] = max( dp[i-1][j], dp[i][j-1] )  (drop a char from X or from Y)
```
Final answer: `dp[m][n]`.

## 4. Algorithm (Pseudocode)
```
Algorithm LCS-LENGTH(X[1..m], Y[1..n]):
    for i <- 0 to m:
        for j <- 0 to n:
            if i == 0 or j == 0:
                dp[i][j] <- 0
            else if X[i] == Y[j]:
                dp[i][j] <- dp[i-1][j-1] + 1
            else:
                dp[i][j] <- max(dp[i-1][j], dp[i][j-1])
    return dp[m][n]
```

### Reconstructing the LCS string
Walk backward from `dp[m][n]`:
```
i <- m, j <- n
while i > 0 and j > 0:
    if X[i] == Y[j]:
        prepend X[i] to result; i--; j--
    else if dp[i-1][j] >= dp[i][j-1]:
        i--
    else:
        j--
```

## 5. Correctness
By induction on `i + j`:
- **Base case**: `dp[i][0] = dp[0][j] = 0` since an LCS with an empty
  string is empty.
- **Inductive step**: assume all `dp[i'][j']` with `i'+j' < i+j` are
  correct.
  - If `X[i] == Y[j]`, then this matching character *can* be part of
    some LCS (a classical exchange argument shows an optimal LCS can
    always be chosen to use the last matching characters when they are
    equal), so `dp[i][j] = dp[i-1][j-1] + 1`.
  - If `X[i] != Y[j]`, the LCS cannot use both last characters
    simultaneously, so the optimal LCS either ignores `X[i]`
    (`dp[i-1][j]`) or ignores `Y[j]` (`dp[i][j-1]`) — taking the max of
    these two (already-correct, by the inductive hypothesis) sub-problems
    gives the correct value for `dp[i][j]`. ∎

## 6. Complexity Analysis

| Metric | Complexity |
|--------|------------|
| Time   | **O(m · n)** — filling an (m+1)×(n+1) table, O(1) work per cell |
| Space  | **O(m · n)** for the full table (needed to reconstruct the actual subsequence); optimizable to **O(min(m,n))** using two rolling rows if only the *length* is required |

## 7. Input Representation
- Two strings `X` and `Y` (read as whitespace-free tokens, max length
  999 in this implementation — trivially extendable).
- A dynamically allocated 2-D DP table of size `(m+1) x (n+1)`.

## 8. Files
- `lcs.c` — builds the DP table, prints the LCS length, reconstructs
  and prints the actual LCS string, and reports the complexity.
