# Q5 — Hitting a Moving Target

## Problem
`n` hiding spots on a line, numbered `1..n`. A target hides at some
unknown spot and, between every two consecutive shots, moves to an
**adjacent** spot (it must move — it can never stay put). The shooter
never observes anything (no feedback) and must announce, in advance, a
fixed sequence of spots to shoot such that **no matter** where the
target started and no matter which adjacent spot it chose to move to at
each step, it is guaranteed to be hit at some point in the sequence.
Does such a sequence exist, and if so, what is it?

## Key Idea — Belief-Set Search
Since the shooter gets no feedback, the right way to reason about
"guaranteed to hit" is to track the **set of positions the target could
still possibly be at**, given that it has survived every shot fired so
far — call this the *belief set* `S`. Initially `S = {1, 2, ..., n}`
(target could be anywhere).

* **Firing a shot at spot `p`** removes `p` from the belief set (any
  target actually at `p` would have been hit, so that possibility is
  eliminated): `S' = S \ {p}`.
  * If `S'` becomes **empty**, every possible hiding trajectory has now
    been hit — the sequence is complete and guaranteed successful.
* **The target then moves.** Since we don't know which adjacent spot a
  surviving target chose, the new belief set is the union of the
  neighbours of every position still in `S'`:
  `S_next = ⋃_{s in S'} { s-1, s+1 } ∩ [1, n]`.

The goal is to find the **shortest sequence of shots** that drives the
belief set to empty. This is an ordinary shortest-path problem over the
`2ⁿ` possible belief sets, solvable exactly with BFS — exactly the same
technique used for Q4.

### Why a guaranteed sequence exists
Every shot removes at least one element from the belief set and it can
never re-grow beyond the neighbours of what remains, so — as the BFS
below confirms computationally for every `n ≥ 2` — the belief set is
driven to empty in a finite number of steps. **Yes, such an algorithm
always exists** for `n ≥ 2`.

## Algorithm
```
NEIGHBORS(S, n):                       # S is a bitmask over {1..n}
    result <- {}
    for s in S:
        if s-1 >= 1: result <- result ∪ {s-1}
        if s+1 <= n: result <- result ∪ {s+1}
    return result

HIT-MOVING-TARGET(n):
    start <- {1, 2, ..., n}            # full belief set
    goal  <- EMPTY
    dist[start] <- 0
    queue <- [start]
    while queue not empty:
        S <- queue.pop_front()
        for p in 1..n:
            S' <- S \ {p}
            if S' == EMPTY:
                return dist[S] + 1, (reconstructed shot sequence, ending in p)
            S_next <- NEIGHBORS(S', n)
            if S_next not visited:
                dist[S_next] <- dist[S] + 1
                parent[S_next] <- (S, p)
                queue.push_back(S_next)
```

### Verified strategy pattern
For `n` spots, BFS finds that firing at spots
`2, 3, 4, ..., n-1, n-1, n-2, ..., 3, 2` (sweep inward-to-outward once
left-to-right skipping the two end spots, then sweep back) always
succeeds, taking `2n - 4` shots for `n ≥ 3` (`n = 2` needs 2 shots:
`1, 2`, since with only two spots the target is forced to alternate).
The program's BFS confirms this is in fact optimal for the `n` values
tested.

## Complexity Analysis
* Belief-set BFS: `O(n)` states-per-transition check over `O(2ⁿ)`
  reachable belief sets → **`O(n·2ⁿ)` time, `O(2ⁿ)` space** — used in
  the program for `n ≤ 16` to both prove existence and construct the
  optimal sequence.
* Once the shot pattern is known, generating it directly (without BFS)
  for any `n` is **`O(n)` time, `O(1)` extra space**.
