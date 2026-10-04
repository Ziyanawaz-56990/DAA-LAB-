# Q8 — Optimal Binary Search Tree (OBST)

## Algorithm Name
Dynamic Programming for Optimal Binary Search Trees (the CLRS formulation, tables `e`, `w`, `root`).

## Objective
We have `n` sorted keys `k1 < k2 < … < kn`. A search is for key `k_i` with probability `p_i`, or it is an *unsuccessful* search that ends in the gap `d_i` (between `k_i` and `k_{i+1}`) with probability `q_i`, `i = 0 … n`. (`Σp_i + Σq_i = 1`.) Build the binary search tree with the **minimum expected search cost**:

```
E[cost] = Σ (depth(k_i)+1)·p_i  +  Σ (depth(d_i)+1)·q_i
```
(the root has depth 0, so "+1" counts the comparisons.)

## Input representation
```
n
p1 p2 ... pn         (n probabilities)
q0 q1 ... qn         (n+1 probabilities)
```
Stored in `double` arrays `p[1..n]`, `q[0..n]`.

## Output
The minimum expected search cost, the `root` table and the shape of the optimal tree.

## Definition of the tables (for 1 ≤ i ≤ n+1, i−1 ≤ j ≤ n)
* `e[i][j]` = expected cost of an optimal BST containing keys `k_i … k_j` (and dummy keys `d_{i-1} … d_j`).
* `w[i][j]` = total probability `p_i + … + p_j + q_{i-1} + … + q_j` of that subtree.
* `root[i][j]` = index of the key chosen as root of that optimal subtree.

## Recurrence
```
e[i][i-1] = q[i-1]            (empty subtree: only the dummy key d_{i-1})
w[i][i-1] = q[i-1]
w[i][j]   = w[i][j-1] + p[j] + q[j]
e[i][j]   = min over r = i..j of ( e[i][r-1] + e[r+1][j] + w[i][j] )
root[i][j]= the r that gives the minimum
```
*Why `+ w[i][j]`:* putting the subtree under a new root makes every node one level deeper, which adds its probability once more; the sum of all those probabilities is `w[i][j]`.

## Algorithm Steps
1. Read `n`, `p`, `q`; allocate `(n+2) × (n+2)` tables `e`, `w` and `root`.
2. **Base cases:** for `i = 1 … n+1`: `e[i][i-1] = w[i][i-1] = q[i-1]`.
3. For chain length `l = 1 … n` (increasing, so smaller subproblems are ready):
   * for `i = 1 … n−l+1`, set `j = i + l − 1`:
     * `e[i][j] = ∞`, `w[i][j] = w[i][j-1] + p[j] + q[j]`;
     * for each candidate root `r = i … j`: `cost = e[i][r-1] + e[r+1][j] + w[i][j]`; if `cost < e[i][j]`, set `e[i][j] = cost`, `root[i][j] = r`.
4. The answer is `e[1][n]`.
5. **Tree construction:** recursively, the root of keys `k_i…k_j` is `root[i][j]`; its left subtree is `(i, r−1)`, its right subtree is `(r+1, j)`; an empty range `i > j` is the dummy key `d_j`.

## Example / Dry Run
`n = 1`, `p1 = 0.5`, `q0 = q1 = 0.25`.
`e[1][0] = 0.25`, `e[2][1] = 0.25`, `w[1][1] = w[1][0] + p1 + q1 = 0.25 + 0.5 + 0.25 = 1.0`.
`e[1][1] = e[1][0] + e[2][1] + w[1][1] = 0.25 + 0.25 + 1.0 = 1.5`, root `k1`.

(The program also reproduces the well-known textbook example with `n = 5`, expected cost 2.75 — see `Outputs/Q08_output.txt`.)

## Complexity Analysis
Derivation: for chain length `l` there are `n − l + 1` subproblems, and each tries `l` roots (O(1) work each). Total candidate evaluations
`Σ_{l=1..n} (n − l + 1)·l = n(n+1)(n+2)/6`, which is Θ(n³).

| Case | Time |
|------|------|
| Best  | Θ(n³) |
| Average | Θ(n³) |
| Worst | Θ(n³) |

* **Space:** three tables of size about `(n+2)²` → Θ(n²). The recursion used to print the tree has depth at most `n`.
* (Knuth's optimisation can reduce the time to O(n²) but is not required here.)
