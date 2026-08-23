# Q2 — Pair from S1, S2 summing to x — O(n·log n)

## Problem
Given sets `S1`, `S2` (each size `n`) and target `x`, determine whether
some `a ∈ S1, b ∈ S2` satisfy `a + b = x`.

## Algorithm

```
findPairWithSum(S1, S2, x):
    sorted2 = sort(S2)                      // O(n log n)
    for each a in S1:                       // n iterations
        need = x - a
        idx  = binarySearch(sorted2, need)  // O(log n)
        if idx found:
            return (a, sorted2[idx])
    return NOT FOUND
```

## Reasoning
`a + b = x  ⟺  b = x - a`. So for every `a`, we don't need to scan all
of `S2` — we only need to check whether the *specific* value `x - a`
exists in `S2`. Sorting `S2` once lets every such check run in
`O(log n)` via binary search, instead of `O(n)` per check (which would
give an `O(n²)` brute-force overall).

Only ONE of the two sets needs sorting; the other is simply iterated.

## Complexity
| | |
|---|---|
| Time  | O(n log n) — O(n log n) sort + n × O(log n) binary searches |
| Space | O(n) — sorted copy of S2 |

## Worked example
S1 = {8,1,14,5,20,3}, S2 = {13,7,2,19,9,4}, x = 22

sorted S2 = {2,4,7,9,13,19}. Try each `a` in S1: need = 22-a.
- a=8  → need=14 → not in S2
- a=1  → need=21 → not in S2
- a=14 → need=8  → not in S2
- a=5  → need=17 → not in S2
- a=20 → need=2  → **found!** → pair (20, 2)

Result: 20 + 2 = 22 ✔
