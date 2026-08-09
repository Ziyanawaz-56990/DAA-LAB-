# Q5 — Find the Partition Point

## Objective

Given an array consisting of a sequence of `0`s followed by a sequence of `1`s, find the exact transition point between the two values.

## Example

```text
0 0 0 0 0 1 1 1 1
          ^
       transition
```

## Approach

The array is monotonic, so a linear scan is unnecessary. Binary search is used to locate the **first occurrence of `1`**.

At each step:

- If the middle element is `1`, the first `1` may be at the middle or somewhere to its left, so the search continues in the left half.
- If the middle element is `0`, the transition must lie to the right.

The returned index therefore identifies the boundary between the zero block and the one block. If the returned index equals `n`, the array contains only zeros.

## Complexity

- **Time:** `O(log n)`
- **Extra space:** `O(1)`

## Learning Outcome

This question demonstrates how knowledge of input structure can reduce the amount of work required. Because the array is ordered, binary search eliminates half of the remaining candidates after every comparison.

## Files

- `q5_partition_point.c` — binary-search implementation.
- `sample_output.txt` — recorded examples and results.

## Conclusion

The transition point can be found efficiently without scanning the complete array. The experiment reinforces the idea that sorted or monotonic data can be exploited to obtain logarithmic-time solutions.
