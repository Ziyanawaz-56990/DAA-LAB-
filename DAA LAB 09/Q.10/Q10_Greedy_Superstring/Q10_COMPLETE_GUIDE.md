# Q10 - Complete Guide: The Shortest Common Superstring and the Greedy Superstring Conjecture
(DAA Lab 09, Question 10. Written 6 October 2026. Everything marked "preprint" is not yet peer reviewed.)

---------------------------------------------------------------------
## 0. One-page preview (read this first)
* **The problem.** Given strings s_1..s_n, find the *shortest* string that contains each of them as a substring (a contiguous piece). Called SCS (Shortest Common Superstring).
* **Why it matters.** DNA/genome assembly (reads are the strings, the genome is the superstring), and data compression.
* **Why it is hard.** It is NP-hard (even for strings of length 3, even on a binary alphabet; it is also hard to approximate). It is the Travelling Salesman problem in disguise.
* **The simple algorithm.** GREEDY: repeatedly merge the two strings with the largest overlap. It is fast and works well in practice.
* **The conjecture (1988).** Greedy's answer is never more than 2x the optimum ("2-approximation").
* **What happened.** For ~40 years nobody found a bad example above ratio 2 or a proof. In September 2026, H. Shibata posted arXiv 2609.01365 claiming greedy can be 9/4 = 2.25 times worse than optimal, found with AI help. So the conjecture would be false.
* **Status today.** A preprint, not yet peer reviewed. We rebuilt his instance by computer and the numbers match (see Section 9). The true greedy ratio is now known to lie between 9/4 (claimed) and 3 (claimed upper bound).
* **What the lab expects.** Understand the problem, explore, write an informed view. It is *not* expected that you solve an open problem.

---------------------------------------------------------------------
## 1. The problem from absolute basics
### 1.1 Substring and superstring
A *substring* is a contiguous piece: "bcd" is a substring of "abcde", "ace" is not.
A string T is a *superstring* of a set S if every string in S is a substring of T.

Example: S = {abc, bcd, cde}.
* "abcbcdcde" (just glue) is a superstring, length 9.
* "abcde" is also a superstring, length 5 - and it is the shortest, because it must contain "abc" (3 letters) and then new letters d and e.

### 1.2 Overlap and merge
`ov(X, Y)` = length of the longest *suffix* of X that equals a *prefix* of Y.
* ov(abc, bcd) = 2 (the shared piece is "bc").
* ov(bcd, abc) = 0. (Overlap is directional!)
`X (+) Y` (merge) = X followed by the part of Y not already covered: abc (+) bcd = abcd (length 4 = 3 + 3 - 2).

### 1.3 Remove redundant strings
If a string is a substring of another (or a duplicate), delete it - it is automatically covered. Example: "bc" is useless if "abc" is present. After this clean-up no string contains another.

### 1.4 The key reformulation (everything else follows from this)
In a superstring of a cleaned set, each input string occurs somewhere, and no one occurs inside another, so we can sort them by where they start. So **a solution is an ordering (permutation) t_1, t_2, ..., t_n** of the strings, and the shortest string for that ordering has length
`|t_1| + |t_2| + ... + |t_n|  -  [ov(t_1,t_2) + ov(t_2,t_3) + ... + ov(t_{n-1},t_n)]`.
So minimising length = maximising the total overlap of consecutive strings ("compression").

### 1.5 The overlap graph and the TSP connection
Draw a vertex for every string and a directed edge X -> Y with weight ov(X, Y). A solution is a *path through all vertices* (Hamiltonian path) of maximum total weight. That is exactly the asymmetric Travelling Salesman Problem - the reason SCS is NP-hard and why no fast exact algorithm is expected.

### 1.6 Worked example 1 (greedy is optimal)
S = {abc, bcd, cde}: ov(abc,bcd)=2, ov(bcd,cde)=2, ov(abc,cde)=1. Greedy merges abc+bcd -> abcd, then abcd+cde (overlap 2 = "cd") -> abcde. Length 5 = optimal.

