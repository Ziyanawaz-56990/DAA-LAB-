# Q1 — Sort n (number, colour) pairs by colour — O(n)

## Problem
`n` pairs `(number, colour)` are given, already sorted by `number`.
`colour ∈ {RED, BLUE, YELLOW}`. Re-sort by colour (all RED, then BLUE,
then YELLOW) such that numbers stay sorted **within** each colour.

## Why not a normal sort?
A comparison-based sort costs Ω(n log n). But colour only has **3** fixed
values — with a small, fixed alphabet you don't need comparisons at all.
This is a job for **counting sort**, which runs in O(n).

## Algorithm

```
sortByColour(items[1..n]):
    count[RED] = count[BLUE] = count[YELLOW] = 0

    // Pass 1: tally
    for i = 1 to n:
        count[items[i].colour]++

    // Prefix sum -> zone start offsets
    pos[RED]    = 0
    pos[BLUE]   = pos[RED] + count[RED]
    pos[YELLOW] = pos[BLUE] + count[BLUE]

    // Pass 2: stable placement, scanning items in ORIGINAL order
    for i = 1 to n:
        out[pos[items[i].colour]] = items[i]
        pos[items[i].colour]++

    items = out
```

## Why it is stable (numbers stay sorted per colour)
Pass 2 scans the array in its original, number-sorted order, and always
writes each item to the **next free slot** of its colour's zone. Two
items of the same colour are therefore written in the same relative
order they appeared in — nothing ever "jumps ahead" of an earlier item
of its own colour.

## Complexity
| | |
|---|---|
| Time  | O(n) — two linear passes, O(1) work per element |
| Space | O(n) output buffer + O(1) counters (3 colours, doesn't grow with n) |

## Worked trace
Input: `(1,R) (3,Y) (5,B) (7,R) (9,Y) (11,B) (13,R) (15,Y)`

- count = {R:3, B:2, Y:3}
- pos   = {R:0, B:3, Y:5}
- Scanning left to right and placing:
  (1,R)→out[0], (3,Y)→out[5], (5,B)→out[3], (7,R)→out[1],
  (9,Y)→out[6], (11,B)→out[4], (13,R)→out[2], (15,Y)→out[7]

Result: `(1,R) (7,R) (13,R) (5,B) (11,B) (3,Y) (9,Y) (15,Y)` ✔
