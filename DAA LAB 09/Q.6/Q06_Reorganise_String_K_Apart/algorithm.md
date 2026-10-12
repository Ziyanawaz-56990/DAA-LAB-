# Q6 - Reorganise String with K-Distance Apart

## 1. Understanding the problem
Rearrange the characters of S so that any two equal characters at positions i < j satisfy j - i >= K. Return an empty string if no arrangement exists.
**Input representation:** the string as a char array, a frequency table `cnt[256]` and a table `last[256]` storing the position where each character was last placed.

## 2. Greedy idea
Fill positions 0,1,2,... one by one. At position i, among characters that are **allowed** (last used at least K positions ago) choose the one with the **largest remaining count**. The most frequent character is the hardest to place, so using it whenever it is allowed avoids getting stuck later. If no character is allowed, the answer is impossible.
(The usual textbook version uses a max-heap plus a waiting queue of size K; scanning the fixed 256-character alphabet is equivalent and simpler.)

## 3. Pseudocode
```
if K <= 1: return S
count frequencies; last[c] = -infinity for all c
for i = 0..n-1:
    best = allowed character (cnt>0 and i - last[c] >= K) with max cnt   (ties: smaller char)
    if none: return ""
    out[i] = best; cnt[best]--; last[best] = i
return out
```

## 4. Dry run (S = aabbcc, K = 3)
i=0: all allowed, counts equal -> a. i=1: a blocked -> b. i=2: c. i=3: a is allowed again (3-0=3) -> a, then b, c. Result **abcabc**.
S = aaabc, K = 3: after a,b,c position 3 needs 'a' but 3-0 = 3 OK -> a; position 4 needs 'a' but 4-3 = 1 < 3 -> impossible, return "".

## 5. Complexity
* Scan version: **O(n * sigma)**, sigma = 256 constant, so O(n) time; O(sigma) extra space.
* Heap version: O(n log sigma).

## 6. Validation done by the program
* A checker confirms the output is a permutation of S with all equal characters >= K apart.
* An exhaustive backtracking search decides feasibility exactly; compared on 9 samples + 2000 random cases (fixed seed): feasibility and validity always agree.

## 7. Edge cases
K <= 1 (no restriction), K larger than string length, single character, one character too frequent (impossible).

## 8. Possible viva questions
* Why choose the most frequent allowed character? (Largest remaining demand is the bottleneck.)
* Why is the problem impossible for aaabc with K=3? (Three a's need positions 0,3,6, so n >= 7.)
