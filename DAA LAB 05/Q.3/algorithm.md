# Program 3 — Quick Sort of N random elements stored in a file

## Problem
Implement Quick Sort of N random elements stored in a file.

## Algorithm

```
ALGORITHM QuickSort(arr, low, high)
    if low < high:
        p = Partition(arr, low, high)
        QuickSort(arr, low, p - 1)
        QuickSort(arr, p + 1, high)

ALGORITHM Partition(arr, low, high)        // Lomuto scheme, random pivot
    pick random index r in [low, high]; swap arr[r], arr[high]
    pivot = arr[high]
    i = low - 1
    for j = low to high-1:
        if arr[j] <= pivot: i++; swap(arr[i], arr[j])
    swap(arr[i+1], arr[high])
    return i + 1
```

### Program flow (file handling)
1. **Generate**: write N random integers to `input.txt`
   (first line = N, followed by N integers).
2. **Read**: load the N integers from `input.txt` into an array.
3. **Sort**: run Quicksort in place on the array.
4. **Write**: save the sorted array to `output.txt`.

## Complexity Analysis

Quicksort's recurrence for a partition split of sizes `k` and `n-1-k`:
```
T(n) = T(k) + T(n-1-k) + O(n)        (the O(n) is the partition step)
```

| Case          | Time complexity | Reasoning |
|---------------|-----------------|-----------|
| Best case     | O(n log n)       | Pivot always splits the array evenly → `T(n) = 2T(n/2) + O(n)` |
| Average case  | O(n log n)       | Expected split is balanced enough on random pivots |
| Worst case    | O(n²)            | Pivot always the smallest/largest element (e.g. naive first-element pivot on sorted data); random pivot selection makes this practically negligible |

**Space complexity:** O(log n) expected recursion stack (in-place sort,
no auxiliary array needed) — better than Merge Sort's O(n) auxiliary space.

**File I/O:** O(n) to generate the numbers, O(n) to read them back,
O(n) to write the sorted result — all linear and dominated by the O(n log n)
sorting cost.
