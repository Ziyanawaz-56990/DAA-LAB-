# Q3 — Reve's Puzzle (Tower of Hanoi with 4 Pegs)

## Problem
`n` disks of different sizes, `4` pegs. All disks start on peg 1
(largest at bottom). Move all disks to another peg, one disk at a time,
never placing a larger disk on a smaller one. Show the puzzle is solved
in **33 moves for n = 8**, and generalise to `n` disks.

## Key Idea — Frame–Stewart Algorithm
With only 3 pegs, the classic Tower of Hanoi needs `2^n - 1` moves. With
a 4th (spare) peg available, we can be smarter: split the `n` disks into
two groups.

1. Move the top `k` disks (the smaller ones) from the source peg to a
   spare peg, **using all 4 pegs** (recursively) — `FS(k)` moves.
2. Move the remaining `n-k` largest disks from source to destination
   using only **3 pegs** (the 4th peg is occupied by the `k` disks we
   just parked, so it is unusable as an intermediate) — this costs the
   classic `2^(n-k) - 1` moves.
3. Move the `k` disks from the spare peg to the destination, again using
   all 4 pegs — `FS(k)` moves.

Total: `2*FS(k) + 2^(n-k) - 1`. We try every split point `k` (`1<=k<n`)
and keep the minimum:

```
FS(n) = min over k in {1..n-1} of  [ 2*FS(k) + 2^(n-k) - 1 ]
FS(0) = 0,  FS(1) = 1
```

For `n = 8`, the optimal split is `k = 2` (park the top 2 disks first),
giving `FS(8) = 33` — exactly matching the puzzle's known answer.

## Algorithm
```
FRAME-STEWART(n):
    if n == 0: return 0
    if n == 1: return 1
    best <- INFINITY
    bestK <- -1
    for k in 1..n-1:
        moves <- 2*FRAME-STEWART(k) + (2^(n-k) - 1)
        if moves < best:
            best <- moves
            bestK <- k
    memoise FRAME-STEWART(n) = best, splitK(n) = bestK
    return best

SOLVE(n, src, dst, spare1, spare2):
    if n == 0: return
    k <- splitK(n)
    SOLVE(k, src, spare1, dst, spare2)      # park top k disks on spare1
    SOLVE_3PEG(n-k, src, dst, spare2)       # classic 3-peg Hanoi (spare1 is "blocked")
    SOLVE(k, spare1, dst, src, spare2)      # bring the k disks onto dst
```

`SOLVE_3PEG` is ordinary recursive 3-peg Tower of Hanoi:
```
SOLVE_3PEG(n, src, dst, spare):
    if n == 0: return
    SOLVE_3PEG(n-1, src, spare, dst)
    move disk n: src -> dst
    SOLVE_3PEG(n-1, spare, dst, src)
```

## Complexity Analysis
* Computing `FS(n)` with memoisation: each `FS(n)` tries `O(n)` split
  points, so overall **O(n²) time, O(n) space** for the move-count table.
* Generating the actual move sequence: the number of moves is `FS(n)`
  itself (exponential-ish but far below `2^n - 1`; e.g. `FS(8)=33` versus
  `2^8-1=255` for 3 pegs), so printing every move costs **O(FS(n))**
  time.
