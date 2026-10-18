# Q2 — Dynamic Connected Components in a Grid (Number of Islands II)

> **Technique:** DSU on flattened coordinates · **Time:** O(R·C + K·α(R·C)) · **Space:** O(R·C)

## 1. Problem

An `R × C` grid starts as all water. `K` land cells `(r₁,c₁), (r₂,c₂), …, (r_k,c_k)` are added one by one.
After **each** addition print the number of islands (4-directionally connected land components).

## 2. Input representation (and why)

| Choice | Reason |
|---|---|
| **Implicit grid graph** | The neighbours of `(r,c)` are `(r±1,c)` and `(r,c±1)`; no adjacency list is needed. |
| **Coordinate flattening** `id = r·C + c` | Maps 2-D cells to 1-D DSU indices (the hint in the question). |
| `parent[id] = -1` ⇒ water | Doubles as the "is land?" flag, so no separate boolean grid. |

```
R C K
r1 c1
r2 c2
...            (0-indexed: 0 ≤ r < R, 0 ≤ c < C)
```

## 3. Key idea

Land is only **added**, never removed, so islands only appear or merge — exactly what DSU supports.

When land is added at `id`:

1. Make it its own set → `islands += 1`.
2. For each of its 4 neighbours that is land, `union(id, neighbour)`.
   Every union that actually joins two **different** sets → `islands -= 1`.

## 4. Algorithm

```
count ← 0;  parent[·] ← -1                       // all water
for each operation (r, c):
    if out of range:            report count (ignored)
    id ← r·C + c
    if parent[id] ≠ -1:         report count (already land – duplicate)
    else:
        parent[id] ← id;  size[id] ← 1;  count ← count + 1
        for (nr, nc) in {(r-1,c),(r+1,c),(r,c-1),(r,c+1)}:
            if inside grid and parent[nr·C+nc] ≠ -1:
                if UNION(id, nr·C+nc): count ← count - 1
        report count

UNION(a, b):  ra ← find(a), rb ← find(b)
              if ra == rb: return false
              attach smaller under larger (union by size); return true
find(x):      path halving, iterative
```

## 5. Correctness (sketch)

*Invariant:* after every step, DSU sets = maximal 4-connected groups of land cells, and `count` = number of sets.
Adding a cell creates a new group (+1). Each neighbour group that was distinct from the cell's current group
is merged into it; every successful merge reduces the number of groups by exactly one (−1), and a neighbour
already in the same group (e.g. two neighbours linked through another route) causes no change. ∎

Duplicate land operations are idempotent (no change); this is why the `parent[id] ≠ -1` check matters.

## 6. Complexity analysis

| Part | Cost |
|---|---|
| Initialise `R·C` cells | O(R·C) |
| One operation: ≤ 4 unions, each 2 `find`s | O(α(R·C)) amortised |
| **K operations** | **O(K·α(R·C))** |
| **Total time** | **O(R·C + K·α(R·C))** |
| Space | O(R·C) for `parent[]` and `size[]` |

(Lazy alternative: a hash map keyed by `id` reduces space to O(K) when `R·C ≫ K`.)
*Naïve alternative:* re-run BFS/DFS after each addition → O(K·R·C).

## 7. Sample run

`sample_input.txt` (3 × 3 grid, 6 operations — the last one is a duplicate)
```
3 3 6
0 0
0 1
1 2
2 1
1 1
1 1
```

`sample_output.txt`
```
After op 1: land (0,0) -> islands = 1
After op 2: land (0,1) -> islands = 1
After op 3: land (1,2) -> islands = 2
After op 4: land (2,1) -> islands = 3
After op 5: land (1,1) -> islands = 1     <- (1,1) touches (0,1),(1,2),(2,1): three islands merge
After op 6: land (1,1) -> islands = 1 (already land)
Final grid ('#' land, '.' water):
##.
.##
.#.
```

## 8. Build & run

```bash
gcc -O2 -Wall -o q2 solution.c
./q2 < sample_input.txt
```

## 9. Edge cases handled

duplicate positions · out-of-range positions (ignored, reported) · `1 × 1` grid · `K = 0` · very large grids
(iterative `find`, no recursion) · malformed input.
