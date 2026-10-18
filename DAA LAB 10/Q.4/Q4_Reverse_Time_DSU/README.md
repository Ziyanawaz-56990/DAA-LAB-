# Q4 — Reverse Time-Step DSU Application

> **Technique:** process deletions in reverse (as insertions) with DSU · **Time:** O((V + E)·α(V)) · **Space:** O(V + E)

## 1. Problem

A vertex-weighted undirected graph loses vertices one by one in the order `P = [p₁, p₂, …, p_V]`
(deleting a vertex removes it **and all its incident edges**). After **each** deletion, output the
**maximum sum of vertex values over all remaining connected components**.

## 2. Interpretation & assumptions

* Vertex values are **non-negative** (the program checks this and stops with an error otherwise).
  This makes the running-maximum in the reverse pass valid, because a merged component is never smaller than its parts.
* When no vertex remains (after the last deletion) the answer is `0`.
* `P` must be a permutation of `1..V` (validated).

## 3. Input representation (and why)

| Choice | Reason |
|---|---|
| **Adjacency list** (forward-star: `head[]`, `nxt[]`, `to[]`) | When a vertex is re-inserted we must enumerate **its neighbours**. Edge list would cost O(E) per vertex; adjacency matrix O(V) per vertex and O(V²) memory. |
| DSU arrays `parent`, `size`, `sum` | Maintain component sums under merges. |
| `active[]` flags | Tell whether a neighbour is already (re-)inserted. |

```
V E
w1 w2 ... wV
u1 v1
...            (E edges, 1-indexed)
p1 p2 ... pV   (deletion order)
```

## 4. Key idea — "reverse time"

DSU supports **union**, not **delete**. Run the timeline backwards:

```
state after i deletions  =  graph induced by { p_{i+1}, …, p_V }
```

* Start with **all vertices deleted** (empty graph → answer 0).
* Walk `i = V, V−1, …, 1`. Before inserting `p_i`, the current best equals the answer **after `i` deletions**
  (the vertices `p_{i+1..V}` are present). Record it.
* Insert `p_i`: mark active, make it a singleton with sum `w[p_i]`, union with every **already active** neighbour,
  and update `best ← max(best, sum[find(p_i)])`.

Only the component that contains the new vertex can change, so the running maximum is correct.

## 5. Algorithm

```
best ← 0
for i ← V down to 1:
    ans[i] ← best                         // answer after deleting p_1 … p_i
    v ← p_i
    active[v] ← true;  parent[v] ← v;  size[v] ← 1;  sum[v] ← w[v]
    for each neighbour x of v with active[x]:
        UNION(v, x)                       // sum and size add up at the root
    best ← max(best, sum[find(v)])
print ans[1], ans[2], …, ans[V]
```

## 6. Correctness (sketch)

*Invariant after inserting `p_i`:* DSU components = connected components of the subgraph induced by
`{p_i, …, p_V}`, each root storing its component's total value; `best` = maximum over them.
Inserting `v` merges exactly the components adjacent to `v` into one, which is the only change in the
component structure; with non-negative weights the merged value ≥ each part, so `best` can only increase
and `max(best, merged)` is exact. The deleted-vertex graph after `i` deletions is the induced subgraph on
`{p_{i+1}, …, p_V}`, whose best value was stored in `ans[i]` just before `p_i` was re-inserted. ∎

## 7. Complexity analysis

| Part | Cost |
|---|---|
| Build adjacency list | O(V + E) |
| Each vertex inserted once; each edge examined from both endpoints once | O(V + E) neighbour checks |
| Each `UNION` | O(α(V)) amortised |
| **Total time** | **O((V + E)·α(V))** — near-linear |
| Space | **O(V + E)** |

*Naïve alternative:* recompute components with BFS after every deletion → O(V·(V + E)).

## 8. Sample run

`sample_input.txt` — a path graph 1–2–3–4–5, values 4 2 7 1 3, deletion order 3 1 5 2 4
```
5 4
4 2 7 1 3
1 2
2 3
3 4
4 5
3 1 5 2 4
```

`sample_output.txt`
```
After deleting vertex 3: max component sum = 6     ({1,2}=6, {4,5}=4)
After deleting vertex 1: max component sum = 4     ({2}=2, {4,5}=4)
After deleting vertex 5: max component sum = 2     ({2}=2, {4}=1)
After deleting vertex 2: max component sum = 1     ({4}=1)
After deleting vertex 4: max component sum = 0     (empty graph)
```

## 9. Build & run

```bash
gcc -O2 -Wall -o q4 solution.c
./q4 < sample_input.txt
```

## 10. Edge cases handled

self-loops and parallel edges (union is idempotent) · isolated vertices · `V = 1` · `E = 0` ·
invalid deletion order / negative weights / bad edges (error message, exit code 1) · 64-bit sums.
