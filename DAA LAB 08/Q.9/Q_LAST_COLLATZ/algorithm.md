# Q9 — Collatz Conjecture: Computational Experiment

> **Important:** this algorithm is an **experiment**, not a proof. It generates Collatz sequences and measures them. Running it for many starting values can *support* the conjecture but can never *prove* it for all positive integers.

## Algorithm Name
Iterative Collatz trajectory generation and analysis (single start value and interval scan), with overflow-safe arithmetic.

## What is a Collatz sequence?
For a positive integer `n`, define

```
T(n) = n / 2        if n is even
T(n) = 3n + 1       if n is odd
```

The **Collatz sequence (trajectory)** of `n` is `n, T(n), T(T(n)), …`. The **Collatz Conjecture** says that this sequence reaches `1` for **every** positive integer `n`. (After 1 the sequence would go 1 → 4 → 2 → 1 → …, so we stop at 1.) Nobody has proved or disproved this; it is an **open problem**.

## Objective (what the question asks)
Write a modular C program that analyses
* **Part A** — the trajectory of a user-given start value `n ≥ 1`, and
* **Part B** — all start values in a user-given interval `[a, b]`.

## Input
```
n          (start value for Part A, n >= 1)
a b        (interval for Part B, 1 <= a <= b)
```

## Output
* Part A: the full trajectory, number of steps, number of even/odd steps, peak value, stopping time.
* Part B: for the whole interval — how many start values reached 1, the start value with the longest trajectory, the start value with the highest peak, and the average number of steps.

## Quantities that are observed
| Quantity | Meaning |
|---|---|
| **Trajectory (orbit)** | the list `n, T(n), T²(n), …, 1` |
| **Total stopping time (steps)** | number of applications of `T` needed to reach 1 (0 for `n = 1`) |
| **Stopping time** | number of steps until the value first becomes **smaller than `n`** (0 for `n = 1`) |
| **Peak / maximum excursion** | the largest value that appears in the trajectory |
| **Even / odd steps** | how many times `n/2` and `3n+1` were used |

## Termination (when does the loop stop?)
1. **Normal termination:** the current value becomes `1`.
2. **Overflow termination:** the next value `3n+1` would not fit into `unsigned long long` (maximum 18 446 744 073 709 551 615). The program stops and reports `OVERFLOW`; it does **not** print a wrong result.

There is **no step limit**. In theory a loop could run forever if a start value entered a cycle other than 1→4→2→1 (no such cycle is known; see `research.md`). All start values below 2⁷¹ are published to reach 1 (Barina, 2025 — see `references.md`), which covers every value that fits in 64 bits, so for every valid input of this program the loop ends — but that guarantee comes from the *published* computation, not from anything this lab program proves.

## Algorithm Steps
**Function `collatzNext(n, &next)`** — one safe step
1. If `n` is even: `next = n / 2`, return success.
2. If `n` is odd: if `n > (ULLONG_MAX − 1) / 3` then `3n + 1` would overflow → return failure.
3. Otherwise `next = 3n + 1`, return success.

**Function `analyzeStart(n)`** — statistics without storing the trajectory
1. `current = n`, `steps = 0`, `peak = n`, `stoppingTime = 0`.
2. While `current != 1`:
   1. Remember whether `current` is odd; call `collatzNext`. If it fails → mark overflow, stop.
   2. `current = next`; `steps++`; count it as an odd step or an even step.
   3. If `current > peak` → `peak = current`.
   4. If `stoppingTime` has not been set yet and `current < n` → `stoppingTime = steps`.
3. Return the statistics.

**Function `buildTrajectory(n)`** (Part A) — stores the sequence in a **dynamically allocated array** that starts with capacity 64 and **doubles with `realloc`** when full; returns the array and its length. It uses `collatzNext`, so overflow is detected here too.

**Function `partA(n)`** — builds and prints the trajectory, then prints the statistics.

**Function `analyzeInterval(a, b)`** (Part B)
1. For each `m = a … b` call `analyzeStart(m)`.
2. If it overflowed, count it separately and exclude it from the statistics.
3. Otherwise add its steps to a running total, and update "longest trajectory" (most steps) and "highest peak".
4. At the end print: how many reached 1, how many overflowed, the longest, the highest peak, and average steps = total steps / number that reached 1.

**`main`** reads `n`, `a`, `b` with validation and calls `partA` and `analyzeInterval`.

## Example / Dry Run
`n = 6`: 6 → 3 → 10 → 5 → 16 → 8 → 4 → 2 → 1.
Steps = 8 (odd steps: 3→10, 5→16 = 2; even steps = 6), peak = 16, stopping time = 1 (the first value below 6 is 3 after one step).

`n = 27` (see `Outputs/Q_LAST_COLLATZ_output.txt`): 111 steps, peak 9232, stopping time 96.

## Complexity Analysis
Let `S(n)` be the total stopping time of `n`.

| Part | Time | Space |
|---|---|---|
| `collatzNext` | O(1) | O(1) |
| `analyzeStart(n)` | O(S(n)) | O(1) |
| Part A (`buildTrajectory` + printing) | O(S(n)) (array doubling gives amortised O(1) per appended value) | O(S(n)) for the stored trajectory (capacity at most max(64, 2·(S(n)+1))) |
| Part B over `[a, b]` | O( Σ S(m) for m = a…b ) | O(1) |

* **Best case:** `n = 1` → 0 steps, O(1).
* **Worst case:** there is **no known bound** for `S(n)` in terms of `n` that is proved for all `n` — proving `S(n)` finite for every `n` *is* the Collatz Conjecture. So the worst-case running time cannot be stated as a theorem; we can only measure it. Experimentally, over `[1, 10⁶]` the maximum is 524 steps (at n = 837 799) and the average is about 131 steps (see the output file). A heuristic (random-model) argument suggests `S(n)` typically grows like a constant times `ln n` — this is a heuristic, **not** a theorem.
* **Overflow safety:** every `3n+1` is guarded by the check `n ≤ (ULLONG_MAX − 1)/3`.

## What the experiment CAN show
* The exact trajectory, step counts and peak for each tested start value.
* That **no counterexample exists among the tested start values** that fit in the program's range (finite evidence).
* Patterns worth studying (e.g. very large peaks compared with the start value).

## What the experiment CANNOT show
* It cannot prove that **every** positive integer reaches 1 — infinitely many values can never be tested.
* It cannot rule out an extremely large counterexample or a very long cycle beyond the tested range or beyond the `unsigned long long` limit.
* A value flagged `OVERFLOW` is **not** a counterexample — it only means this 64-bit program cannot continue (bigger-integer arithmetic would be needed).
