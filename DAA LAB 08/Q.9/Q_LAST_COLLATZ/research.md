# Collatz Conjecture — Research Investigation

> **Honesty statement.** The Collatz Conjecture is an **open (unsolved) problem**. This document and the C program in this folder do **not** solve it and do **not** prove it. They are a learning investigation: we study the problem, read what mathematicians have established, run a finite computer experiment, and clearly separate *experiment* from *proof*.
> All literature facts below come from the sources listed in `references.md`, which were checked against the publishers' / authors' / repositories' pages.

---

## 1. Introduction
Take any positive integer. If it is even, halve it. If it is odd, triple it and add 1. Repeat. Does the sequence **always** end up at 1? This question — the **Collatz Conjecture**, also called the **3n+1 problem**, the Syracuse problem, Ulam's problem or Kakutani's problem — is traditionally credited to Lothar Collatz (1937) and has been open for decades.

It is interesting because (a) a school child can understand and test it, (b) it has resisted all attempts at a full solution, (c) it connects number theory, dynamical systems and the theory of computation, and (d) it is a perfect playground for C programming: loops, functions, dynamic memory and integer overflow. Our lab question says exactly this: *the simplicity of this problem is deceptive.*

## 2. Mathematical Definition
Let ℕ⁺ = {1, 2, 3, …}. The Collatz map T : ℕ⁺ → ℕ⁺ is

```
T(n) = n/2        if n ≡ 0 (mod 2)
T(n) = 3n + 1     if n ≡ 1 (mod 2)
```

Write Tᵏ for k-fold iteration (T⁰(n) = n). For a start value n:

* the **trajectory** (or **orbit**) of n is the sequence n, T(n), T²(n), …;
* the **total stopping time** σ_total(n) is the smallest k with Tᵏ(n) = 1 (if it exists);
* the **stopping time** σ(n) is the smallest k ≥ 1 with Tᵏ(n) < n (for n ≥ 2);
* the **maximum excursion** (peak) is max{ Tᵏ(n) : k ≥ 0 };
* a **cycle** is a trajectory with Tᵏ(m) = m for some k ≥ 1. The map has the cycle 1 → 4 → 2 → 1, called the **trivial cycle**.

(Many papers use the equivalent "shortcut" map n ↦ (3n+1)/2 for odd n, which combines the odd step with the forced even step that follows. Our lab uses the form given in the question paper.)

## 3. Simple Examples
| Start | Trajectory | Steps |
|---|---|---|
| 1 | 1 | 0 |
| 2 | 2 → 1 | 1 |
| 3 | 3 → 10 → 5 → 16 → 8 → 4 → 2 → 1 | 7 |
| 6 | 6 → 3 → 10 → 5 → 16 → 8 → 4 → 2 → 1 | 8 |
| 7 | 7 → 22 → 11 → 34 → 17 → 52 → 26 → 13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1 | 16 |
| 27 | needs 111 steps and reaches 9232 on the way (see our output file) | 111 |

These step counts for 1–10 and 27 were also produced by our C program (`Outputs/Q_LAST_COLLATZ_output.txt`). Notice that the sequence can first **rise a lot** (27 climbs to 9232, about 342 times its start) and then fall — the reason it is sometimes called a *hailstone* sequence.

## 4. The Collatz Conjecture
> **Conjecture.** For every positive integer n there is a k ≥ 0 with Tᵏ(n) = 1.

Equivalent: every trajectory enters the trivial cycle 1 → 4 → 2 → 1. If the conjecture is **false**, then there is at least one start value whose trajectory either **grows without bound** (divergent trajectory) or **enters a different cycle**.