### 1.7 Worked example 2 (greedy loses)
S = {cabab, ababc, baba}. Overlaps: ov(cabab, ababc) = 4 (suffix "abab" = prefix "abab"), ov(cabab, baba) = 3, ov(baba, ababc)=3 ("aba")... The largest is 4, so greedy first builds cababc. Now ov(cababc, baba) = 0 and ov(baba, cababc) = 0, so it appends: cababcbaba, length 10. But "cabababc" (length 8) contains cabab, baba (positions 2-5) and ababc, so OPT <= 8. Ratio >= 10/8 = 1.25. The classic family c(ab)^n, (ab)^n c, (ba)^n pushes the ratio towards 2.

---------------------------------------------------------------------
## 2. Approximation ratio - the vocabulary you need
* **OPT(S)**: length of the shortest superstring.
* An algorithm A is a **rho-approximation** if for every input |A(S)| <= rho * OPT(S).
* A "2-approximation" never exceeds twice the optimum. The **approximation ratio of greedy** is the smallest rho that works, i.e. the worst case over all inputs *and over all legal tie-breaking choices*.
* **Compression** = total length - superstring length = total overlap used. Greedy is known to get at least 1/2 of the optimal compression (Tarhio-Ukkonen / Turner), but a good compression ratio does not automatically give a good length ratio.
* **NP-hard / APX-hard**: no polynomial exact algorithm is expected (unless P=NP); APX-hard means there is a constant c>1 such that even approximating within c is NP-hard.

---------------------------------------------------------------------
## 3. Algorithms for SCS (what exists)
### 3.1 Exact, small n: Held-Karp bitmask DP
`dp[mask][last]` = maximum total overlap of an ordering of the strings in `mask` ending in `last`.
Time O(2^n n^2), space O(2^n n). Works up to n ~ 20. (Implemented in `program.c`.)
Better exact bounds exist for special cases, e.g. length-3 strings in about 3^(n/3) time (Golovnev, Kulikov, Mihajlin, MFCS 2013).

### 3.2 GREEDY
```
S = remove duplicates and strings contained in others
while |S| > 1:
    choose X != Y in S with maximum ov(X,Y)        # ties: ANY choice allowed
    replace X and Y by X (+) Y
return the single remaining string
```
* Running time can be made linear in the total input length (Ukkonen 1990) with suffix-tree-like structures.
* Works well in practice (assemblers use greedy-style ideas).
* **Tie-breaking matters for worst-case examples.** The research question concerns the worst legal run. (Nikolaev 2021 showed that the 2-approximation conjecture for one tie-breaking rule is equivalent to the conjecture for every rule.)

### 3.3 Cycle-cover based approximation algorithms (not greedy)
1. Build the overlap (or "prefix/distance") graph.
2. Find a **minimum-weight cycle cover** - disjoint directed cycles covering all vertices. This is solvable in polynomial time (it is an assignment problem / bipartite matching, Hungarian algorithm).
3. A cycle cover is a *relaxation* of the TSP tour, so its cost is a lower bound on OPT.
4. Break each cycle by deleting its cheapest-overlap edge, build a superstring for each cycle, and concatenate/merge them.
Blum et al. (1994) got ratio 4, then 3. The ratio was pushed down by Armen-Stein (2 2/3), Sweedyk (2 1/2), Mucha 2013 (2 11/23 = 2.478) and Englert-Matsakis-Vesely (about 2.475, i.e. (37+sqrt 57)/18). These are all *non-greedy* and complicated to implement; the best known ratio is therefore still above 2.

### 3.4 Special structure: strings of equal length and the de Bruijn graph
If all strings have length k, make vertices = strings of length k-1 and let each input string be an **edge** from its first k-1 letters to its last k-1 letters (a de Bruijn graph). A superstring that uses every edge once is an **Eulerian path/circuit** and has length |S| + k - 1. If an Eulerian circuit exists, this is optimal (a string of length L has at most L-k+1 different length-k windows). This is the idea behind modern genome assemblers - and it is exactly how Shibata proves OPT for his instance.

