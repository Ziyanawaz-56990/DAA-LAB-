# Q9 - Hu-Tucker Algorithm (optimal alphabetic tree)

## 1. Understanding the problem
Unlike Huffman, the leaves must stay in the **given left-to-right order** (like keys in a search tree), so we cannot freely merge the two smallest weights - they may not be neighbours. Minimise sum(w_i * depth_i).
**Input representation:** array `w[0..n-1]` in fixed order; an array of nodes with fields *weight*, *isSquare* (original leaf?) and *tree-id*.

## 2. Hu-Tucker idea
Leaves are **squares**; merged nodes are **circles**. Two nodes are **compatible** if *no square lies strictly between them* (circles are "transparent"). 
**Phase 1 (combination):** repeatedly merge the compatible pair with the smallest weight sum (ties: leftmost). The new circle node takes the position of the left node.
**Phase 2 (levels):** the depth of each leaf in this (possibly non-alphabetic) merge tree is its level.
**Phase 3 (recombination):** rebuild an order-preserving tree with exactly those levels (stack: push levels left to right, whenever the top two are equal merge them into one of level-1). The theorem of Hu and Tucker says the resulting alphabetic tree is optimal.
Alphabetic codes follow from the levels: code_1 = 0...0; code_{i+1} = (code_i + 1) shifted to the next length.

## 3. Pseudocode
```
nodes = leaves (all squares)
while more than one node:
    (i,j) = compatible pair with minimum w_i + w_j     // scan j to the right of i, stop after the first square
    new circle node at position i, weight w_i + w_j, children (node i, node j)
    delete node j
level[leaf] = depth of leaf in this tree
verify with stack and build codes
```

## 4. Dry run (weights 3 1 2 5 4)
1. Compatible pairs at the start are neighbours only. Smallest sum: 1+2 = 3 -> merge, circle [3] takes the place of leaf 2: sequence 3, [3], 5, 4.
2. Now 3 and [3] are compatible (sum 6); 3 and 5 are also compatible (only a circle between, sum 8). Smallest is 6 -> merge: [6], 5, 4.
3. Pairs: ([6],5)=11, (5,4)=9; ([6],4) is not compatible because square 5 lies between. Merge 5+4 = 9: [6], [9].
4. Merge the last two (15) -> root.
Levels: leaf1 = 2, leaf2 = 3, leaf3 = 3, leaf4 = 2, leaf5 = 2. Cost = 3*2 + 1*3 + 2*3 + 5*2 + 4*2 = **33**, equal to the DP optimum. Codes: 00, 010, 011, 10, 11.

## 5. Complexity
* As implemented: n-1 merges, each scanning O(n^2) pairs worst case (the square-blocking rule makes it much less in practice), so **O(n^3) worst case**, O(n) space. 
* The famous bound is **O(n log n)** using a priority queue of locally minimal compatible pairs (Hu-Tucker 1971; Garsia-Wachs gives a simpler O(n log n) route) - not required for this lab.
* The reference DP (for validation) is O(n^3).

## 6. Validation done by the program
Compare the Hu-Tucker cost with the classic optimal-alphabetic-tree dynamic programme `dp[i][j] = W(i..j) + min_k dp[i][k] + dp[k+1][j]`. 5 tests + 3000 random cases (fixed seed, n <= 10): all match, and every output forms a valid ordered tree.

## 7. Edge cases
n = 1 (depth 0), equal weights (balanced tree), very light leaves between heavy ones.

## 8. Possible viva questions
* Why can't plain Huffman be used? (It reorders leaves.)
* What does "compatible" mean and why does a square block it? (A square leaf between two nodes would have to cross the merge, breaking the order.)
* What is the optimality guarantee? (Hu-Tucker theorem; we also verified numerically against DP.)
