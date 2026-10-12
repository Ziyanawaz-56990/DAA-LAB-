# Q2 - Huffman Coding with Canonical Codebook

## 1. Understanding the problem
Given n symbols with frequencies f_i, assign each a binary codeword so that no codeword is a prefix of another (prefix-free) and sum(f_i * length_i) is as small as possible.
**Input representation:** two parallel arrays `sym[]` (characters) and `freq[]` (positive integers); frequencies can be counted from a text first.

## 2. Greedy idea
Repeatedly take the **two least frequent** nodes and merge them under a new parent whose frequency is their sum. Frequent symbols end near the root (short codes), rare ones deep (long codes).
*Greedy-choice property:* the two rarest symbols can always be siblings at the deepest level of some optimal tree (exchange argument), so merging them first loses nothing.

## 3. Canonical codebook
Different Huffman trees can give different codes with the same lengths. Canonical Huffman keeps **only the lengths** and assigns codes in a fixed way:
1. Sort symbols by (length, symbol) - "ordered lexicographically by length".
2. First symbol gets code 0 (all zeros). 
3. Each next code = (previous code + 1) shifted left by (length difference).

## 4. Pseudocode
```
min-heap H of leaves keyed by (freq, index)
while |H| > 1:
    a = extract-min(H); b = extract-min(H)
    create node z with freq = a.freq + b.freq, children a, b
    insert z into H
lengths = depth of each leaf in the final tree (single symbol: length 1)
sort symbols by (length, symbol)
code = 0
for i = 1..n-1:  code = (code + 1) << (len[i] - len[i-1]);  assign
```

## 5. Dry run (CLRS example, program Test 1)
a:45 b:13 c:12 d:16 e:9 f:5
merge 5+9=14; merge 12+13=25; merge 14+16=30; merge 25+30=55; merge 45+55=100.
Lengths: a=1, b=c=d=3, e=f=4. Canonical codes: a=0, b=100, c=101, d=110, e=1110, f=1111. Total bits = 224 (2.24 bits/symbol).

## 6. Complexity
* n-1 merges, each uses 2 extract-min + 1 insert on a heap of size <= n: **O(n log n)**.
* Canonical assignment: sort O(n log n), assign O(n). 
* Overall **O(n log n) time, O(n) space.**

## 7. Validation done by the program
* Kraft sum equals 1 (the code tree is full), pairwise prefix-free check.
* Total cost compared with an independent O(n^2) "merge two smallest by scanning" computation.
* 500 random cases (fixed seed) all pass.

## 8. Edge cases
Single symbol (code length forced to 1), equal frequencies (balanced tree), two symbols.

## 9. Possible viva questions
* Why is Huffman optimal? (Greedy-choice property + optimal substructure.)
* Why canonical? (Only the lengths need to be stored; decoder rebuilds codes identically.)
* Is the Huffman tree unique? (No, ties can be broken differently, but the optimal cost is the same.)