---------------------------------------------------------------------
## 4. The Greedy Superstring Conjecture
* **Statement.** GREEDY is a 2-approximation (Tarhio and Ukkonen 1988; also Storer, Turner, Blum et al.).
* **Why 2 is the natural guess.** Easy lower bound: for {a b^n, b^(n+1), b^n c} greedy (with an unlucky tie) merges a b^n with b^n c first (overlap n) to get a b^n c, and then cannot use b^(n+1) at all: length about 2n against optimal a b^(n+1) c of length n+3. So the ratio tends to 2. Everyone believed 2 was also the upper bound.
* **Why it was hard.** Greedy's choices interact globally; an inductive proof needs a good invariant, and for 40 years only partial results existed.

### 4.1 Timeline of results
| Year | Result | Who |
|---|---|---|
| 1980 | SCS is NP-hard | Gallant, Maier, Storer |
| 1988 | Greedy conjecture; greedy compresses at least half | Tarhio, Ukkonen |
| 1990 | Linear-time greedy implementation | Ukkonen |
| 1994 | First constant approximation (3); greedy within 4 | Blum, Jiang, Li, Tromp, Yannakakis |
| 1998-2013 | 2 2/3, 2 1/2, 2 11/23 approximation algorithms | Armen-Stein; Sweedyk; Mucha |
| 2005 | Greedy within 3.5 | Kaplan, Shafrir |
| 2005 | Binary alphabet APX-hardness bounds | Vassilevska |
| 2015 | Conjecture true for length-4 strings | Kulikov, Savinov, Sluzhaev |
| 2018 | Compression vs superstring ratios; greedy at most 2 for length-3 strings | Cazaux, Rivals |
| 2019 | Collapsing Superstring Conjecture | Golovnev, Kulikov, Logunov, Mihajlin, Nikolaev |
| 2021 | All greedy tie-breaking variants equivalent for the conjecture | Nikolaev |
| 2023 | Improved approximation to about 2.475; greedy bound about 3.396 | Englert, Matsakis, Vesely |
| 2024 | Strengthenings of the conjecture | Nikolaev |
| 2026 | Ratio at least 2 already for length-6 strings; exact ratio 9/5 for length 3 | Chukhin, Kulikov, Mihajlin, Smal (arXiv 2608.20018) |
| 2026 | Greedy's guarantee at most 3 (claimed; "tight cycle-cover inequality") | Chukhin et al. (ECCC TR26-157) |
| 1 Sep 2026 | **Greedy ratio at least 9/4: conjecture disproved (preprint)** | Shibata (arXiv 2609.01365) |

---------------------------------------------------------------------
## 5. Shibata's counterexample, step by step
**Parameters.** Even k = 2s >= 10 and t >= 1 "gadgets". Alphabet: x, y, a, and private letters b_1..b_t.
Helper: X_n = (yx)^(n/2) for even n; x(yx)^((n-1)/2) for odd n.
For gadget i:
* U_i = x a X_{s-3} b_i x a X_{s-3} y   (length k)
* V_i = x a X_{s-3} b_i                  (length s)
* W_i = x a X_{s-1} b_i                  (length s+2)

**The input S_{k,t}** = all length-k *circular substrings* ("windows that wrap around") of every U_i, V_i, W_i, i.e. k + s + (s+2) = 4s+2 strings per gadget, all different, all of length k. In total |S| = t(4s+2).

### Concrete tiny case: k = 10, s = 5, t = 1 (22 strings)
U = xayxbxayxy, V = xayxb, W = xayxyxb (b stands for b_1).
Windows from U: xayxbxayxy, ayxbxayxyx, yxbxayxyxa, xbxayxyxay, ... (10 windows).
From V (repeat V forever, take 10 letters): xayxbxayxb, ayxbxayxbx, ... (5 windows).
From W: xayxyxbxay, ayxyxbxayx, ... (7 windows).