## 5. Why Is It Difficult?
* **Two incompatible operations.** Division by 2 is a "2-adic" operation, multiplication by 3 is a "3-adic" operation. They do not interact in any simple way, so the sequence behaves like a pseudo-random walk, even though it is completely deterministic.
* **Heuristic says "should converge", but heuristics are not proofs.** A simple probabilistic argument: an odd step multiplies by about 3 and is always followed by at least one halving; if we treat the parity as a fair coin, the *average* change of ln n per "odd step + halving(s)" is negative, so trajectories tend to drift downwards. This is only a **heuristic model** — real trajectories are not random, and one exceptional trajectory would break the conjecture.
* **No monotone quantity is known.** Unlike a typical termination proof, nobody has found a "measure" that always decreases along the sequence.
* **No local structure to induct on.** Knowing that the numbers below n converge does not directly tell us about n, because the trajectory of n may climb far above n first.
* **Connections to computability.** Conway (1972) showed that *generalised* Collatz-type problems can be formally undecidable, and Kurtz & Simon (2007) proved that a natural generalisation is undecidable. These results do **not** show that the original 3n+1 problem is undecidable; they only show that the family of problems it belongs to can be extremely hard, which is one reason the question paper mentions "computability".
* Famous remarks about its difficulty: Paul Erdős is quoted as saying mathematics may not be ready for such problems, and Jeffrey Lagarias has described it as extraordinarily difficult (both quoted in the Wikipedia article cited in `references.md`; Erdős's remark is also on the Chamberland page cited there). These are opinions of experts, not theorems.

## 6. Computational Investigation
A computer can test the conjecture for a *given* start value n: iterate T until the value is 1. If the value reaches 1, that start value is **verified**. Because the iteration is simple, we can test enormous ranges, using tricks such as lookup tables, sieves (skipping start values that are guaranteed to drop below a smaller, already-verified number), and GPUs/supercomputers (Barina 2021, 2025).

Programming issues:
* **Integer overflow.** Values can become far bigger than the start value. A signed 32-bit `int` (maximum 2 147 483 647) already fails for small starts: for example the trajectory of n = 113 383 reaches 2 482 111 348 (we checked this with a separate script); a 64-bit `unsigned long long` still overflows for large starts. Serious verification projects use 128-bit integers.
* **Termination.** The loop would run forever on a counterexample that falls into a new cycle (no such start value is known).

## 7. Important Quantities
| Quantity | Meaning | Where our program reports it |
|---|---|---|
| Trajectory / orbit | the full sequence | Part A |
| Total stopping time | steps to reach 1 | "steps" in Parts A and B |
| Stopping time | steps until the value first drops below the start | Parts A and B |
| Maximum excursion | highest value reached | "peak" |
| Cycle | a trajectory that returns to its starting value | not searched — only the trivial cycle is known |

The stopping time is the central quantity of the early theory: if every n ≥ 2 had a finite stopping time, then (by induction) the Collatz Conjecture would follow. That is why Terras's and Tao's theorems are about stopping behaviour.

## 8. Known Computational Verification
* David Barina (2021) described a new algorithm for convergence verification and a GPU/CPU implementation (*Journal of Supercomputing*, 2021).
* Barina (2025, *Journal of Supercomputing* 81, article 810) reports verification of the conjecture for **all start values up to 2⁷¹** (about 2.4 × 10²¹) and found four new "path records". The project's web page (see `references.md`) lists a slightly higher limit, 2075 × 2⁶⁰ ≈ 2^71.02, as of its page date (5 August 2026); the figure may have moved since.
* **What this establishes:** no counterexample exists below that limit, i.e. every integer below it has a trajectory that reaches 1.
* **What it does not establish:** anything about integers above the limit. History shows that other famous conjectures (for example the Pólya and Mertens conjectures, as the Wikipedia article reminds readers) failed only at extremely large numbers, so "no counterexample so far" is **not a proof**.
* Our own program checks only a tiny part of this (up to 10⁶ in the recorded run), and it is limited by 64-bit arithmetic.

## 9. Known Mathematical Results
(Each is described carefully; none solves the conjecture.)
1. **Almost all numbers have finite stopping time.** Terras (1976), and independently Everett (1977), proved that for "almost all" n the trajectory eventually goes below n (the exceptional set has density zero).
2. **Korec's exponent.** For any θ > log 3 / log 4 ≈ 0.7924, almost all n have a trajectory that goes below n^θ (as quoted in the abstract of Tao's paper).
3. **Many numbers do converge.** Krasikov & Lagarias (2003) showed that at least x^0.84 of the integers up to x have 1 in their orbit (for large x). That is a lower bound far below "all", and far below "almost all" in the density sense.
4. **Tao (2019/2022).** For logarithmic-density "almost all" n, the trajectory comes below f(n) for *any* function f that tends to infinity, however slowly (an example is log log log log n). This is considered the strongest result of this "almost all" type. It does **not** cover every n.
5. **Cycles.** The only known cycle on positive integers is 1 → 4 → 2 → 1. Hercher (2023) proved that a non-trivial cycle cannot have 91 or fewer "local minima" (his parameter *m*), building on earlier work that gave smaller bounds; this uses the computational verification limits above.
6. **Undecidability of generalisations.** Conway (1972) and Kurtz & Simon (2007), as discussed in Section 5. Not applicable to the original problem.

