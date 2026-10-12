# Q10 - Greedy Superstring (research-oriented)

Full from-scratch explanation, research timeline and a computer check of the new counterexample: see `Q10_full_explanation.md`, `verify_shibata_construction.py`, `research_notes.md`, `references.md`.

> This is an **open / research** question. We do not claim to prove or disprove anything. The program is an *experiment* that builds insight; see `research_notes.md` for the status of the conjecture and honest limitations.

## 1. The problem
Shortest Common Superstring (SCS): given strings s_1..s_n find the shortest string containing all of them as substrings. SCS is NP-hard, so exact algorithms are exponential in general.
**Input representation:** array of C strings (n <= 12, length <= 40 for the demo).

## 2. Preprocessing
Remove duplicates and strings that are substrings of another string (they are covered automatically). After this, in any superstring the strings appear in a definite order, so a solution is an **ordering** of the strings, and the length is `sum(lengths) - sum(overlaps between consecutive strings)`.
overlap(a,b) = length of the longest suffix of a that is a prefix of b.

## 3. Greedy algorithm
```
repeat while more than one string:
    choose (i, j), i != j, with maximum overlap(s_i, s_j)      // ties: smallest i, then smallest j
    replace s_i and s_j by s_i + s_j[overlap:]
return the last string
```
Note: the choice of tie-breaking is part of the algorithm; research literature studies whether all tie-breaking rules behave equally.

## 4. Exact algorithm used for comparison (Held-Karp bitmask DP)
`dp[mask][last]` = maximum total overlap of an ordering of the strings in `mask` ending with `last`. Optimal length = sum(lengths) - max overlap. Time O(2^n * n^2), practical only for small n.

## 5. Dry run (Test 2: cabab, ababc, baba)
Greedy first merges cabab+ababc (overlap 4) -> cababc, then baba (no good overlap) -> cababcbaba, length **10**. Optimal is cabababc, length **8**; ratio 1.25. For k=3 the ratio is 1.4; for the family c(ab)^k, (ab)^k c, (ba)^k it tends to 2 as k grows - the classic example showing greedy is not optimal.

## 6. Complexity
* Overlap of two strings: O(L^2) naive (or O(L) with KMP/failure function). 
* Greedy (naive): O(n) merge rounds x O(n^2) pairs x O(L^2) per overlap. Efficient versions run in O(total length * log) using suffix trees/automata.
* Optimal DP: O(2^n n^2) time, O(2^n n) space.

## 7. What the program shows
1. Greedy vs optimal on hand-made examples (ratio >= 1).
2. A random search of 20000 tiny instances: the worst ratio found stays well below 2 - small cases do **not** reveal the conjecture's failure; the claimed counterexamples are specially constructed (even length k >= 10, large).
3. The values of the claimed bound (9k+2)/(4k+4) for several k, labelled as an unverified claim.

## 8. Possible viva questions
* Is greedy optimal? (No - Test 2 and 3.) Is it always within factor 2? (Conjectured for ~40 years; a Sept 2026 preprint claims a counterexample with ratio >= 9/4 in the limit; not yet independently validated/peer reviewed.)
* What is known? (Greedy is within 3.5 of optimal, proved in 2005, and a 2026 report claims at most 3; lower bound now 9/4 if the Sept 2026 preprint holds.)
* Why remove substrings first? (They never need extra characters.)
