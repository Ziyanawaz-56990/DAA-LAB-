# Q1 — Online Graph-Bipartiteness Testing

> **Technique:** Disjoint Set Union with parity (2-colouring DSU) · **Time:** O((V + E)·α(V)) · **Space:** O(V)

## 1. Problem

An undirected graph on `V` vertices receives edges **online**, one at a time. A graph is bipartite
**iff it has no odd-length cycle**. For each arriving edge `(u, v)`:

* if adding it keeps the graph bipartite → **add it**;
* otherwise → **report a violation and skip it**.

## 2. Input representation (and why)

| Choice | Reason |
|---|---|
| **Edge stream**, read one edge at a time | The edges arrive online; we never need the whole graph. |
| No adjacency matrix / list | A BFS/DFS re-check per edge would cost O(V + E) each → O(E·(V + E)). The DSU forest alone is enough. |
| `parent[]`, `size[]`, `parity[]` arrays | O(V) memory in total. |

```
V E
u1 v1
u2 v2
...            (vertices numbered 1..V)
```

## 3. Key idea

Every connected component is 2-coloured. Store, for each vertex `x`,

```
parity[x] = colour(x) XOR colour(parent[x])
```

After path compression `parity[x]` becomes the colour of `x` **relative to its root**.
For an edge `(u, v)` with roots `ru = find(u)`, `rv = find(v)`:

| Case | Meaning | Action |
|---|---|---|
| `ru == rv` and `parity[u] == parity[v]` | `u`,`v` already forced to the **same** colour ⇒ edge closes an **odd cycle** | **REJECT** |
| `ru == rv` and `parity[u] != parity[v]` | edge closes an **even** cycle | accept (no structural change) |
| `ru != rv` | different components, always safe | **merge**, choosing the offset so that `u` and `v` get different colours |

Merging root `rv` under `ru` requires `colour(u) ≠ colour(v)`:

```
colour(u) = parity[u]            (relative to ru)
colour(v) = parity[v] XOR parity[rv]
need parity[u] ≠ parity[v] XOR parity[rv]   ⇒   parity[rv] = parity[u] XOR parity[v] XOR 1
```

(The formula is symmetric in `u` and `v`, so it is the same whichever root becomes the child.)
A self-loop `(u, u)` has `parity[u] == parity[u]` ⇒ always rejected (odd cycle of length 1).

## 4. Algorithm

```
find(x):                                   // returns root, compresses parity
    if parent[x] == x: return x
    p ← parent[x]
    r ← find(p)
    parity[x] ← parity[x] XOR parity[p]    // p is now relative to r
    parent[x] ← r
    return r

ADD-EDGE(u, v):
    ru ← find(u);  rv ← find(v)
    if ru == rv:
        return (parity[u] ≠ parity[v])     // false ⇒ odd cycle
    if size[ru] < size[rv]: swap(ru, rv)   // union by size
    parent[rv] ← ru
    parity[rv] ← parity[u] XOR parity[v] XOR 1
    size[ru]   ← size[ru] + size[rv]
    return true

MAIN:
    for each vertex i: parent[i] ← i, size[i] ← 1, parity[i] ← 0
    for each arriving edge (u, v):
        if ADD-EDGE(u, v): print ACCEPTED else print REJECTED (odd cycle)
```

## 5. Correctness (sketch)

*Invariant:* for any two vertices in the same DSU tree, `parity[a] XOR parity[b]` (after compression)
equals `colour(a) XOR colour(b)` in **every** valid 2-colouring of the accepted edges of that component.
Since a connected bipartite graph has exactly two proper 2-colourings (a colouring and its complement),
two vertices in one component are forced to have equal or different colours independent of the chosen one.
Hence an edge between equal-forced vertices makes a proper colouring impossible ⇔ it creates an odd cycle.
Rejected edges never modify the structure, so the invariant is preserved. ∎

## 6. Complexity analysis

| Quantity | Bound | Justification |
|---|---|---|
| `find` | O(α(V)) amortised | union by size + path compression (Tarjan 1975) |
| one edge | O(α(V)) amortised | two `find`s + O(1) work |
| **All E edges** | **O(V + E·α(V))** | initialisation O(V) + E edges |
| Space | **O(V)** | `parent`, `size`, `parity` |
| Recursion depth of `find` | O(log V) | union by size keeps trees ≤ log₂V high |

α is the inverse Ackermann function, α(V) ≤ 4 for every V < 10⁸⁰ — effectively constant.
*Naïve alternative:* BFS/DFS 2-colouring after each edge costs O(E·(V + E)).

## 7. Sample run

`sample_input.txt`
```
6 9
1 2
2 3
3 4
4 1
1 3
5 6
4 5
6 1
2 6
```

`sample_output.txt`
```
Edge 1: (1, 2) -> ACCEPTED
Edge 2: (2, 3) -> ACCEPTED
Edge 3: (3, 4) -> ACCEPTED
Edge 4: (4, 1) -> ACCEPTED            (even cycle 1-2-3-4)
Edge 5: (1, 3) -> REJECTED (odd cycle) (triangle 1-2-3)
Edge 6: (5, 6) -> ACCEPTED
Edge 7: (4, 5) -> ACCEPTED
Edge 8: (6, 1) -> ACCEPTED            (even cycle 1-4-5-6)
Edge 9: (2, 6) -> REJECTED (odd cycle) (2 and 6 have the same colour)
Summary: accepted = 7, rejected = 2
Colouring (0/1 relative to component root): 1:0 2:1 3:0 4:1 5:0 6:1
```
(The bracketed remarks are explanations added here; the program does not print them.)

## 8. Build & run

```bash
gcc -O2 -Wall -o q1 solution.c
./q1 < sample_input.txt
```

## 9. Edge cases handled

self-loops · duplicate edges · vertices out of range (ignored with a message) · `E = 0` · malformed input (error + exit code 1).
