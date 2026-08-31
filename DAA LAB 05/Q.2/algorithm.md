# Program 2 — K'th smallest element without sorting

## Problem
Given a list of N numbers, find the K'th smallest element **without
sorting the list**, and analyse the complexity of the algorithm.

## Algorithm — Randomized Quickselect

Same core idea as Program 1, generalised to an arbitrary rank `k`
(1-indexed in the user interface, converted to 0-indexed internally).

```
ALGORITHM QuickSelect(arr, low, high, k)      // k is 0-indexed rank
    if low == high: return arr[low]
    pivotIndex = Partition(arr, low, high)
    if k == pivotIndex:      return arr[k]
    else if k < pivotIndex:  return QuickSelect(arr, low, pivotIndex-1, k)
    else:                     return QuickSelect(arr, pivotIndex+1, high, k)

ALGORITHM Partition(arr, low, high)           // Lomuto scheme, random pivot
    pick random index r in [low, high]; swap arr[r], arr[high]
    pivot = arr[high]
    i = low - 1
    for j = low to high-1:
        if arr[j] <= pivot: i++; swap(arr[i], arr[j])
    swap(arr[i+1], arr[high])
    return i + 1
```

### Walkthrough
1. Pick a random pivot and partition the array around it (exactly like one
   pass of Quicksort). After partitioning, the pivot sits at its final
   sorted position `p`.
2. If `p` equals the target rank `k`, we found the answer immediately.
3. Otherwise, the k-th smallest element must lie entirely on one side of
   `p` — recurse into only that side. The other side is discarded and
   never touched again, so the array is never fully sorted.

## Complexity Analysis

| Case          | Time complexity | Reasoning |
|---------------|-----------------|-----------|
| Best case     | O(n)             | Lucky pivots shrink the search range fast |
| Average case  | O(n)             | `T(n) = T(n/2) + O(n) → O(n)` (random pivot, expected) |
| Worst case    | O(n²)            | Pivot is always the smallest/largest remaining element every time (essentially impossible with random pivoting; would require an adversary who can see the random choices) |

**Space complexity:** O(1) extra (in-place partitioning);
recursion stack O(log n) expected, O(n) worst case.

### Comparison with the "sort first" approach
Sorting the whole array first and then picking `arr[k-1]` costs
`O(n log n)`. Quickselect answers the same question in expected `O(n)` —
asymptotically faster because it never needs the *entire* array ordered,
only the one element requested.
