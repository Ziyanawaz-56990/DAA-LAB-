# Q3 — Do k numbers in S sum to T? — O(n^(k−1)·log n)

## Problem
Given a set `S` of `n` integers and target `T`, test whether some `k`
of the integers sum to `T`.

## Algorithm — generalised k-SUM

```
kSum(S, n, T):
    sorted = sort(S)                          // O(n log n)
    return solve(sorted, lo=0, k, target=T, depth=0)

solve(a, lo, k, target, depth):
    if k == 2:
        return twoPointer(a, lo, n-1, target)  // O(n), base case

    for i = lo to n-k:
        combo[depth] = a[i]
        if solve(a, i+1, k-1, target - a[i], depth+1):
            return TRUE
    return FALSE

twoPointer(a, lo, hi, target):
    l, r = lo, hi
    while l < r:
        s = a[l] + a[r]
        if s == target: return TRUE (record a[l], a[r])
        elif s < target: l++
        else: r--
    return FALSE
```

## Reasoning
- **Base case (k=2):** on a *sorted* array, two pointers starting at
  opposite ends can find a pair summing to `target` in O(n): if the
  current sum is too small, only moving the left pointer right can
  increase it; if too big, only moving the right pointer left can
  decrease it.
- **Reduction (k>2):** "k numbers summing to T" = "pick one number
  `a[i]`, then recursively find k−1 numbers (searched only to the right
  of `i`, to avoid duplicate/reordered picks) summing to `T − a[i]`."
  This peels the problem down one level at a time until it reaches the
  2-SUM base case.

## Complexity
- Each recursive level above the base case is an O(n) loop.
- There are `k−2` such nested levels wrapping the O(n) two-pointer base.
- Total recursive work: `O(n^(k-1))`.
- Plus the one-time initial sort: `O(n log n)`.
- Combined, as stated in the problem: **O(n^(k−1) · log n)**.

| | |
|---|---|
| Time  | O(n^(k−1) · log n) |
| Space | O(k) recursion depth |

## Worked example
S (sorted) = {-3,-1,2,4,6,7,8,9,11,15}, k=4, T=24

Recursion tries `i=0 (-3)` → needs 3 more summing to 27 → tries
`i=3 (4)` → needs 2 more summing to 23, two-pointer on {6,7,8,9,11,15}
finds `8+15=23`.

Result: -3 + 4 + 8 + 15 = 24 ✔