## 10. Research Papers (what each actually contributes)
Detailed notes are in `research_notes.md`; in one line each:
* **Lagarias (1985)** — classic survey of the problem's history, results and generalisations.
* **Terras (1976)** — introduces the stopping-time viewpoint and proves "almost all" numbers have finite stopping time.
* **Krasikov & Lagarias (2003)** — lower bound x^0.84 for how many integers ≤ x reach 1.
* **Tao (2022)** — almost all orbits attain almost bounded values (logarithmic density).
* **Hercher (2023)** — no non-trivial cycles with m ≤ 91 local minima.
* **Barina (2021, 2025)** — algorithms and records for computational verification up to 2⁷¹.
* **Kurtz & Simon (2007) / Conway (1972)** — undecidability of generalised Collatz problems.

## 11. Our C Experiment
`program.c` implements what the question asks:
* **Part A** — for a user-given start `n ≥ 1` it prints the trajectory (stored in a dynamically allocated, growing array), the number of steps, the even/odd step counts, the peak and the stopping time.
* **Part B** — for a user-given interval `[a, b]` it analyses every start value and reports how many reached 1, the longest trajectory, the highest peak and the average number of steps.
* **Overflow handling** — each `3n+1` is guarded; when it would overflow, the run is stopped and reported as `OVERFLOW` (not as a result).
* **Modular design** — separate functions for one step, statistics, trajectory building, printing and interval analysis.

## 12. Experimental Observations
These are observations of **our runs** (see `Outputs/Q_LAST_COLLATZ_output.txt`). They are *not* theorems.

| # | Observation (from the recorded output) |
|---|---|
| 1 | n = 27 needs 111 steps (70 halvings, 41 triplings), peaks at 9232 and has stopping time 96, although 27 is small. |
| 2 | In [1, 20] every start value reached 1; the longest trajectories (20 steps) belong to 18 and 19, the highest peak (160) belongs to 15; the average is 9.8 steps. |
| 3 | In [1, 1 000 000] **all 1 000 000** start values reached 1 (no overflow occurred). The longest trajectory is n = 837 799 with 524 steps; the highest peak is 56 991 483 520, reached from n = 704 511; the average is about 131.4 steps. |
| 4 | n = 837 799 climbs to 2 974 984 576 on its way down — the peak is thousands of times larger than the start. |
| 5 | The overflow test with start values near 2⁶⁴ − 1 produced `OVERFLOW` for all 10 values; the program reported this instead of printing a wrong answer. |
| 6 | The observed average of ≈131 steps for [1, 10⁶] is the same order as the "typical ≈ constant × ln n" growth predicted by heuristic models (about 10·ln(5×10⁵) ≈ 130–140); this agreement is only a heuristic sanity check. |

Our program's results for 1 … 10⁶ agree with the verification published in the literature (all those numbers are covered by the 2⁷¹ limit); our program adds nothing new to the research.

