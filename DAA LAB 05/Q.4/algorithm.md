# Program 4 — Heap Sort of N randomly generated elements stored in a file

## Problem
Implement Heap Sort to sort N randomly generated elements stored in a
file. Do the complexity analysis of your algorithm.

## Algorithm

Heap Sort has two phases: **build a max-heap**, then **repeatedly extract
the maximum**.

```
ALGORITHM HeapSort(arr, n)
    // Phase 1: build max-heap  (bottom-up)
    for i = n/2 - 1 down to 0:
        Heapify(arr, n, i)

    // Phase 2: extract elements one by one
    for i = n-1 down to 1:
        swap(arr[0], arr[i])       // move current max to the end
        Heapify(arr, i, 0)         // restore heap property on the rest

ALGORITHM Heapify(arr, heapSize, i)
    largest = i
    left  = 2*i + 1
    right = 2*i + 2
    if left  < heapSize and arr[left]  > arr[largest]: largest = left
    if right < heapSize and arr[right] > arr[largest]: largest = right
    if largest != i:
        swap(arr[i], arr[largest])
        Heapify(arr, heapSize, largest)     // sift down further
```

### Program flow (file handling)
1. **Generate**: write N random integers to `input.txt`.
2. **Read**: load the N integers from `input.txt` into an array.
3. **Sort**: build a max-heap from the array, then repeatedly swap the
   root (maximum) with the last unsorted element and re-heapify.
4. **Write**: save the sorted array to `output.txt`.

## Complexity Analysis

**Build-heap phase:** Although each `Heapify` call costs `O(log n)`, most
nodes are near the bottom of the tree and need very little sifting.
Summing the work over all nodes gives a tight bound of `O(n)`, not the
loose `O(n log n)` one gets by simply multiplying `n` nodes × `O(log n)`.

**Extraction phase:** `n - 1` extractions, each followed by one
`Heapify` call that costs `O(log n)` → `O(n log n)` total.

**Overall:** `O(n) + O(n log n) = O(n log n)`.

| Case          | Time complexity | Reasoning |
|---------------|-----------------|-----------|
| Best case     | O(n log n)       | Heap operations always take O(log n) regardless of data arrangement |
| Average case  | O(n log n)       | Same reasoning — no dependence on input order |
| Worst case    | O(n log n)       | **Unlike Quicksort, Heap Sort has no pathological worst case** |

**Space complexity:** O(1) extra — the heap is built directly inside the
input array (fully in-place), and extraction is iterative.

**File I/O:** O(n) to generate, O(n) to read, O(n) to write — all linear,
dominated by the O(n log n) sort.

### Heap Sort vs Quick Sort
Heap Sort guarantees `O(n log n)` in every case (no random pivot needed),
but Quicksort is usually faster in practice due to better cache locality
and smaller constant factors, despite its theoretical `O(n²)` worst case.
