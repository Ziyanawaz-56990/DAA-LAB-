# Q3 — Longest Common Subsequence (LCS)

## Algorithm Name
Dynamic Programming for Longest Common Subsequence (table + traceback).

## Objective
Given two sequences `X` (length `m`) and `Y` (length `n`), find the **length** of their longest common subsequence and **print one such subsequence**. A subsequence keeps the original order but may skip elements (it need not be contiguous).

## Input representation
Two strings (no spaces), one per line:
```
X
Y
```
Characters are the sequence elements. They are stored in character arrays `X[0..m-1]`, `Y[0..n-1]`.

## Output
The length of the LCS and the LCS string.

## Definition of the table
`L[i][j]` = length of the LCS of the first `i` characters of `X` and the first `j` characters of `Y` (a table of size `(m+1) × (n+1)`).

## Recurrence
```
L[i][0] = 0,  L[0][j] = 0                          (empty prefix)
L[i][j] = L[i-1][j-1] + 1                if X[i-1] == Y[j-1]
L[i][j] = max( L[i-1][j], L[i][j-1] )    otherwise
```
*Optimal substructure:* if the last characters match, that character belongs to some LCS and the rest is an LCS of the shorter prefixes; if they differ, at least one of the two last characters is not used, so we drop one of them and take the better result.

## Algorithm Steps
1. Allocate `L` of size `(m+1) × (n+1)` and set row 0 and column 0 to 0.
2. For `i = 1 … m` and for `j = 1 … n`: apply the recurrence above.
3. `L[m][n]` is the length of the LCS.
4. **Reconstruction (traceback):** let `k = L[m][n] − 1`, `i = m`, `j = n`. While `i > 0` and `j > 0`:
   * if `X[i-1] == Y[j-1]`: store this character at position `k` of the answer, `k--`, `i--`, `j--`;
   * else if `L[i-1][j] >= L[i][j-1]`: `i--` (move up);
   * else: `j--` (move left).
5. The stored characters form the LCS (filled from the back, so no reversing is needed).

## Example / Dry Run
`X = ABCB`, `Y = BDCB`.

|   |   | B | D | C | B |
|---|---|---|---|---|---|
|   | 0 | 0 | 0 | 0 | 0 |
| A | 0 | 0 | 0 | 0 | 0 |
| B | 0 | 1 | 1 | 1 | 1 |
| C | 0 | 1 | 1 | 2 | 2 |
| B | 0 | 1 | 1 | 2 | **3** |

Length = 3. Traceback from `(4,4)`: B=B ✔, C=C ✔, then `B≠D` → move left, B=B ✔ → LCS = **BCB**.

## Complexity Analysis
Derivation: the table has `(m+1)(n+1)` cells and each cell takes O(1) time. The traceback moves one step up/left each time, so at most `m + n` steps.

| Case | Time |
|------|------|
| Best  | Θ(m·n) (every cell is still computed) |
| Average | Θ(m·n) |
| Worst | Θ(m·n) |

* **Space:** Θ(m·n) for the full table (needed for reconstruction). If only the length were needed, two rows would suffice: O(min(m, n)).
