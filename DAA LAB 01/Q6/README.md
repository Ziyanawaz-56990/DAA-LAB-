# Q6 — Element Uniqueness

## Objective

For a sequence of random numbers, determine whether any two elements are equal and analyse the amount of work required by a direct pairwise method.

## Approach

The implementation compares every possible pair of elements:

```text
for each i
    compare a[i] with every a[j], j > i
```

If a matching pair is found, the algorithm can stop immediately and report a duplicate. If every pair is checked without a match, all elements are unique.

The experiment records both the observed comparison count and the theoretical worst-case count.

## Complexity

For `n` elements, the number of possible unordered pairs is:

`n(n-1)/2`

Therefore:

- **Best case:** `O(1)` when a duplicate is found immediately.
- **Worst case:** `Theta(n^2)` comparisons.
- **Auxiliary space for the checking routine:** `O(1)`.

## Experimental Observation

Random data may contain duplicates, allowing the algorithm to terminate before examining every pair. As `n` becomes large, however, the worst-case work grows quadratically. The graph therefore compares the observed work with the theoretical `n(n-1)/2` envelope.

This illustrates why pairwise uniqueness checking becomes unattractive for very large datasets and motivates more efficient approaches such as hashing or sorting when the problem constraints permit them.

## Learning Outcome

The question demonstrates how the same algorithm can have very different actual work depending on where a successful condition is encountered, while its worst-case complexity remains quadratic. It also reinforces the importance of distinguishing best-case, observed, and worst-case behaviour.

## Files

- `q6_element_uniqueness.c` — pairwise duplicate detection.
- `q6_element_uniqueness.csv` — measured comparison counts and theoretical worst-case counts.
- `plots/` — observed-vs-worst-case and log-log growth plots in PNG and SVG formats.
- `sample_output.txt` — recorded output.

## Conclusion

Pairwise comparison is simple and correct, but its quadratic worst-case growth makes it increasingly expensive as the input size grows. The experimental data reflects this increasing cost and supports the theoretical analysis.
