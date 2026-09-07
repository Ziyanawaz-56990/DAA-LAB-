# Question 1 — 1-D Array Operations and their Worst-Case Complexities

## 1. Problem Statement
Given an array of `n` unsorted integers, determine the worst-case
complexity (in terms of `n`) of, and implement in C:

(i) maximum element (ii) first & second largest (iii) mean (iv) median
(v) standard deviation (vi) mode (vii) removing duplicates
(viii) reversing the array (ix) partitioning around a random pivot so
that all elements **less than** the pivot appear **after** all elements
**≥** the pivot.

## 2. Input Representation
A dynamically allocated 1-D array `int a[n]`, read from standard input
together with its length `n`. This representation is chosen because
every required operation (linear scan, in-place swap, sort) works
naturally and efficiently on a flat contiguous array.

## 3. Per-Operation Algorithm & Complexity

### (i) Maximum element — O(n)
```
max <- a[0]
for i <- 1 to n-1: if a[i] > max: max <- a[i]
return max
```
A single linear scan is both necessary and sufficient — no algorithm can
guarantee correctness examining fewer than `n-1` elements, since any
unexamined element could be the true maximum. **Θ(n)**.

### (ii) First and second largest — O(n)
Maintain two running variables `first`, `second` during a single pass;
update both when a new maximum is found, or just `second` when a new
"runner-up" is found. One pass suffices: **Θ(n)**.

### (iii) Mean — O(n)
`mean = (Σ a[i]) / n`. Summing requires touching every element once:
**Θ(n)**.

### (iv) Median — O(n log n) (this implementation), O(n) possible
The implementation sorts a *copy* of the array (`qsort`, O(n log n))
and returns the middle element(s). **Note:** the median can be found in
worst-case **O(n)** using the deterministic "median of medians"
(BFPRT) selection algorithm, or expected O(n) via randomized Quickselect
— the sort-based approach is used here for simplicity and because it
also directly enables (vi) and (vii) below on the same sorted copy.

### (v) Standard deviation — O(n)
`σ = sqrt( (Σ (a[i]-mean)²) / n )`. Requires one more linear pass over
the array once the mean is known: **Θ(n)** (Θ(n) total, since the mean
computation is also Θ(n)).

### (vi) Mode — O(n log n) (this implementation), O(n) with hashing
After sorting, the array's equal values become contiguous, so the
longest run of equal values (the mode) is found with one more linear
scan over the sorted array: **O(n log n)** total (dominated by the
sort). Using a hash table of value → frequency counts, this can be done
in **expected O(n)** instead, at the cost of O(n) extra hash-table space.

### (vii) Removing all duplicates — O(n log n) (this implementation)
Sort a copy of the array (O(n log n)), then a single linear scan keeps
only the first occurrence of each run of equal values: **O(n log n)**
total. Using a hash set, this is achievable in **expected O(n)** instead.

### (viii) Reversing the array — O(n)
Two-pointer in-place swap from both ends moving toward the centre,
`⌊n/2⌋` swaps total: **Θ(n)** time, **O(1)** extra space.

### (ix) Partition around a random pivot (reversed order) — O(n)
Standard Lomuto-style single-pass partitioning, but the comparison is
flipped: we advance a boundary index `i` for every element `≥ pivot`
(instead of the usual `< pivot`), swapping it into place. One pass over
the array (`for j <- 0 to n-1`) performs at most one swap per iteration:
**Θ(n)** time, **O(1)** extra space (in-place).

## 4. Summary Table

| # | Operation                | Time (this impl.)  | Best possible |
|---|---------------------------|---------------------|----------------|
| i | Maximum                    | O(n)                | O(n) |
| ii| First & second largest     | O(n)                | O(n) |
| iii| Mean                      | O(n)                | O(n) |
| iv| Median                     | O(n log n)          | O(n) (median-of-medians) |
| v | Standard deviation         | O(n)                | O(n) |
| vi| Mode                       | O(n log n)          | O(n) expected (hashing) |
| vii| Remove duplicates         | O(n log n)          | O(n) expected (hashing) |
| viii| Reverse array            | O(n)                | O(n) |
| ix| Partition around pivot      | O(n)                | O(n) |

## 5. Files
- `array_ops.c` — implements and demonstrates all nine sub-problems on a
  single user-supplied array, printing the result and time-complexity
  class of each.
