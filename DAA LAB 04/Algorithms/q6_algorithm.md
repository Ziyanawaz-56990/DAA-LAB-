# Q6 — Point Covered by the Most Intervals — O(n·log n)

## Problem
Given `n` intervals `(l_i, r_i)` on a line (endpoints inclusive), find
a point `p` contained in the largest number of intervals.

## Algorithm — sweep line with tie-break

```
maxOverlapPoint(S[1..n]):
    events = []
    for i = 1 to n:
        events.add( (l_i, START) )
        events.add( (r_i, END) )

    sort events by coordinate;
        on a tie, START before END              // <-- key subtlety

    coverage = 0, best = 0, bestPoint = -infinity
    for each event (x, type) in sorted events:
        if type == START:
            coverage++
            if coverage > best:
                best = coverage
                bestPoint = x
        else:
            coverage--

    return bestPoint, best
```

## Reasoning
Same sweep-line family as Q4, but with **inclusive endpoints**, which
introduces a subtlety: if interval A ends at `x` and interval B starts
at `x`, point `x` belongs to *both*. If END events were processed
before START events at a shared coordinate, the sweep would drop
coverage before raising it, and we'd miss the true overlap at that
instant. Processing **all START events before END events** on a tie
fixes this — coverage correctly reflects both intervals being "open"
at that shared point.

We only check for a new maximum right after a START event, since
coverage only *increases* there — checking at an END event would be
pointless (coverage is decreasing).

## Complexity
| | |
|---|---|
| Time  | O(n log n) — dominated by the sort |
| Space | O(n) — event array |

## Worked example (the assignment's own test case)
S = {(10,40), (20,60), (50,90), (15,70)}

Events sorted: 10(S) 15(S) 20(S) 40(E) 60(E) 70(E) 50(S) 90(E)
*(50(S) actually sorts before 60(E)/70(E) by coordinate — full order:
10S,15S,20S,40E,50S,60E,70E,90E)*

Sweeping: coverage hits 3 right after the START event at x=20
(intervals (10,40), (20,60), (15,70) all open) — that's the peak.

Result: p = 20, covered by 3 intervals ✔ (the assignment's own example
answer, p=50, also achieves 3 — either is valid, since the true maximum
holds across the whole range [20,40]).
