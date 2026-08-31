# Program 1 — Median of N numbers without sorting

## Problem
Given a list of N numbers, find the median **without sorting the list**,
and analyse the complexity of the algorithm.

## Algorithm — Randomized Quickselect

The median of a list is simply an *order statistic*:
- If N is odd  → the element at (unsorted) rank `N/2` (0-indexed).
- If N is even → the average of the elements at ranks `N/2 - 1` and `N/2`.

Quickselect finds the k-th smallest element using the same partitioning
idea as Quicksort, but — unlike Quicksort — it recurses into **only one
side** of the partition, so the array is never fully sorted.

```
ALGORITHM QuickSelect(arr, low, high, k)
    if low == high: return arr[low]
    pivotIndex = Partition(arr, low, high)      // random pivot
    if k == pivotIndex:  return arr[k]
    else if k < pivotIndex: return QuickSelect(arr, low, pivotIndex-1, k)
    else:                    return QuickSelect(arr, pivotIndex+1, high, k)

ALGORITHM Partition(arr, low, high)              // Lomuto scheme
    pick random index r in [low, high]; swap arr[r], arr[high]
    pivot = arr[high]
    i = low - 1
    for j = low to high-1:
        if arr[j] <= pivot: i++; swap(arr[i], arr[j])
    swap(arr[i+1], arr[high])
    return i + 1

ALGORITHM FindMedian(arr, n)
    if n is odd:  return QuickSelect(arr, 0, n-1, n/2)
    else:         left  = QuickSelect(arr, 0, n-1, n/2 - 1)
                   right = QuickSelect(arr, 0, n-1, n/2)
                   return (left + right) / 2
```

### Why a random pivot?
Choosing the pivot uniformly at random (instead of always the first/last
element) makes worst-case behaviour (already-sorted or adversarial input)
extremely unlikely, giving expected linear time on any input.

## Complexity Analysis

**Partitioning** one sub-array of size m costs `O(m)`.

With a random pivot, on average the pivot splits the array roughly in
half, so the size of the sub-problem we recurse into shrinks
geometrically:

```
T(n) = T(n/2) + O(n)
     = O(n) + O(n/2) + O(n/4) + ...
     = O(n) · (1 + 1/2 + 1/4 + ...)
     = O(n) · 2
     = O(n)
```

| Case          | Time complexity | When it happens |
|---------------|-----------------|------------------|
| Best case     | O(n)             | Pivot always splits array evenly |
| Average case  | O(n)             | Random pivot — expected behaviour |
| Worst case    | O(n²)            | Pivot is always the min/max of the remaining sub-array (astronomically unlikely with a random pivot) |

**Space complexity:** O(1) extra (in-place partitioning); recursion stack
is O(log n) expected, O(n) worst case.

> A deterministic O(n) **worst-case** algorithm exists (Median-of-Medians /
> BFPRT algorithm), but it has larger constant factors and is normally
> only used when a hard worst-case guarantee is required.
