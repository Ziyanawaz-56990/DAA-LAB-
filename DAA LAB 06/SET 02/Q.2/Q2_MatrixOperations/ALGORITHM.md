# Question 2 — 2-D Square Matrix Operations and their Worst-Case Complexities

## 1. Problem Statement
Given square matrices with `n` rows and `n` columns, determine the
worst-case computational complexity (in terms of `n`) of, and implement
in C: (i) addition (ii) multiplication (iii) zero-matrix check
(iv) symmetric check (v) determinant (vi) in-place transpose
(vii) eigenvalue & eigenvector.

## 2. Input Representation
Two dynamically allocated `n x n` matrices of `double`, stored as arrays
of row-pointers (`double **`). `double` is used (rather than `int`) so
that Question (vii)'s iterative eigen-decomposition can converge to a
non-integer numerical answer.

## 3. Per-Operation Algorithm & Complexity

### (i) Matrix Addition — O(n²)
`C[i][j] = A[i][j] + B[i][j]` for every cell — exactly `n²` additions,
each O(1): **Θ(n²)**, and this is optimal since every one of the `n²`
output cells must be produced.

### (ii) Matrix Multiplication — O(n³) (naive)
```
for i <- 0..n-1:
  for j <- 0..n-1:
     C[i][j] <- Σ_{k=0}^{n-1} A[i][k]*B[k][j]
```
Three nested loops give **O(n³)**. (Strassen's algorithm improves this
to O(n^2.807); the Coppersmith–Winograd family goes lower still, but the
straightforward triple-loop is what's implemented here for clarity.)

### (iii) Zero-matrix check — O(n²)
Scan all `n²` cells; return false as soon as any non-zero entry is
found (early-exit optimizes the *average* case, but the worst case,
e.g. an all-zero matrix, still requires all `n²` cells): **O(n²)**.

### (iv) Symmetric check — O(n²)
A matrix is symmetric iff `A[i][j] == A[j][i]` for all `i < j`. Checking
the upper triangle (≈ n²/2 comparisons) suffices: **Θ(n²)**.

### (v) Determinant
- **Cofactor (Laplace) expansion** (implemented here, matches what's
  taught for 2x2/3x3 by hand): expanding along the first row makes `n`
  recursive calls on (n-1)x(n-1) minors, giving the recurrence
  `T(n) = n * T(n-1) + O(n²)`, which solves to **O(n!)** — factorial time,
  impractical beyond n ≈ 10-12.
- **Practical alternative**: **LU / Gaussian-elimination based
  determinant** reduces the matrix to upper-triangular form in O(n³)
  and multiplies the diagonal, giving **O(n³)** — this is the method
  used in real numerical libraries (LAPACK, NumPy, etc.) and is
  mentioned here as the recommended production approach.

### (vi) Transpose in-place (in situ) — O(n²)
Since the matrix is square, swap `A[i][j]` with `A[j][i]` for all `i < j`
(the diagonal doesn't move): **Θ(n²/2) = Θ(n²)** swaps, **O(1)** extra
space — this is only possible in-place because the matrix is *square*;
a non-square matrix's transpose changes its dimensions and cannot be
done truly in-place without a permutation-cycle algorithm.

### (vii) Eigenvalue & Eigenvector — Power Iteration, O(k·n²)
Finding **all** eigenvalues/eigenvectors of a general `n x n` matrix in
exact closed form is only possible via the characteristic polynomial for
`n <= 4` (Abel–Ruffini: no general algebraic solution exists for
polynomial roots when the polynomial's degree is 5 or higher). For
general `n`, numerical iterative methods are required:
- **Power Iteration** (implemented here): repeatedly compute
  `v <- A·v`, normalize `v`, and the normalization constant converges to
  the eigenvalue of **largest magnitude** (the "dominant" eigenvalue),
  with `v` converging to its eigenvector. Cost per iteration is a
  matrix-vector product, O(n²); with `k` iterations to reach the
  convergence tolerance, total cost is **O(k·n²)**.
- Finding **all** eigenvalues numerically (e.g. via the **QR algorithm**)
  costs **O(n³)** per iteration and is beyond this lab's scope, but is
  the standard method used by libraries like LAPACK.

```
Algorithm POWER-ITERATION(A, n, maxIter, tol):
    v <- [1,1,...,1]
    repeat up to maxIter times:
        w <- A * v                       // O(n^2)
        lambda <- component of w with largest magnitude
        v <- w / lambda                  // normalize
        if |lambda - previous_lambda| < tol: break
    return lambda, v
```

## 4. Summary Table

| # | Operation                | Complexity |
|---|---------------------------|------------|
| i | Addition                   | O(n²) |
| ii| Multiplication (naive)     | O(n³) |
| iii| Zero-matrix check          | O(n²) |
| iv| Symmetric check             | O(n²) |
| v | Determinant (cofactor)      | O(n!) — O(n³) via LU decomposition |
| vi| Transpose (in-place)        | O(n²), O(1) extra space |
| vii| Eigenvalue/vector (dominant, power iteration) | O(k·n²) |

## 5. Files
- `matrix_ops.c` — implements and demonstrates all seven sub-problems on
  user-supplied square matrices, printing each result and its
  complexity class.
