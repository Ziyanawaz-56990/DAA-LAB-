# Q3 — Maximum Weight Component with Size Constraint

> **Technique:** DSU with (size, weight-sum) + max-heap with lazy deletion · **Time:** O((V + E)·α(V) + V log V) · **Space:** O(V)

## 1. Problem

Each vertex `v` has a weight `w_v`. Edges (with weights) are added one by one. At any moment report the
**maximum total vertex weight of a connected component whose size (number of vertices) is ≤ K**.

## 2. Interpretation & assumptions

* "Size" = number of vertices in the component. Vertex weights may be any integers (negatives are handled).
* The answer is reported **initially** (all vertices are singleton components) and **after every edge**.
* If no component has size ≤ K (e.g. `K = 0`, or everything merged into one big component) the program prints `NONE`.
* The **edge weight** does not affect connectivity or the vertex-weight sum, so it is read but not used.
  (It is kept in the input format because the question states edges arrive with weights.)

## 3. Input representation (and why)

| Choice | Reason |
|---|---|
| Vertex-weight array `w[1..V]` | Needed once to initialise the component sums. |
| **Edge stream** `(u, v, wt)` | Edges arrive sequentially; connectivity is tracked purely by DSU. No adjacency list required. |
| DSU arrays `parent`, `size`, `sum` | Component size and total weight are maintained at the root. |
| Binary **max-heap** of candidates | Gives the best valid component in O(log V) instead of scanning all components. |

```
V K E
w1 w2 ... wV
u1 v1 wt1
...            (vertices numbered 1..V)
```

## 4. Key idea

1. Components only **merge** (edges are never deleted), so a component whose size exceeds `K` is
   **invalid forever**.
2. Merging two valid components can *lower* the global maximum (the best component may be swallowed by a
   bigger, invalid one). So a plain running `max` is **wrong** — we need a structure that supports removal.
3. **Lazy deletion:** push a heap entry `(sum, root, size)` for every component version that is valid when it
   is created. Never remove entries eagerly. When querying, pop from the top while the entry is **stale**:

```
entry (sum, root, size) is current  ⇔  parent[root] == root   (still a root)
                                     ∧ size[root] == size      (has not grown since)
                                     ∧ size ≤ K
```

   Sizes strictly increase on every merge, so `size` acts as a *version number*.

## 5. Algorithm

```
INIT:  for each v: parent[v] ← v, size[v] ← 1, sum[v] ← w_v
                   if 1 ≤ K: HEAP.push(w_v, v, 1)
QUERY: while HEAP non-empty and top is stale: HEAP.pop()
       return HEAP empty ? NONE : top.sum

PROCESS edge (u, v, wt):
       a ← find(u), b ← find(v)
       if a ≠ b:
           attach smaller root under larger (union by size)    // say b under a
           size[a] += size[b];  sum[a] += sum[b]
           if size[a] ≤ K: HEAP.push(sum[a], a, size[a])
       print QUERY
```

## 6. Correctness (sketch)

*Invariant:* the heap contains an entry for **every** currently valid component (its latest version), plus possibly
stale entries. A stale entry is detected by the test above, so after the `while` loop the top is a genuine valid
component with the largest sum among all valid components. Components with `size > K` are never pushed and cannot
become valid again, because sizes never shrink. ∎

## 7. Complexity analysis

| Part | Cost |
|---|---|
| Build initial heap (V pushes) | O(V log V) |
| `find` / union per edge | O(α(V)) amortised → O(E·α(V)) |
| Pushes | ≤ V (initial) + (V − 1) (merges) < 2V |
| Pops | each entry is popped **at most once** ⇒ ≤ 2V pops overall → O(V log V) |
| `valid()` check | O(1) (no `find` needed) |
| **Total time** | **O((V + E)·α(V) + V log V)** |
| Space | **O(V)** (DSU arrays + heap ≤ 2V entries) |

*Naïve alternative:* rescan all components after every edge → O(E·V).

## 8. Sample run

`sample_input.txt` (V = 6, K = 3)
```
6 3 5
5 3 8 2 7 4
1 2 10
3 4 5
2 3 7
5 6 1
1 5 9
```

`sample_output.txt`
```
Initially: max = 8                          (best singleton is vertex 3)
After edge 1 (1,2,wt=10): max = 8          ({1,2}=8 ties with {3}=8)
After edge 2 (3,4,wt=5): max = 10          ({3,4}=10)
After edge 3 (2,3,wt=7): max = 7           ({1,2,3,4} has size 4 > K → invalid; best left is {5}=7)
After edge 4 (5,6,wt=1): max = 11          ({5,6}=11)
After edge 5 (1,5,wt=9): max = NONE        (everything is one component of size 6 > K)
```
Edge 3 shows why a plain running maximum would be wrong: the answer **drops** from 10 to 7.

## 9. Build & run

```bash
gcc -O2 -Wall -o q3 solution.c
./q3 < sample_input.txt
```

## 10. Edge cases handled

`K = 0` or `K ≥ V` · negative vertex weights · repeated or self-loop edges (no merge, answer unchanged) ·
out-of-range endpoints (edge skipped) · 64-bit sums (`long long`) · malformed input.
