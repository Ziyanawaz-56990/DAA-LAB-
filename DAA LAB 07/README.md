# Design and Analysis of Algorithm (DAA) — Lab 07

BTech (CS-B and CE), 3rd Semester — September 8, 2026
Instructor: Dr. Ajaya Kumar Dash

## Contents

```
DAA_Lab07/
├── README.md                              <- this file
├── DAA_lab_Lab07_Questions.pdf            <- original lab question sheet
├── Q1_Invert_Coin_Triangle/
│   ├── algorithm.md                       <- algorithm design + complexity
│   └── solution.c                         <- C implementation
├── Q2_Super_Egg_Testing/
│   ├── algorithm.md
│   └── solution.c
├── Q3_Reves_Puzzle/
│   ├── algorithm.md
│   └── solution.c
├── Q4_Security_Switches/
│   ├── algorithm.md
│   └── solution.c
├── Q5_Hitting_Moving_Target/
│   ├── algorithm.md
│   └── solution.c
├── Q6_Best_Time_To_Be_Alive/
│   ├── algorithm.md
│   └── solution.c
└── Q7_Matrix_Chain_Multiplication/
    ├── algorithm.md
    └── solution.c
```

Each question folder is self-contained: `algorithm.md` gives the problem
restatement, the algorithm design (with pseudocode), a worked example /
verification, and a full time & space complexity analysis; `solution.c`
is a compileable, runnable, and tested C program implementing it.

## Question index

| # | Title | Technique | Complexity |
|---|-------|-----------|------------|
| Q1 | Invert the Coin-Triangle | Overlay / maximum-overlap search on a triangular lattice | O(n⁴) exact search, O(1) closed form for n ≡ 0 (mod 4) |
| Q2 | Super Egg Testing Experiment | Dynamic programming (generalized egg drop) | O(E·F²) direct DP; O(E·F) optimised dual DP |
| Q3 | Reve's Puzzle | Frame–Stewart algorithm (4-peg Tower of Hanoi) | O(n²) to compute move count, O(FS(n)) to generate moves |
| Q4 | Security Switches | BFS on switch-state graph + closed-form (Jacobsthal-style) recurrence | O(n·2ⁿ) exact BFS; O(log n) formula |
| Q5 | Hitting a Moving Target | BFS on "belief-set" states (pursuit game) | O(n·2ⁿ) exact BFS; O(n) to generate the known sweep pattern |
| Q6 | The Best Time to Be Alive | Sweep line over birth/death events | O(n log n) |
| Q7 | Matrix Chain Multiplication | Dynamic programming over sub-chain length | O(n³) time, O(n²) space |

## How to build and run each solution

Every `solution.c` is a standalone C99 program with no external
dependencies. From inside any question folder:

```bash
gcc -O2 -Wall -o solution solution.c
./solution
```

The program will then prompt for whatever input that question needs
(e.g. `n` for Q1/Q3/Q4/Q5, `E` and `F` for Q2, the birth/death years for
Q6, the matrix dimensions for Q7).

## Verification notes

Every solution was compiled and run against a known/classic instance of
its puzzle before being included here, to make sure the algorithm is
actually correct and not just "plausible":

* **Q1**: `n = 4` (the famous 10-coin triangle) gives **3** moves —
  matches the well-known answer. The general algorithm is an exact
  O(n⁴) search; a closed form `M(n) = 5n²/32 + 3n/8 − 1` is proven and
  verified for `n ≡ 0 (mod 4)`.
* **Q2**: The classic "2 eggs, 100 floors" instance gives **14** —
  matches the standard textbook answer. Two independent DP formulations
  (O(E·F²) and O(E·F)) are cross-checked against each other.
* **Q3**: `n = 8` disks gives exactly **33** moves — matches the puzzle
  statement itself ("solves the puzzle in 33 moves").
* **Q4**: The exact BFS solver and the closed-form formula agree for
  every `n` tested (1 through 10); e.g. `n = 3` needs **5** moves.
* **Q5**: The BFS-computed optimal shot count always equals the known
  `2n − 4` sweep-pattern length, for every `n` tested (2 through 10).
* **Q6**: Verified by hand against a 3-scientist example that
  deliberately tests the "death before birth on a tie" rule, and the
  program reproduces the hand-worked answer exactly.
* **Q7**: The classic CLRS textbook instance
  `p = {30, 35, 15, 5, 10, 20, 25}` gives **15125** scalar
  multiplications with parenthesisation `((A1(A2A3))((A4A5)A6))` —
  matches the standard textbook result.

## Common structure used across the algorithms

Several of these puzzles reduce to the same two core algorithmic ideas
taught in DAA:

* **Dynamic programming over subproblems** (Q2 egg drop, Q3 disk-count
  recurrence, Q7 matrix chain) — build the answer for a larger instance
  from optimal answers to smaller instances.
* **State-space / graph search** (Q1 overlap search, Q4 switch states,
  Q5 belief-set states) — model every reachable configuration as a node
  in a graph and use BFS to find the shortest sequence of moves that
  reaches a goal configuration.
* **Greedy sweep over sorted events** (Q6) — a single linear pass over
  pre-sorted events is enough once the right event representation
  (+1/−1) is chosen.

