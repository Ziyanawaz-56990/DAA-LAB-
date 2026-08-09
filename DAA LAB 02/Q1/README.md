# Q1 — Dictionary Operations

## Objective
Analyse the asymptotic worst-case running times of `Search`, `Insert`, `Delete`,
`Maximum`, `Minimum`, `Predecessor`, and `Successor` for six representations of
a dictionary `D`: unsorted array, sorted array, singly linked unsorted list,
singly linked sorted list, doubly linked unsorted list, and doubly linked
sorted list — and validate the claims with a C program.

## Assumptions
- **Delete(D, x)** receives a pointer/index to the item already (as stated in
  the problem), not just a key. So Delete never has to *search* for the item.
  What it still may have to do is either (a) close the resulting gap in an
  array by shifting elements, or (b) find the node's predecessor if the
  structure has no back-pointer (singly linked lists).
- Arrays preserve relative order on delete (elements are shifted to close the
  gap) rather than swapping in the last element, matching the same convention
  used for the sorted array (where order must be preserved).
- For **sorted doubly linked lists** we maintain both a `head` and a `tail`
  pointer, so both `Maximum` and `Minimum` are O(1).
- For **singly linked sorted lists** only a `head` pointer is kept (no tail),
  so `Minimum`/`Successor` are O(1) but `Maximum`/`Predecessor` require a
  traversal.

## Complexity Summary

| Representation | Search | Insert | Delete | Maximum | Minimum | Predecessor | Successor |
|---|---|---|---|---|---|---|---|
| Unsorted array | O(n) | O(1) | O(n) | O(n) | O(n) | O(n) | O(n) |
| Sorted array | O(log n) | O(n) | O(n) | O(1) | O(1) | O(1) | O(1) |
| Singly linked, unsorted | O(n) | O(1) | O(n) | O(n) | O(n) | O(n) | O(n) |
| Singly linked, sorted | O(n) | O(n) | O(n) | O(n) | O(1) | O(n) | O(1) |
| Doubly linked, unsorted | O(n) | O(1) | O(1) | O(n) | O(n) | O(n) | O(n) |
| Doubly linked, sorted | O(n) | O(n) | O(1) | O(1) | O(1) | O(1) | O(1) |

**Why each entry holds:**
- *Unsorted array*: append is O(1); everything else needs a scan; delete
  needs a scan-free shift of up to n-1 elements once the index is known.
- *Sorted array*: binary search gives O(log n) search; insert/delete must
  shift up to n elements to keep order; the four extremal/neighbour queries
  are O(1) index arithmetic.
- *Singly linked, unsorted*: push-to-front insert is O(1); delete needs the
  predecessor, found only by walking from `head` — O(n) even with a pointer
  to the target node.
- *Singly linked, sorted*: `Successor` is a single `next` dereference — O(1).
  Everything that needs the predecessor or the tail still requires a walk.
- *Doubly linked, unsorted*: `prev`/`next` let a known node unlink itself in
  O(1); nothing here is sorted, so search/extrema/neighbours still need a
  full scan.
- *Doubly linked, sorted*: `head`/`tail` pointers give O(1) extrema; a known
  node's `prev`/`next` give O(1) neighbours; delete is a O(1) unlink. Only
  `Search`/`Insert` need a scan to locate the right key/position.

## Experimental Design
`q1_dictionary_operations.c` **actually implements all six data structures**
(arrays with real shifting, real singly/doubly linked lists with `malloc`'d
nodes) rather than encoding the formulas directly. For each `n` in
`{10, 20, 50, 100, 200, 500, 1000, 2000, 5000}` it:
1. Builds a fresh structure holding `n` distinct keys.
2. Drives each operation into its worst case (e.g. search for an absent key,
   insert a new maximum into a sorted structure, delete the element whose
   removal costs the most, etc.).
3. Counts every elementary step (key comparison, pointer hop, or array
   shift) with a global counter — never uses a stopwatch, since wall-clock
   time is too noisy for small `n` and depends on the machine.

Output: `q1_operation_growth.csv` (raw counts per n/structure/operation) and
`q1_operation_complexity.csv` (the Big-O class table above, restated as CSV).

## Results
The measured counts match the table exactly, e.g. at `n = 5000`:

| Structure | Search | Insert | Delete | Max | Min | Pred | Succ |
|---|---|---|---|---|---|---|---|
| Unsorted array | 5000 | 1 | 4999 | 4999 | 4999 | 5000 | 5000 |
| Sorted array | 12 (≈log₂5000) | 5001 | 4999 | 1 | 1 | 1 | 1 |
| Singly linked, unsorted | 5000 | 1 | 4999 | 4999 | 4999 | 5000 | 5000 |
| Singly linked, sorted | 5000 | 5000 | 4999 | 4999 | 1 | 4999 | 1 |
| Doubly linked, unsorted | 5000 | 1 | 1 | 4999 | 4999 | 5000 | 5000 |
| Doubly linked, sorted | 5000 | 5001 | 1 | 1 | 1 | 1 | 1 |

Plots in `plots/` (`q1_search_growth`, `q1_insert_growth`, `q1_delete_growth`,
and a combined `q1_all_operations_comparison`) show the O(1)/O(log n)/O(n)
curves visually.

## Observations
- Ordering an array turns search from linear into logarithmic — at the cost
  of linear insert/delete.
- Linked structures give O(1) insertion; a *second* link (doubly linked)
  is what turns delete-by-pointer into O(1) as well.
- Sorted, doubly linked structures are the only representation here where
  every operation except `Search`/`Insert` is O(1).
- No single representation is best for all seven operations — the right
  choice depends on which operations dominate the workload.

## Files
- `q1_dictionary_operations.c` — real implementations + instrumentation.
- `q1_operation_growth.csv`, `q1_operation_complexity.csv` — generated data.
- `plot_q1.py` — regenerates the plots from the CSV.
- `plots/` — PNG + SVG growth charts.
