# Q4 — Security Switches

## Problem
`n` switches, numbered `1` (rightmost) to `n` (leftmost), all initially
ON. Rules:
* Switch 1 (rightmost) may be toggled freely.
* Switch `k` (`k > 1`) may be toggled **only if** switch `k-1` (its
  immediate right neighbour) is ON and every switch to the right of
  `k-1` (i.e. switches `1..k-2`) is OFF.
* One switch toggles per move.

Turn all switches OFF in the minimum number of moves.

## Key Idea
This puzzle is the classic **Chinese Rings (Baguenaudier) puzzle** in
disguise. Represent the `n` switches as an `n`-bit mask (bit `i-1` =
state of switch `i`, `1` = ON). From any state there are only a handful
of *legal* moves (toggling switch 1 is always legal; at most one other
switch `k` is legal at a time, the one whose immediate-right neighbour is
ON and everything closer is OFF). Because the state graph has this very
restricted branching, the **shortest path** from "all ON"
(`mask = 2ⁿ-1`) to "all OFF" (`mask = 0`) can be found exactly with a
breadth-first search over the `2ⁿ` reachable states — this is both a
correctness proof and a constructive move-generator.

## Algorithm

### 1. Legal-move check (used by both BFS and the greedy player)
```
LEGAL-MOVE(state, k, n):
    if k == 1: return true
    rightBits <- state & ((1 << (k-2)) - 1)     # switches 1..k-2
    return (bit (k-2) of state is ON) and (rightBits == 0)
```

### 2. Exact solver via BFS (also yields the explicit move sequence)
```
SOLVE-SWITCHES(n):
    start <- (1 << n) - 1            # all ON
    goal  <- 0                        # all OFF
    dist[start] <- 0
    queue <- [start]
    while queue not empty:
        s <- queue.pop_front()
        if s == goal: break
        for k in 1..n:
            if LEGAL-MOVE(s, k, n):
                s2 <- s XOR (1 << (k-1))
                if s2 not visited:
                    dist[s2] <- dist[s] + 1
                    parent[s2] <- (s, k)
                    queue.push_back(s2)
    reconstruct move list by walking parent[] from goal back to start
    return dist[goal], move list
```

### 3. Closed-form move count (fast, for any n)
The recurrence obeyed by `T(n) = dist(goal)` for `n` switches is the
Jacobsthal-style recurrence
```
T(0) = 0,  T(1) = 1,  T(n) = T(n-1) + T(n-2) + 1
```
which solves to:
```
T(n) = (2^(n+1) - 1) / 3      if n is odd
T(n) = (2^(n+1) - 2) / 3      if n is even
```
Both this closed form and the BFS above are implemented in the program,
and their results are cross-checked against each other for small `n`.

### Verified values
| n | 1 | 2 | 3 | 4 | 5 |  6 |  7 |
|---|---|---|---|---|---|----|----|
| T(n) | 1 | 2 | 5 | 10 | 21 | 42 | 85 |

## Complexity Analysis
* **BFS solver** (exact, also gives the move sequence): visits every
  reachable state once and checks `O(n)` candidate moves per state, so
  it is **O(n·2ⁿ) time, O(2ⁿ) space** — used in the program for
  `n ≤ 16` as a correctness proof / demonstration.
* **Closed-form formula**: **O(log n)** time using fast exponentiation
  (or `O(n)` with the simple recurrence), **O(1)** space — used for any
  `n`, since the guaranteed minimum itself grows as `Θ(2ⁿ)` (inherent to
  the problem, not a weakness of the algorithm).
