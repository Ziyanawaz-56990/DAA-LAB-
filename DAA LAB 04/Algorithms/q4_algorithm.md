# Q4 — Time of Maximum Simultaneous Presence — O(n·log n)

## Problem
`n` persons enter at `a_i` and leave at `b_i` (all `2n` times distinct).
Find the time at which the most people are present at once.

## Algorithm — event sweep line

```
maxPresenceTime(arrive[1..n], leave[1..n]):
    events = []
    for i = 1 to n:
        events.add( (arrive[i], +1) )
        events.add( (leave[i],  -1) )

    sort events by time                    // O(n log n)

    present = 0, best = 0, bestTime = -infinity
    for each event (t, delta) in sorted events:
        present += delta
        if present > best:
            best = present
            bestTime = t

    return bestTime, best
```

## Reasoning
Rather than checking every possible instant (there are infinitely
many), only check the moments where the headcount actually *changes* —
an arrival (+1) or a departure (−1). Between two consecutive events the
crowd size is constant, so the true maximum can only occur starting
exactly at some event's time. Sweeping through the sorted events with a
running counter finds it in one linear pass after the sort.

## Complexity
| | |
|---|---|
| Time  | O(n log n) — dominated by sorting 2n events |
| Space | O(n) — event array |

## Worked example
Persons (entry, exit): (1,8) (3,10) (5,22) (20,40) (15,30) (25,33)

Sweeping: at t=5, persons 1, 2, and 3 have all arrived and none have
left yet → present = 3, which is the maximum reached anywhere in the
timeline.

Result: max presence = 3, first reached at t = 5 ✔