## 13. Limitations
* **Finite computation.** We tested at most 10⁶ start values in the recorded run; there are infinitely many integers.
* **Computational resources / time.** Part B scans every value one by one with no caching, so very large intervals are slow (the program refuses intervals above 10⁸ values).
* **Integer limits and overflow.** `unsigned long long` (typically 64 bits) holds values up to 18 446 744 073 709 551 615. Larger values need multi-precision or 128-bit arithmetic as used in published verification projects. `OVERFLOW` is not a counterexample.
* **No new mathematics.** A counterexample, if one exists, would most likely be astronomically large; no ordinary lab program will find it.
* **Heuristics.** Statements such as "trajectories drift downwards on average" are plausibility arguments, not proofs.

## 14. What Remains Unsolved?
**The Collatz Conjecture is still an open problem.** As far as the sources in `references.md` indicate, nobody has proved that every positive integer reaches 1, and nobody has found a counterexample. In particular it is open whether (a) some trajectory diverges to infinity and (b) a non-trivial cycle exists (only cycles with a huge number of local minima remain possible).

## 15. Possible Future Research
(Directions that are legitimate for a student to read about or explore computationally; none is claimed to be a path to a proof.)
* Study the distribution of total stopping times and peaks for large random samples; compare with probabilistic models.
* Implement 128-bit or big-integer versions and speed-ups (sieving, lookup tables, multithreading) to see how verification projects work.
* Study the "shortcut" (Syracuse) map and the inverse Collatz tree (which numbers lead to a given number).
* Study generalisations (e.g. 5n+1) where divergent or other cyclic behaviour appears, to see why the 3n+1 case is special.
* Read about the connection to undecidability (Conway; Kurtz & Simon) and to dynamical systems.

## 16. Conclusion
We learned that the Collatz rule is trivially easy to implement, yet its long-term behaviour is unproven. Our C program lets us observe stopping times, peaks and overflow issues for ranges of numbers, and our results are consistent with the conjecture — but **consistency is not proof**. Mathematicians have proved strong partial results (Terras, Everett, Krasikov–Lagarias, Tao, Hercher) and verified the conjecture up to about 2⁷¹, yet the statement for **all** positive integers remains unresolved.

---

### Clear separation of the five ideas
| Concept | In this assignment |
|---|---|
| **The Problem** | Does every positive integer reach 1 under T? (open) |
| **The Algorithm** | Repeatedly apply T and record steps, peak, stopping time (`algorithm.md`) |
| **The Experiment** | Our C program on a finite range, e.g. 1…10⁶ (`program.c`, output file) |
| **Existing Research** | Terras, Everett, Krasikov–Lagarias, Tao, Hercher, Barina, … (`research_notes.md`) |
| **Mathematical Proof** | A rigorous argument for **all** positive integers — **does not exist yet** |

> **Testing a very large finite number of starting values does not prove that the Collatz Conjecture is true for every positive integer.**

### Figure 1 (from the question paper) — `collatz_figure.png`
The figure, taken from the PDF and captioned there as *Collatz conjecture paths for 5000 random starting points below 10⁶ (Ref: wiki)*, draws the Collatz paths of 5000 random start values below one million as one merged picture. Reading its own legend: the paths are drawn starting from the "tree root" 1; the drawing turns left by 8.65° for an even node and right by 16° for an odd node, edge length shrinks with the logarithm of the distance from the root, and colour/thickness show how often an edge was used by the sampled paths. The marked numbers (16, 40, 22, 130, 94, …) are values on the common path near the root; 2¹⁹ = 524 288 and 837 799 ("the longest path below 1 million") are annotated.
**Why it matters:** the paths from many different start values **merge** into a small number of shared "trunks" near 1 — a visual way to see that trajectories funnel into the same final descent. The picture shows only 5000 sampled start values; like our program it is **evidence**, not a proof. Our recorded run found the same value 837 799 as the longest trajectory below 10⁶ (524 steps), consistent with the annotation in the figure.