### 5.1 The optimum
Let each string be an edge in the de Bruijn graph of order k-1 = 9. The windows of U form one directed cycle, those of V a second cycle, those of W a third; they touch at shared vertices (alpha_i, beta_i, and gamma). Every vertex has in-degree = out-degree and the graph is connected, so an **Eulerian circuit** exists. Spelling it gives a superstring of length |S| + k - 1 = t(4s+2) + k - 1.
For k=10, t=1 the computer found the 31-letter superstring `xayxbxayxbxayxyxbxayxyxayxbxayx` that contains all 22 strings.
Lower bound: a string of length L contains at most L-k+1 distinct length-k substrings, so any superstring has length >= |S| + k - 1. Hence **OPT = t(4s+2) + k - 1**.

### 5.2 The greedy run that goes wrong
1. All overlaps among the original strings are at most k-1, and neighbouring windows of the same cycle overlap by exactly k-1, so greedy may merge each cycle into one long string (called U-hat, V-hat, W-hat). Lemma: merging all windows of a periodic string around once yields a string that has the first k-1 letters again at its end (a "border") - wasted, because the cycle is closed.
2. After that, the strings U-hat_i, V-hat_i, W-hat_i overlap only through their borders. The paper tabulates all these overlaps (Table 1): the best remaining overlap is only s-3 (from V-hat_i to U-hat_i), then overlaps of 1 between different gadgets, then 0.
3. Greedy merges V-hat_i with U-hat_i (overlap s-3), chains the results (overlap 1), and finally appends all W-hat_i with overlap 0.
4. Final length = t(9s+1) + 1.
5. A key lemma shows the maximum overlap never increases during greedy, so each of these merges really is a *legal* greedy choice at the moment it is made.

### 5.3 The ratio
`(t(9s+1)+1) / (t(4s+2)+k-1)  ->  (9s+1)/(4s+2) = (9k+2)/(4k+4)` as t -> infinity. For k = 10 this is 92/44 = 2.0909; as k grows it tends to 9/4 = 2.25.

---------------------------------------------------------------------
## 6. What the numbers look like (our computer check)
Script: `verify_shibata_construction.py` (Python). It rebuilds the instance, finds OPT with an Eulerian circuit, and replays the paper's merges while asserting that each chosen pair has the **maximum** overlap in the current set.

| k | t | strings | OPT | greedy | ratio |
|---|---|---|---|---|---|
| 10 | 1 | 22 | 31 | 47 | 1.516 |
| 10 | 3 | 66 | 75 | 139 | 1.853 |
| 10 | 6 | 132 | 141 | 277 | 1.965 |
| 10 | 15 | 330 | 339 | 691 | 2.038 |
| 10 | 30 | 660 | 669 | 1381 | **2.064** |
| 14 | 6 | 180 | 193 | 385 | 1.995 |

