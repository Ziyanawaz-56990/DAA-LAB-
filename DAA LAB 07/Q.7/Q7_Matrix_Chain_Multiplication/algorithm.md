# Q7 — Matrix Chain Multiplication (MCM)

## Problem
Given a chain of `n` matrices `A1, A2, ..., An` where `Ai` has dimensions
`p[i-1] x p[i]`, find the order of multiplying them (i.e. the
parenthesisation) that minimises the total number of scalar
multiplications, and report both that minimum and the corresponding
optimal parenthesisation.

## Key Idea — Dynamic Programming over Interval Length
Let `m[i][j]` = minimum number of scalar multiplications needed to
compute the product `Ai * Ai+1 * ... * Aj`. If the chain is split at
some point `k` (`i <= k < j`) into `(Ai..Ak) * (Ak+1..Aj)`, the cost is:

```
m[i][j] = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j]
```

(the last term is the cost of multiplying the two resulting matrices,
which have dimensions `p[i-1] x p[k]` and `p[k] x p[j]`). We try every
split point `k` and keep the minimum. The base case is a single matrix:
`m[i][i] = 0` (no multiplication needed).

```
m[i][j] = min over k in {i..j-1} of  m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j]
```

Because `m[i][j]` depends only on shorter sub-chains, we compute `m` by
increasing **chain length** `L = 2, 3, ..., n`, filling the DP table
bottom-up. A companion table `s[i][j]` records the best split point `k`
for each `(i, j)`, which is used afterwards to reconstruct the actual
optimal parenthesisation.

## Algorithm
```
MATRIX-CHAIN-ORDER(p[0..n]):
    let m[1..n][1..n] and s[1..n][1..n] be new tables
    for i in 1..n: m[i][i] <- 0
    for L in 2..n:                       # L = chain length
        for i in 1..n-L+1:
            j <- i + L - 1
            m[i][j] <- INFINITY
            for k in i..j-1:
                cost <- m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j]
                if cost < m[i][j]:
                    m[i][j] <- cost
                    s[i][j] <- k
    return m, s

PRINT-OPTIMAL-PARENS(s, i, j):
    if i == j: print "A" + i
    else:
        print "("
        PRINT-OPTIMAL-PARENS(s, i, s[i][j])
        PRINT-OPTIMAL-PARENS(s, s[i][j]+1, j)
        print ")"
```

## Complexity Analysis
* The DP table has `O(n²)` entries `m[i][j]`.
* Filling each entry tries `O(n)` split points.
* **Total: `O(n³)` time, `O(n²)` space.**
* Reconstructing and printing the optimal parenthesisation from `s`
  takes an additional `O(n)` time.

This is the standard, well-known optimal bound for the matrix chain
ordering problem (it is one of the canonical dynamic-programming
examples).
