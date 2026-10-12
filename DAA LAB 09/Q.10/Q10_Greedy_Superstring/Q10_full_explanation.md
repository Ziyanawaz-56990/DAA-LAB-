# Q10 - The Shortest Common Superstring and the Greedy Conjecture: full explanation

## Part A - The problem from scratch
**Superstring.** A string T is a *superstring* of a set S if every string of S appears inside T as a contiguous piece (substring).
Example: S = {abc, bcd, cde}. "abcbcdcde" is a superstring (just glue them), but "abcde" is also one and much shorter.
**SCS (Shortest Common Superstring):** find the shortest superstring. Real uses: assembling a genome from short DNA reads, data compression.

**Overlap.** ov(X, Y) = length of the longest suffix of X that equals a prefix of Y. ov(abc, bcd) = 2 ("bc").
**Merge.** X (+) Y = X followed by Y without the overlapping part: abc (+) bcd = abcd.

**Key simplification.** First delete every string that is a substring of another (it comes free). Then in any superstring the strings appear in a definite left-to-right order, so *a solution is just an ordering (permutation)* t_1, ..., t_n, and its length is
`|t_1| + ... + |t_n| - (ov(t_1,t_2) + ... + ov(t_{n-1},t_n))`.
So SCS = "choose the order that maximises total overlap" = a *Travelling Salesman (path)* problem on the overlap graph. That is why it is NP-hard (NP-hard already for strings of length 3; APX-hard even on a binary alphabet).

## Part B - Algorithms
1. **Exact (small n):** Held-Karp bitmask DP, O(2^n n^2). Implemented in `program.c`.
2. **Greedy:** while more than one string remains, merge the ordered pair with the largest overlap. Very fast (linear time with suffix-tree style data structures) and good in practice (e.g. genome assembly).
   * Tie-breaking: several pairs may share the maximum overlap; the algorithm may pick any. Different picks can give different results, so "the greedy algorithm" is really a family; the approximation ratio is the worst case over all legal choices.
3. **Approximation algorithms (not greedy):** Blum et al. 1994 (ratio 3 via cycle covers), Armen-Stein 2 2/3, Sweedyk 2 1/2, Mucha (Lyndon words) 2 11/23, Englert-Matsakis-Vesely (about 2.475, (37+sqrt 57)/18, a slight improvement on Mucha). They use cycle covers (assignment problem) and are complicated, which is why greedy is so attractive.

## Part C - What the "approximation ratio" means
For an algorithm A: ratio = max over inputs of |A(S)| / OPT(S). A "2-approximation" never returns more than twice the optimum.

**The Greedy Conjecture (Tarhio-Ukkonen 1988; also Storer, Turner, Blum et al.):** greedy is a 2-approximation.
* Easy lower bound: ratio at least 2. Example from the new paper: {ab^n, b^(n+1), b^n c}. Greedy (with a bad tie-break) merges ab^n with b^n c first (overlap n) giving a b^n c, then the leftover b^(n+1) overlaps nothing useful; length about 2n vs optimal about n. 
* Also the classic family c(ab)^n, (ab)^n c, (ba)^n.

## Part D - Timeline of research (what is known as of 6 Oct 2026)
| Year | Result |
|------|--------|
| 1980 | Gallant, Maier, Storer: SCS is NP-hard |
| 1988 | Tarhio-Ukkonen state the greedy (2-approximation) conjecture |
| 1994 | Blum, Jiang, Li, Tromp, Yannakakis: first constant-factor approximation (3); greedy within 4 |
| 1998-2013 | Armen-Stein 2 2/3, Sweedyk 2 1/2, Mucha 2 11/23 |
| 2005 | Kaplan-Shafrir: greedy within 3.5 |
| 2015 | Kulikov et al.: conjecture true when all strings have length 4 |
| 2018 | Cazaux-Rivals: greedy has ratio at most 2 for length-3 strings, new compression-based view |
| 2019 | Golovnev, Kulikov, Logunov, Mihajlin, Nikolaev: Collapsing Superstring Conjecture |
| 2021 | Nikolaev: all tie-breaking instantiations of greedy are equivalent as far as the 2-approximation conjecture goes |
| 2023 | Englert, Matsakis, Vesely: improved approximation guarantees; also a bound of about 3.396 for greedy |
| 2026 | Chukhin, Kulikov, Mihajlin, Smal (arXiv 2608.20018): greedy ratio is at least 2 already for length-6 strings; rho_3 = 9/5; ECCC TR26-157: greedy's guarantee is at most 3 |
| Sep 2026 | **Shibata (arXiv 2609.01365): greedy ratio at least 9/4 - disproves the conjecture (preprint, not yet peer reviewed)** |

