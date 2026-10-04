# Q6 — Edit Distance with Traceback

## Algorithm Name
Dynamic Programming for Edit (Levenshtein) Distance, with traceback of the edit operations.

## Objective
Given strings `A` (length `m`) and `B` (length `n`), find the **minimum number of operations** — insert a character, delete a character, or substitute (replace) a character — needed to turn `A` into `B`, and print the sequence of operations (the traceback).

## Input representation
Two strings without spaces, one per line:
```
A
B
```

## Output
The minimum edit distance and the list of operations (Match / Replace / Delete / Insert) from the start of `A` to the end.

## Definition of the table
`D[i][j]` = edit distance between the first `i` characters of `A` and the first `j` characters of `B`. Table size `(m+1) × (n+1)`.

## Recurrence
```
D[i][0] = i          (delete all i characters)
D[0][j] = j          (insert all j characters)
if A[i-1] == B[j-1]:   D[i][j] = D[i-1][j-1]
else: D[i][j] = 1 + min( D[i-1][j],     // delete A[i-1]
                         D[i][j-1],     // insert B[j-1]
                         D[i-1][j-1] )  // replace A[i-1] by B[j-1]
```

## Algorithm Steps
1. Create `D` of size `(m+1) × (n+1)`; fill column 0 with `0,1,…,m` and row 0 with `0,1,…,n`.
2. For `i = 1 … m`, `j = 1 … n`: fill `D[i][j]` with the recurrence.
3. `D[m][n]` is the edit distance.
4. **Traceback:** start at `(i, j) = (m, n)` and repeat until `(0, 0)`:
   * if `A[i-1] == B[j-1]` and `D[i][j] == D[i-1][j-1]` → **Match**, move diagonally;
   * else if `D[i][j] == D[i-1][j-1] + 1` → **Replace** `A[i-1]` with `B[j-1]`, move diagonally;
   * else if `D[i][j] == D[i-1][j] + 1` → **Delete** `A[i-1]`, move up;
   * else → **Insert** `B[j-1]`, move left.
5. The operations are discovered from the end, so they are stored and printed in reverse order (start of `A` → end).

## Example / Dry Run
`A = cat`, `B = cut`

|   |   | c | u | t |
|---|---|---|---|---|
|   | 0 | 1 | 2 | 3 |
| c | 1 | 0 | 1 | 2 |
| a | 2 | 1 | 1 | 2 |
| t | 3 | 2 | 2 | **1** |

Distance = 1. Traceback: Match `c`, Replace `a`→`u`, Match `t`.

## Complexity Analysis
Derivation: `(m+1)(n+1)` cells with O(1) work each; the traceback takes at most `m + n` steps.

| Case | Time |
|------|------|
| Best  | Θ(m·n) (the whole table is always filled) |
| Average | Θ(m·n) |
| Worst | Θ(m·n) |

* **Space:** Θ(m·n) for the table (needed for the traceback) plus O(m+n) for the stored operations. Without traceback two rows suffice.
