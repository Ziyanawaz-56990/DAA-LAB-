# DAA LAB-6 — Set 2: Arrays, Matrices, Convolution & Reversal-Sorting

This folder contains complete, compiled-and-verified solutions to the four
problems from the **"Design and Analysis of Algorithm (DAA), Lab-06"**
question sheet PDF (BTech CS-B & CE, 3rd Semester, August 31 2026,
Instructor: Dr. Ajaya Kumar Dash). See `questions.md` / `question_sheet.pdf`
for the original.

## Folder Structure

```
Set2_ArraysMatricesConvolutionSorting/
├── README.md                          <- this file
├── questions.md                       <- transcribed questions
├── question_sheet.pdf                 <- original question sheet (PDF)
├── all_outputs.md                     <- consolidated, real program run transcripts
│
├── Q1_ArrayOperations/
│   ├── array_ops.c                    <- source code
│   └── ALGORITHM.md                   <- per-operation complexity derivation + sample I/O
│
├── Q2_MatrixOperations/
│   ├── matrix_ops.c
│   └── ALGORITHM.md
│
├── Q3_Convolution/
│   ├── convolution.c
│   └── ALGORITHM.md
│
└── Q4_SortingByReversal/
    ├── sorting_by_reversal.c
    └── ALGORITHM.md
```

Each question has its **own folder** with the C source and a detailed
`ALGORITHM.md`. All real program run transcripts are consolidated in
**`all_outputs.md`** at the top level.

## Question Summary

| # | Question | Core Technique | Key Complexity Result |
|---|----------|-----------------|------------------------|
| 1 | 1D array operations (9 sub-parts) | linear scans, sorting, two-pointer, partitioning | max/mean/stddev/reverse/partition = O(n); median/mode/dedup = O(n log n) |
| 2 | 2D square matrix operations (7 sub-parts) | nested loops, cofactor expansion, power iteration | add/zero-check/symmetric-check/transpose = O(n²); multiply = O(n³); determinant = O(n!) (O(n³) via LU); eigenvalue = O(k·n²) |
| 3 | Convolution of two vectors | FFT — Cooley–Tukey divide & conquer | O(n log n), verified against naive O(n·m) |
| 4 | Sorting via `reverse(p,i,j)` | Part A: selection-by-reversal. Part B: merge-sort-by-rotation | Part A: O(n) reversals. Part B: O(n log²n) total cost |

## How to Compile & Run

Each `.c` file is standalone:
```bash
gcc -O2 -Wall -o solution <filename>.c -lm
./solution
```
`-lm` is required for `array_ops.c` (uses `sqrt`), `matrix_ops.c` (uses
`fabs`), and `convolution.c` (uses trig functions for the FFT). All
programs read input interactively from stdin — see `all_outputs.md` for
exact sample inputs and outputs.

## Verification Notes

- **Q3 (Convolution)** is the most notable: the program computes the result
  **both** via the O(n log n) FFT-based divide-and-conquer algorithm *and*
  the naive O(n·m) double loop, then prints the maximum absolute difference
  between them on every run — confirming the fast algorithm is correct
  (difference is floating-point noise only, ~1e-10).
- **Q4 (Sorting by Reversal)** implements *both* required algorithms side
  by side: Part A (`O(n)` reversals, but cost can be `Θ(n²)`) and Part B
  (`O(n log²n)` total cost via a merge-sort structured around array
  rotations, each rotation built from 3 `reverse()` calls). Both are run on
  the same input and cross-verified as producing a fully sorted result; the
  program also prints the reference bound `n·(log₂n)²` next to Part B's
  actual achieved cost so you can see it comfortably fits under the bound.
- **Q2 (Matrix Operations)** was run on both a symmetric matrix (to confirm
  the symmetric-check reports YES and the transpose equals the original)
  and a non-symmetric matrix (to confirm the check reports NO and the
  transpose is visibly different) — both cases match expectations.
- **Q1 (Array Operations)** was run on a dataset with duplicate values and
  again on one with negative numbers, confirming mean/median/std-dev/mode/
  dedup/reverse/partition all behave correctly under both conditions.

## A Note on Q2(v) and Q2(vii) — Determinant and Eigenvalues

These two sub-questions don't have a single "the" algorithm at arbitrary
`n`, so the choices made here are explained explicitly in
`Q2_MatrixOperations/ALGORITHM.md`:
- **Determinant** is implemented via cofactor expansion (`O(n!)`, matches
  the standard 2×2/3×3 formulas taught in class) — the file also notes that
  a production system would use LU/Gaussian elimination for `O(n³)`.
- **Eigenvalue/eigenvector**: a closed-form solution for *all* eigenpairs
  only exists for `n ≤ 4` (via the characteristic polynomial); for general
  `n` there is no closed-form algorithm at all, only iterative numerical
  methods. This program implements **Power Iteration** (`O(k·n²)`), which
  finds the dominant eigenpair — the most commonly taught introductory
  method, with the limitation (only the dominant pair, not the full
  spectrum) stated directly in the code's comments and this file.
