# Q3 — Merging k Sorted Arrays

## Objective
Given `k` sorted arrays of `n` elements each (`kn` elements total), combine
them into one sorted array of `kn` elements using two strategies, find the
worst-case running time of each, and validate with a C program.

### Method 1 — Sequential
Merge array 1 with array 2, merge that result with array 3, ..., merge in
array `k` last.

### Method 2 — Balanced Pairwise (tournament)
Split the `k` arrays into `k/2` pairs, merge each pair, then repeat on the
`k/2` resulting arrays, and so on until one array remains.

## Theoretical Analysis

**Method 1:** the i-th merge combines a run of length `(i-1)n` (everything
merged so far) with a fresh run of length `n`, costing Θ(i·n). Summing:
```
Σ_{i=2}^{k} Θ(i·n) = Θ(n · Σ_{i=2}^{k} i) = Θ(n · k²)
```

**Method 2:** this is exactly the merge phase of merge sort, starting from
`k` sorted runs of length `n` instead of `k` single elements. There are
`Θ(log k)` levels, and at every level the total amount of data merged across
all pairs is `kn` elements, so each level costs Θ(kn):
```
Θ(log k) levels × Θ(kn) per level = Θ(nk log k)
```

| Method | Worst-case running time |
|---|---|
| Sequential (Method 1) | Θ(nk²) |
| Balanced pairwise (Method 2) | Θ(nk log k) |

For fixed `n`, Method 2 is asymptotically better by a factor of `k / log k`,
which grows without bound as `k` increases.

## Experimental Design
`q3_k_sorted_arrays.c` builds `k` **real** randomly generated sorted arrays
of `n` elements, then runs **both algorithms for real** (genuine merge
subroutine calls, not a formula) for every combination of
`k ∈ {2,4,8,16,32,64}` and `n ∈ {10,20,50,100,200}`. It counts true
comparisons/writes and **verifies**: (a) each method's output is fully
sorted, and (b) both methods agree element-for-element on the same input —
so a bug in either implementation would be caught immediately.

## Results
At `n = 200` (from `q3_k_sorted_arrays.csv`):

| k | Method 1 comparisons | Method 2 comparisons | Ratio (M1/M2) |
|---|---|---|---|
| 2 | 393 | 393 | 1.0× |
| 8 | 6,978 | 4,792 | 1.5× |
| 16 | 26,833 | 12,765 | 2.1× |
| 32 | 104,842 | 31,938 | 3.3× |
| 64 | 413,674 | 76,681 | 5.4× |

The gap widens quickly as `k` grows, exactly as `k²` vs `k log k` predicts.
On a log-log plot (`plots/q3_loglog_k.png`), Method 1 tracks a `k²` reference
curve and Method 2 tracks a `k log₂k` reference curve almost exactly.

## Observations
- At `k = 2` the two methods are identical (there's only one merge either
  way), so the counts match exactly — a useful sanity check.
- Sequential merging keeps re-touching the same, ever-growing accumulated
  array, which is what produces the quadratic-in-k blowup.
- Balanced merging keeps every array it touches roughly the same size at
  each level, which is what keeps the total work per level linear in `kn`.
- Both methods are linear in `n` for fixed `k` (merging is linear-time), so
  the entire advantage of Method 2 comes from how it manages `k`, not `n`.

## Files
- `q3_k_sorted_arrays.c` — real sequential and balanced-pairwise merging + instrumentation + verification.
- `q3_k_sorted_arrays.csv` — measured comparisons/writes per (k, n).
- `plot_q3.py` — regenerates the plots from the CSV.
- `plots/` — PNG + SVG charts (`q3_vs_k`, `q3_vs_n`, `q3_loglog_k`).