Why the ratio starts low: the "+k-1" and "+1" constants matter until t is large (that is the limit in the formula).
What this proves: the construction behaves as the paper claims for these parameters. What it does not prove: the claim for every k and t (the paper's lemmas do that), nor that the paper is free of errors outside these cases - that is what peer review is for.

---------------------------------------------------------------------
## 7. Where the open questions are now
* True ratio of greedy: somewhere in [9/4, 3] (claims; 3 is not yet widely checked).
* For strings of each fixed length k, what is rho_k? Known: rho_3 = 9/5; rho_k >= 2 for k >= 6; rho_k >= (9k+2)/(4k+4) for even k >= 10 (e.g. k=10: 2.0909). Gap for k = 5, 7, 8, 9.
* Is there a simple non-greedy algorithm that beats 2.475? The best known is still around 2.475.
* Can a cleverly modified greedy (e.g. cycle-cover aware) recover a 2-approximation?
* Hardness side: best known inapproximability constants are small (close to 1), so the "true" difficulty of SCS is unknown.

---------------------------------------------------------------------
## 8. How to present YOUR view (a template you can adapt honestly)
Your teacher says "explore and develop an insight". A good answer separates **facts, evidence and opinion**:

**Facts (cite them):** the problem definition; NP-hardness; greedy's description; the 1988 conjecture; the known bounds; Shibata's claim.
**Evidence you produced yourself:** your C program (exact vs greedy on small inputs), the Python reproduction of the construction (ratio 2.064 at k=10, t=30).
**Your view (state it with its limits). Example paragraph:**
> "My view is that the 2026 preprint is very likely correct, because (i) its optimum comes from a clean Eulerian-circuit argument that I can check, (ii) its greedy run follows legal steps - I verified by program that every merge has maximum overlap, and (iii) my numerical reproduction matches its formula exactly for k = 10, 12, 14 and several t. However it is a single-author preprint, AI-assisted in discovery, and not yet peer reviewed, so I treat it as strong but unconfirmed. Even if it stands, it only shows greedy's ratio is above 2; the exact value (between 9/4 and 3) remains open, and greedy remains an excellent practical heuristic - its worst-case instances are very artificial (long periodic strings with many carefully placed borders)."

Other opinions you could argue (pick one and defend it with evidence):
1. *Practical*: worst-case ratio 2.25 hardly matters for real DNA reads, which are not adversarial.
2. *Theoretical*: the interesting fact is that a 40-year-old conjecture fell to a computer-assisted construction - which suggests re-examining other "obviously true" conjectures with AI search.
3. *Methodological*: the importance of tie-breaking and of Eulerian/de Bruijn structure.
Avoid claiming things you did not check (e.g. "the paper is definitely right", or "I proved it").

---------------------------------------------------------------------
## 9. Resources (what to read, in order)
### Easiest first
1. **Lecture notes - "The Shortest Superstring Problem" (HKUST COMP572)**: overlap, prefix graph, cycle cover, 4-approximation. https://cse.hkust.edu.hk/mjg_lib/Classes/COMP572_Fall07/Notes/Shortest_Superstring.pdf
2. **Slides - A. Kulikov, "Shortest common superstring: overview" (CPM 2015)**: overlap graph, greedy and its analysis. https://cpm.cs.helsinki.fi/cpm15/26_Kulikov.pdf
3. **Frankfurt lecture notes (in German)** on SCS: greedy 4-approximation and cycle covers. https://ae.informatik.uni-frankfurt.de/teaching/18ws/+apa/Sequenzierung.pdf
4. **Scribe notes, Computational Biology (Virginia Tech)**: GREEDY, MGREEDY, TGREEDY and the DNA assembly context. https://courses.cs.vt.edu/~cs6104/compbio2000/scribenotes/CLASS20.html
5. **Books**: V. Vazirani, *Approximation Algorithms* (the chapter on the Shortest Superstring Problem); D. Gusfield, *Algorithms on Strings, Trees and Sequences* (suffix trees, overlap computation, assembly).
### The research papers
6. Shibata 2026, *Disproving the Greedy Superstring Conjecture*, arXiv:2609.01365 - https://arxiv.org/pdf/2609.01365 (preprint; read Sections 1-3 first, skip proofs on first read).
7. Chukhin, Kulikov, Mihajlin, Smal 2026, *The greedy superstring algorithm achieves ratio 2 for strings of length 6 already*, arXiv:2608.20018 - https://arxiv.org/pdf/2608.20018
8. Chukhin et al. 2026, *A tight cycle-cover inequality for shortest common superstring*, ECCC TR26-157 - https://eccc.weizmann.ac.il/report/2026/157/download
9. Nikolaev 2024, *Greedy Conjecture for the SCS Problem and its Strengthenings*, arXiv:2407.20422 - https://arxiv.org/abs/2407.20422
10. Nikolaev 2021, *All instantiations of the greedy algorithm for the shortest superstring problem are equivalent*, arXiv:2102.05579 - https://arxiv.org/abs/2102.05579
11. Golovnev, Kulikov, Logunov, Mihajlin, Nikolaev 2019, *Collapsing Superstring Conjecture* (APPROX/RANDOM 2019) - https://drops.dagstuhl.de/entities/document/10.4230/LIPIcs.APPROX-RANDOM.2019.26
12. Englert, Matsakis, Vesely, *Improved approximation guarantees for shortest superstrings using cycle classification by overlap to length ratios* (arXiv:2111.03968) / ISAAC 2023 - https://ar5iv.arxiv.org/html/2111.03968
13. Cazaux, Rivals 2018, *Relationship between superstring and compression measures: new insights on the greedy conjecture*, Discrete Applied Math - https://hal-lirmm.ccsd.cnrs.fr/lirmm-01800340
14. Kulikov, Savinov, Sluzhaev 2015, *Greedy conjecture for strings of length 4*, CPM 2015.
15. Kaplan, Shafrir 2005, *The greedy algorithm for shortest superstrings*, Inf. Process. Lett.
16. Blum, Jiang, Li, Tromp, Yannakakis 1994, *Linear approximation of shortest superstrings*, J. ACM 41(4) - the foundational approximation paper.
17. Tarhio, Ukkonen 1988, *A greedy approximation algorithm for constructing shortest common superstrings*, Theor. Comput. Sci. 57 - where the conjecture is stated.
18. Gallant, Maier, Storer 1980, *On finding minimal length superstrings*, JCSS 20 - NP-hardness.
19. Mucha 2013 (*Lyndon words and short superstrings*, SODA); Sweedyk 1999; Armen-Stein 1998 - the non-greedy approximation algorithms.
20. Golovnev, Kulikov, Mihajlin 2013, *Solving 3-superstring in 3^(n/3) time*, MFCS 2013 - exact algorithm for length-3 strings.
### Code
21. A. Kulikov's experiments repository: https://github.com/alexanderskulikov/greedy-superstring-conjecture (as cited in the Collapsing Superstring paper).
22. This project: `program.c` (exact DP + greedy) and `verify_shibata_construction.py`.
### Context on the news
23. Breakthrough Tracker entry (provisional status record): https://kingy.ai/mathematics-science-breakthrough-tracker/records/cs-greedy-superstring-conjecture-counterexample-2026/ - useful for the status, not a primary source.
### Where to look for updates
arXiv listing cs.DS (search "superstring"), ECCC reports, and the Google Scholar page of Kulikov, Mihajlin, Nikolaev; check whether Shibata's paper gets a v2, a journal venue, or an independent verification.

---------------------------------------------------------------------
## 10. Practice questions / viva
1. Define overlap and merge. Compute ov(aabaab, abaabb).
2. Why can we delete strings that are substrings of others?
3. Why is SCS equivalent to a TSP on the overlap graph?
4. Run greedy on {ate, half, lethal, alpha, alfalfa} by hand. Is the result optimal? (Our program: both are 17.)
5. What is an approximation ratio? What does "2-approximation" mean?
6. State the greedy conjecture. What is its easy lower bound example?
7. Why do tie-breaking rules matter?
8. How does an Eulerian circuit give an optimal superstring in the equal-length case?
9. What is the claim of the Sept 2026 preprint and why do we say "claimed"?
10. What did you verify yourself, and what did you not verify?
11. Why is the cycle-cover relaxation a lower bound for OPT?
12. Which of "ratio >= 9/4" and "ratio <= 3" has stronger independent support? (Be honest: both are recent and unrefereed; the 9/4 construction is explicit and checkable, which we did for small cases.)

## 11. Glossary
* **Border of T**: a string that is both a prefix and a suffix of T (e.g. "ab" in "abcab").
* **Circular substring**: a window of the infinite repetition T T T ... (wraps around).
* **de Bruijn graph (order m)**: vertices = strings of length m; an input string of length m+1 is an edge from its prefix to its suffix.
* **Eulerian circuit**: a closed walk using every edge exactly once; exists iff the graph is connected and in-degree = out-degree at every vertex.
* **Cycle cover**: vertex-disjoint cycles covering all vertices.
* **NP-hard / APX-hard**: see Section 2.
* **Preprint**: a paper posted publicly (e.g. arXiv) before peer review.
