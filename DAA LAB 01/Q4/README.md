# Q4 — Towers of Hanoi

## Objective

Implement the recursive Towers of Hanoi algorithm and analyse how the total number of moves changes as the number of discs increases.

## Recursive Structure

For `n` discs, the solution consists of:

1. Moving the top `n-1` discs from the source peg to the auxiliary peg.
2. Moving the largest disc to the destination peg.
3. Moving the `n-1` discs from the auxiliary peg to the destination peg.

This produces the recurrence:

`T(n) = 2T(n-1) + 1`, with `T(0) = 0`.

Solving the recurrence gives:

`T(n) = 2^n - 1`.

## Experimental Analysis

The program records the total number of moves for increasing values of `n` rather than printing every individual move. This makes the growth of the required work visible without filling the output with the complete sequence of moves.

The ordinary plot shows the rapid rise directly, while the logarithmic y-axis plot makes the exponential pattern easier to compare across the entire range.

## Complexity

- **Time:** `Theta(2^n)`
- **Auxiliary recursion space:** `O(n)`
- **Number of moves:** exactly `2^n - 1`

## Learning Outcome

The problem gives a concrete example of how recursion can lead to exponential growth. A seemingly small increase in the number of discs causes the required work to multiply rapidly. It also reinforces the connection between recursive code, recurrence relations, and asymptotic complexity.

## Files

- `q4_hanoi.c` — recursive implementation.
- `q4_hanoi.csv` — number of moves for each tested disc count.
- `plots/` — ordinary and logarithmic growth plots in PNG and SVG formats.
- `sample_output.txt` — recorded output.

## Conclusion

The measured values follow `2^n - 1`, confirming the exponential nature of the recursive solution. Towers of Hanoi is therefore a clear example of an algorithm whose cost becomes impractical very quickly as the input size grows.
