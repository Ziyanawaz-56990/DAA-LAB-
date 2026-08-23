# Q5 — Merge Overlapping Intervals — O(n·log n)

## Problem
Given a list `I` of `n` intervals `(x_i, y_i)`, merge all overlapping
intervals into a minimal disjoint set, in worst-case O(n log n).

## Algorithm

```
mergeIntervals(I[1..n]):
    sort I by left endpoint x            // O(n log n)

    result = []
    cur = I[1]
    for i = 2 to n:
        if I[i].x <= cur.y:              // overlap / touch
            cur.y = max(cur.y, I[i].y)
        else:                             // gap
            result.add(cur)
            cur = I[i]
    result.add(cur)
    return result
```

## Reasoning
Once intervals are sorted by left endpoint, any interval that can
possibly overlap the interval currently being built can only appear
immediately next in the sequence — you'll never need to "look back."
So a single left-to-right pass, extending a running merged interval
`cur` whenever the next interval starts within (or touching) it, and
closing it off when a gap appears, is enough.

## Complexity
| | |
|---|---|
| Time  | O(n log n) — dominated by the sort |
| Space | O(n) — output list |

## Worked example (matches the assignment's own test case)
I = {(1,3), (2,6), (8,10), (7,18)}

Sorted by left endpoint: (1,3) (2,6) (7,18) (8,10)
- cur=(1,3); next (2,6): 2≤3 → overlap, extend → cur=(1,6)
- next (7,18): 7≤6? No → gap → flush (1,6), cur=(7,18)
- next (8,10): 8≤18 → overlap, but 10 > 18? No → cur stays (7,18)
- flush (7,18)

Result: {(1,6), (7,18)} ✔ — exactly the expected output.
