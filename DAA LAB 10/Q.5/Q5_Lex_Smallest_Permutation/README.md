# Q5 — Lexicographically Smallest Connected-Component Permutation

> **Technique:** DSU components + bucket placement · **Time:** O((n + m)·α(n)) · **Space:** O(n)

## 1. Problem

Given a permutation `A` of `1..n` and a list of allowed swaps `(u, v)` (swap the elements at **positions**
`u` and `v`, any number of times, in any order), output the **lexicographically smallest** permutation reachable.

## 2. Input representation (and why)

| Choice | Reason |
|---|---|
| Positions as graph vertices, allowed swaps as an **edge list** | We only need connected components; a DSU consumes an edge list directly. No adjacency structure required. |
| `posOf[v]` (position of value `v`) | Lets us scan values in ascending order without sorting. |

```
n m
A1 A2 ... An         (a permutation of 1..n)
u1 v1
...                  (m swaps, positions 1..n)
```

## 3. Key idea

1. **Swaps generate everything inside a component.** If positions `i` and `j` are linked by a chain of allowed
   swaps, then using adjacent swaps along the chain (like bubble sort on a path) any value can be moved to any
   position of the component. Hence the reachable set = all rearrangements *within each connected component*,
   independently.
2. **Greedy per component:** to minimise lexicographically, the smallest available value must go to the
   smallest position, the next smallest to the next position, … i.e. **sort the component's values and place
   them on its sorted positions.**
3. **Linear-time sorting trick:** `A` is a permutation of `1..n`, so scanning positions `1..n` yields every
   component's positions in ascending order, and scanning values `1..n` yields every component's values in
   ascending order — no comparison sort needed (bucket / counting placement).

## 4. Algorithm

```
for each swap (u, v): UNION(u, v)                          // build components of positions
assign each component root r a contiguous block [off_r, off_r + size_r) in two arrays posList, valList

for i ← 1 … n:  r ← find(i);          posList[fillP[r]++] ← i      // positions, ascending
for v ← 1 … n:  r ← find(posOf[v]);   valList[fillV[r]++] ← v      // values,    ascending

for k ← 0 … n−1: result[ posList[k] ] ← valList[k]
print result
```

Inside each block both lists are ascending, so the k-th smallest position of a component receives the
k-th smallest value of that component.

## 5. Correctness (sketch)

*Reachability:* a component is connected, so every permutation of its values is reachable (adjacent
transpositions along a spanning tree generate the symmetric group on the component). Components don't interact.
*Optimality (exchange argument):* in the first position `p` (smallest index) of a component, putting any value
other than the component minimum makes the result larger at index `p` or earlier-differing — with all earlier
indices unaffected or fixed by other components — so the minimum must be placed at `p`; induct on the remaining
positions. ∎

## 6. Complexity analysis

| Part | Cost |
|---|---|
| Read input, validate permutation | O(n + m) |
| `m` unions | O(m·α(n)) amortised |
| Block offsets, position scan, value scan | O(n·α(n)) (one `find` per element) |
| Write result | O(n) |
| **Total time** | **O((n + m)·α(n))** |
| Space | **O(n)** |

*Sorting-based version:* sort each component's values → O(n log n); the bucket trick avoids the log factor.
*Naïve alternative:* BFS over the state space of permutations is exponential.

## 7. Sample run

`sample_input.txt`
```
7 4
5 2 6 1 7 3 4
1 2
2 3
4 5
6 7
```

Components of positions: `{1,2,3}`, `{4,5}`, `{6,7}`.

| Component | Positions | Values | Sorted values → placed |
|---|---|---|---|
| {1,2,3} | 1 2 3 | 5 2 6 | 2 5 6 |
| {4,5} | 4 5 | 1 7 | 1 7 |
| {6,7} | 6 7 | 3 4 | 3 4 |

`sample_output.txt`
```
Smallest permutation: 2 5 6 1 7 3 4
```

## 8. Build & run

```bash
gcc -O2 -Wall -o q5 solution.c
./q5 < sample_input.txt
```

## 9. Edge cases handled

no swaps (`m = 0` → permutation unchanged) · self-swaps `(u,u)` and repeated swaps · `n = 1` · fully connected
graph (output is `1 2 … n`) · input that is not a permutation or has out-of-range positions (error, exit code 1).