## Part E - How Shibata's counterexample works (plain-language)
Fix an even k = 2s >= 10 and a number t of "gadgets". For every gadget i build three periodic strings U_i, V_i, W_i (made from letters x, y, a and a private marker b_i). The input is *all length-k circular substrings* ("rotations that wrap around") of U_i, V_i and W_i - in total t(4s+2) strings, all different, all of length k.
1. **Optimal solution:** think of each string as an edge in a de Bruijn graph (vertices = strings of length k-1). The three cycles of each gadget share vertices, and every vertex has equal in- and out-degree, so there is an **Eulerian circuit** through all edges. Walking it spells a superstring of length (number of strings) + k - 1 = t(4s+2) + k - 1. Since a string of length L contains at most L-k+1 distinct length-k substrings, nothing shorter is possible. So OPT = t(4s+2) + k - 1 exactly.
2. **Greedy's trap:** all overlaps among the original strings are at most k-1, and consecutive rotations overlap by exactly k-1, so greedy is allowed to merge each cyclic chain into one long string (U-hat, V-hat, W-hat) first. Each of these has a border (its first k-1 letters repeat at the end), and the borders are chosen so that afterwards the largest overlap left is only s-3 (merging V-hat with U-hat), then 1, then 0. Each long merged string has wasted a whole period of "wrap-around", so greedy ends with length t(9s+1)+1.
3. **Ratio:** (9s+1)t+1 over (4s+2)t+k-1 tends to (9s+1)/(4s+2) = (9k+2)/(4k+4) as t grows, which is above 2 for k >= 10 (k=10: 2.0909) and tends to 9/4.
Every greedy merge in this run is legal because the paper checks, at each step, that no pair in the current set has a larger overlap (Lemma 3 / Corollary 4: the maximum overlap never increases).

## Part F - We checked it by computer
`verify_shibata_construction.py` rebuilds the instance, computes OPT by an Eulerian circuit (and verifies the string contains every input), then performs the paper's merge order while asserting at **every** merge that the chosen pair has the global maximum overlap. Results (see `../Outputs/Output_Q10_shibata_check.txt`): the greedy lengths equal the paper's formula t(9s+1)+1 for k = 10, 12, 14 and t = 1, 2, 3, 6, 15, 30; for k = 10, t = 30 the ratio is 1.381/669 = **2.0643 > 2**, and it keeps rising toward 2.0909.
This is strong evidence that the *construction* is correct for these sizes. It is not a proof for all k and t and it is not peer review; the general proof is in the paper (Lemmas 1-12, Theorems 10 and 13).

## Part G - Can we "solve" the problem?
Honestly: **no one can give a polynomial-time exact algorithm** unless P = NP, and the exact approximation ratio of greedy is still open (now known to lie between 9/4 and 3). What a student can do:
* Understand and implement the exact DP (n <= ~20) and greedy (done).
* Use the greedy for large inputs, knowing it can be as bad as 9/4 times optimal in the worst case but usually is near optimal on realistic data.
* Explore: the verification script (a computer-checkable counterexample family), different tie-breaking rules, the equal-length case (de Bruijn / Eulerian view gives exact answers when the strings form the "spectrum" of one string).
* Open directions: find the true ratio (between 2.25 and 3); is there a counterexample for shorter strings (length 7, 8)? Chukhin et al. proved only 2 for length 6 and Shibata needs k >= 10; improve the 3 upper bound.

## Part H - Viva-ready summary (30 seconds)
SCS asks for the shortest string containing all given strings; it is NP-hard because it is a TSP on the overlap graph. Greedy merges the max-overlap pair. For 40 years it was conjectured to be within factor 2. A September 2026 preprint by Shibata (arXiv 2609.01365) constructs instances of length-k strings (even k >= 10) where greedy is at least (9k+2)/(4k+4) times optimal, tending to 9/4. The optimum comes from an Eulerian circuit in a de Bruijn graph. The result is a preprint not yet peer reviewed; I reproduced the construction numerically for small parameters.
