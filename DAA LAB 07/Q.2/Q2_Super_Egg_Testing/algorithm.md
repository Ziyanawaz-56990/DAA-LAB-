# Q2 — Super Egg Testing Experiment (Egg Drop Problem)

## Problem
Given `E` identical eggs and an `F`-storey building, find the **minimum
number of droppings that guarantees** determining the highest floor from
which an egg does not break, in the worst case. An egg that breaks cannot
be reused; one that survives can be dropped again.

## Key Idea (Dynamic Programming)
Let `dp[e][f]` = minimum number of trials needed to guarantee finding the
critical floor, given `e` eggs and `f` floors.

Suppose with `e` eggs and `f` floors we first drop an egg from floor `x`
(`1 <= x <= f`). Two outcomes:

* **Egg breaks** — the critical floor is below `x`. We now have `e-1`
  eggs and `x-1` floors left to test: `dp[e-1][x-1]`.
* **Egg survives** — the critical floor is at or above `x`. We now have
  `e` eggs and `f-x` floors left: `dp[e][f-x]`.

Since we must guarantee success, we take the **worst** of the two
outcomes, plus the 1 trial just used, then pick the floor `x` that
**minimises** this worst case:

```
dp[e][f] = 1 + min over x in {1..f} of  max( dp[e-1][x-1], dp[e][f-x] )
```

### Base cases
```
dp[1][f] = f            (one egg -> must test floors one at a time, 1..f)
dp[e][0] = 0             (no floors -> 0 trials needed)
dp[e][1] = 1 for e >= 1  (one floor -> exactly 1 trial)
```

### For the classic instance (E = 2 eggs, F = 100 floors)
`dp[2][100] = 14`, which is the well-known answer to the "two-egg"
puzzle.

## Algorithm (bottom-up DP)
```
EGG-DROP(E, F):
    let dp[0..E][0..F] be a new array
    for e in 1..E: dp[e][0] <- 0
    for f in 1..F: dp[1][f] <- f
    for e in 2..E:
        for f in 1..F:
            dp[e][f] <- INFINITY
            for x in 1..f:
                worst <- max(dp[e-1][x-1], dp[e][f-x])
                dp[e][f] <- min(dp[e][f], 1 + worst)
    return dp[E][F]
```

### Faster O(E·F) formulation (used as an optimisation in the code)
Instead of asking "minimum trials for e eggs, f floors", ask the dual
question: "with `m` trials and `e` eggs, what is the **maximum** number
of floors `f` we can resolve?"

```
f(m, e) = f(m-1, e-1) + f(m-1, e) + 1        (break case + survive case + this trial)
f(0, e) = 0,   f(m, 0) = 0
```

Then the answer to the original problem is the smallest `m` such that
`f(m, E) >= F`. Since `m` only needs to range up to `F` (and in practice
is `O(log F)` for reasonable `E`), this runs in `O(E·F)` time and `O(E)`
space (rolling array), much faster than the `O(E·F²)` triple loop above.

## Complexity Analysis
* Direct DP recurrence: `O(E·F²)` time (outer 2 loops of size `E,F`, inner
  loop of size `F` to choose `x`), `O(E·F)` space.
* Optimised dual-DP: `O(E·F)` time (in the worst case; typically far less
  since `m` is `O(log F)`), `O(E)` space.
* The C program implements **both** and cross-checks that they agree.
