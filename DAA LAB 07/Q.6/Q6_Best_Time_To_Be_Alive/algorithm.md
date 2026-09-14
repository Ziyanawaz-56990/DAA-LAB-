# Q6 — The Best Time to Be Alive

## Problem
Given a list of prominent scientists, each with a birth year and a death
year (no living scientists included), find the year (or years) in which
the **largest number of these scientists were alive simultaneously**.
Tie-breaking rule: if person A died in the same year person B was born,
A's death is considered to have happened **before** B's birth in that
year (so they do not count as overlapping in that year).

## Key Idea — Sweep Line over Events
This is the classic **maximum overlapping intervals** problem. Each
scientist `(birth, death)` contributes two *events*:
* a **+1 (arrival)** event at year `birth`,
* a **-1 (departure)** event at year `death`.

Sort all events by year. When years tie, the problem's own rule says a
death in year `Y` must be processed **before** a birth in year `Y`
(departures before arrivals on ties), so we sort departure events ahead
of arrival events within the same year.

Sweep through the sorted events left to right, maintaining a running
counter `alive` (add 1 on an arrival, subtract 1 on a departure). The
counter's value right after processing all events of a given year is the
number of scientists alive that year; track the year(s) where this
counter reaches its **maximum**.

## Algorithm
```
BEST-YEAR(scientists[1..n]):                 # each has birth, death
    events <- empty list
    for each scientist (b, d) in scientists:
        events.append( (year = b, type = +1) )
        events.append( (year = d, type = -1) )

    sort events by year ascending;
       break ties with type = -1 (death) before type = +1 (birth)

    alive <- 0
    best  <- 0
    bestYears <- {}
    i <- 0
    while i < events.length:
        y <- events[i].year
        while i < events.length and events[i].year == y:
            alive <- alive + events[i].type
            i <- i + 1
        if alive > best:
            best <- alive
            bestYears <- { y }
        else if alive == best:
            bestYears <- bestYears ∪ { y }

    return best, bestYears
```

## Worked check
If the tie-break rule were reversed (birth before death on a tie), a
person who died the same year another was born would incorrectly be
counted as briefly overlapping; the algorithm above avoids this exactly
as the problem specifies, by ordering deaths (`-1`) before births (`+1`)
whenever years are equal.

## Complexity Analysis
* Building the `2n` events: `O(n)` time, `O(n)` space.
* Sorting the events: `O(n log n)` time.
* Single sweep to find the maximum: `O(n)` time.
* **Total: `O(n log n)` time, `O(n)` space** — this is optimal, since
  any correct algorithm must at least read all `n` input intervals.
