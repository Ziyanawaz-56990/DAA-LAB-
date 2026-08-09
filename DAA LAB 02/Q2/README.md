# Q2 — Merge Sort vs Modified (3-way) Merge Sort

## Objective
The lab modifies Merge Sort by dividing the input into **thirds**, recursively
sorting each third, and combining them with a **three-way merge**. Find the
worst-case running time and validate it against ordinary (two-way) Merge Sort
with a C program.

## Theoretical Analysis

**Standard Merge Sort:** split into 2 halves, merge in Θ(n):
```
T(n) = 2T(n/2) + Θ(n)
```
By the Master Theorem (a=2, b=2, f(n)=Θ(n) = Θ(n^{log_b a})), this is case 2:
```
T(n) = Θ(n log n)
```

**Modified (3-way) Merge Sort:** split into 3 thirds, merge in Θ(n) (a 3-way
merge still does one O(1) "pick the smallest of the current 3 candidates" per
output element, so combining still costs Θ(n)):
```
T(n) = 3T(n/3) + Θ(n)
```
Here a=3, b=3, f(n)=Θ(n)=Θ(n^{log_b a}) again — case 2 of the Master Theorem:
```
T(n) = Θ(n log₃ n) = Θ(n log n)
```
(log₃ n = log₂ n / log₂ 3, a constant factor — the base of the logarithm never
changes the asymptotic class.)

**So both algorithms are Θ(n log n).** The recursion tree has fewer levels for
the 3-way version (log₃n vs log₂n ≈ 0.63 log₂n), but each 3-way merge step can
need up to **2** comparisons per output element (comparing the smallest of 3
candidates) instead of **1** comparison per output element for a 2-way merge.
These two effects roughly cancel, so the constant factor in front of `n log n`
is not obviously smaller for the 3-way version — this is exactly what the
experiment below checks rather than assumes.

## Experimental Design
`q2_merge_sort.c` implements both algorithms for real (no formulas), runs them
on **identical** randomized input for `n ∈ {30, 60, ..., 15360}`, counts true
comparisons and array writes, and **verifies** the output is sorted after
every run (the program aborts with an error message if either algorithm ever
produces an unsorted array).

## Results
Measured comparisons (selected rows from `q2_merge_sort.csv`):

| n | 2-way comparisons | 3-way comparisons | 2-way writes | 3-way writes |
|---|---|---|---|---|
| 480 | 3,657 | 4,476 | 8,576 | 5,224 |
| 3840 | 40,906 | 50,823 | 91,648 | 57,946 |
| 15360 | 194,398 | 241,435 | 428,032 | 263,366 |

Both curves track an `n log n` reference line closely on a log-log plot
(`plots/q2_loglog.png`) — confirming the Θ(n log n) class for **both**
algorithms. Interestingly:
- **3-way merge sort performs *more* comparisons** than 2-way (≈24% more at
  n=15360), because each output element in a 3-way merge can need up to two
  comparisons instead of one, and this outweighs having fewer recursion
  levels.
- **3-way merge sort performs *fewer* writes/copy-back passes** (≈38% fewer
  at n=15360), because there are only ⌈log₃n⌉ levels of "copy the merged
  range back into the array" instead of ⌈log₂n⌉.

So the modification does **not** change the asymptotic order — it only trades
comparisons for writes, which is a genuinely useful thing to observe
empirically rather than assume from the recurrence alone.

## Observations
- Both algorithms exhibit Θ(n log n) growth, matching the recurrence solution.
- The base of the logarithm (2 vs 3) only rescales the constant, never the
  asymptotic class.
- A "modification" to a divide-and-conquer algorithm can leave the Big-O
  class unchanged while still shifting real costs between different kinds of
  elementary operations (comparisons vs. writes) — a distinction Big-O alone
  cannot show, which is why the empirical counts matter.

## Files
- `q2_merge_sort.c` — real 2-way and 3-way merge sort + instrumentation + verification.
- `q2_merge_sort.csv` — measured comparisons/writes per n.
- `plot_q2.py` — regenerates the plots from the CSV.
- `plots/` — PNG + SVG charts (`q2_comparisons`, `q2_writes`, `q2_loglog`).
