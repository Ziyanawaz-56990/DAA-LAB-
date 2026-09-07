# Question 3 — O(n log n) Divide & Conquer Convolution (FFT)

## 1. Problem Statement
The convolution of two vectors `A` (length `m`) and `B` (length `n`,
with `n >= m`) is a new vector `C` such that

```
C[k] = Σ_{j=0}^{m-1} A[j] * B[k-j]
```

Find an **O(n log n)** divide-and-conquer algorithm for this operation,
and write a C program to validate it.

## 2. Why Naive Convolution Isn't Enough
Computing every `C[k]` directly from the summation costs `O(m)` work per
output index, and there are `O(m+n)` output indices, giving
**O(n·m)** — quadratic when `m` and `n` are close in size. We need a
fundamentally different approach to reach `O(n log n)`.

## 3. Key Idea — Convolution as Polynomial Multiplication
Treat `A` and `B` as coefficient vectors of two polynomials:
```
P_A(x) = A[0] + A[1]x + A[2]x² + ... + A[m-1]x^(m-1)
P_B(x) = B[0] + B[1]x + B[2]x² + ... + B[n-1]x^(n-1)
```
Then the coefficients of the product polynomial `P_A(x) · P_B(x)` are
**exactly** the convolution `C[k]` — multiplying polynomials and
convolving their coefficient vectors are the same operation.

Multiplying two polynomials by naive coefficient-by-coefficient
expansion is again **O(n·m)**. We instead use the classical
**evaluate → pointwise-multiply → interpolate** strategy:
1. Evaluate `P_A` and `P_B` at enough points (their degree + 1).
2. Multiply the *values* pointwise (O(n) — cheap!).
3. Interpolate back to get the product polynomial's coefficients.

Doing step 1/3 naively still costs `O(n²)`. The **Fast Fourier
Transform (FFT)** does steps 1 and 3 in `O(n log n)` by evaluating
(and interpolating) at the **complex n-th roots of unity**, using a
**Divide & Conquer** recursive structure (the Cooley–Tukey algorithm).

## 4. The FFT as Divide & Conquer
Split a degree-`(L-1)` polynomial `P(x)` into its even- and odd-indexed
coefficient sub-polynomials:
```
P_even(y) = a0 + a2*y + a4*y^2 + ...
P_odd(y)  = a1 + a3*y + a5*y^2 + ...
P(x) = P_even(x^2) + x * P_odd(x^2)
```
Because we only need to evaluate `P` at the `L` complex `L`-th roots of
unity `ω_L^k` (`k = 0..L-1`), and `(ω_L^k)^2` only takes `L/2` distinct
values, evaluating `P_even` and `P_odd` at these `L/2` points is enough
to reconstruct all `L` evaluations of `P` via the **butterfly
combination**:
```
P(ω_L^k)          = P_even(ω_{L/2}^k) + ω_L^k * P_odd(ω_{L/2}^k)
P(ω_L^(k+L/2))    = P_even(ω_{L/2}^k) - ω_L^k * P_odd(ω_{L/2}^k)
```
This is a genuine Divide & Conquer algorithm:
- **Divide**: split coefficients into even/odd halves — O(L).
- **Conquer**: recursively FFT each half of size L/2.
- **Combine**: the butterfly step combines the two half-results in O(L).

### Recurrence & Complexity
```
T(L) = 2*T(L/2) + O(L)   =>  T(L) = O(L log L)     (Master Theorem, case 2)
```
The **Inverse FFT** (used to interpolate back to coefficients) has an
identical structure (just conjugated angles, and a final division by
`L`), so it is also **O(L log L)**.

## 5. Full Convolution Algorithm
```
Algorithm FFT-CONVOLUTION(A[0..m-1], B[0..n-1]):
    convLen <- m + n - 1
    L <- smallest power of two >= convLen      // padding for radix-2 FFT
    pad A and B with zeros up to length L

    fa <- FFT(A, L)              // O(L log L), forward transform
    fb <- FFT(B, L)              // O(L log L), forward transform

    for i <- 0 to L-1:
        fa[i] <- fa[i] * fb[i]   // O(L), pointwise multiply in "value" space

    result <- INVERSE-FFT(fa, L) // O(L log L)
    divide every result[i] by L  // un-normalize the inverse transform

    return result[0 .. convLen-1]   // real parts, rounded
```
Since `n >= m`, `convLen = O(n)`, so `L = O(n)` and the **overall time
complexity is O(n log n)**, as required. Space is **O(L) = O(n)** for
the complex-valued transform arrays.

## 6. Correctness
By the **Convolution Theorem**: pointwise multiplication of two
functions' Fourier-domain representations equals the Fourier transform
of their convolution/product. Since the DFT (evaluated by FFT) computes
the exact polynomial evaluation at the roots of unity, and the values of
a degree-`(L-1)` polynomial at `L` distinct points uniquely determine its
coefficients (`L` equations, `L` unknowns, and a Vandermonde-type system
with distinct evaluation points is always invertible), the inverse FFT
exactly recovers `P_A(x)·P_B(x)`'s coefficients — which are exactly the
convolution `C[]`. This is verified numerically in the program by
cross-checking against a brute-force **O(n·m)** convolution
implementation (`naiveConvolution`); the two must agree up to floating
point rounding error.

## 7. Input Representation
- Two arrays of `double`, `A[0..m-1]` and `B[0..n-1]` with `n >= m`
  (as stated in the problem).
- Internally, `double complex` arrays (C99 `<complex.h>`) of length `L`
  (next power of two ≥ `m+n-1`) are used for the FFT.

## 8. Files
- `convolution.c` — implements the recursive radix-2 FFT (forward and
  inverse), the O(n log n) convolution pipeline, a brute-force
  O(n·m) reference implementation for cross-verification, and prints
  both results with the maximum absolute difference between them.
