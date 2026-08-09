# Q3 — Performance Analysis of Bubble Sort

## Objective

Implement two Bubble Sort variants for randomized sequences and compare their efficiency by measuring the number of element comparisons.

## Two Versions

### Version 1 — Early termination

After each pass, a `swapped` flag records whether any exchange occurred. If a complete pass makes no swaps, the array is already sorted and the algorithm terminates immediately.

### Version 2 — Full passes

The second implementation performs all `n-1` passes regardless of whether the array becomes sorted earlier.

Both versions receive **copies of the same original random array** for each value of `n`. This makes the comparison meaningful because the two algorithms are tested on identical data.

## Why Count Comparisons?

The number of comparisons is a useful machine-independent measure of algorithmic work. It allows the two implementations to be compared without depending on processor speed, compiler settings, or wall-clock timing noise.

## Complexity Analysis

| Version | Best case | Average case | Worst case |
|---|---|---|---|
| Early termination | `Theta(n)` | `Theta(n^2)` | `Theta(n^2)` |
| Full passes | `Theta(n^2)` | `Theta(n^2)` | `Theta(n^2)` |

The early-termination improvement changes the **best case**, not the worst-case asymptotic class.

## Experimental Observation

On randomized data, both algorithms usually perform a large number of passes before the array becomes sorted, so the difference in comparison counts may be smaller than it would be on an already-sorted input. The benefit of the early-exit test becomes especially clear when the data is already ordered or becomes ordered early.

The log-log graph is included to make the approximately quadratic growth easier to inspect across the tested input sizes.

## Learning Outcome

This question demonstrates an important principle of algorithm analysis: a small implementation improvement can significantly improve a **best-case scenario** without changing the overall worst-case complexity. It also shows why controlled experiments require both algorithms to operate on the same input data.

## Files

- `q3_bubble_sort.c` — both Bubble Sort implementations.
- `q3_bubble_sort.csv` — measured comparisons and swaps.
- `plots/` — comparison plots in PNG and SVG formats.
- `sample_output.txt` — recorded output.

## Conclusion

The experiment shows the practical value of an early termination condition while confirming that Bubble Sort remains quadratic in the worst case. The comparison counts connect the theoretical analysis directly with measured algorithmic work.
