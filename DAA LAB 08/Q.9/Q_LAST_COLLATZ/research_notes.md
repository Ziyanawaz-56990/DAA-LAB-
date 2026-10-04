# Research Notes — What We Learned from Each Source

These notes are written in beginner-friendly language. Each summary is based on the abstract / catalogue page of the source (see `references.md`), not on a full line-by-line reading of every proof. Where something is only a summary, it is labelled as such. None of these sources proves the Collatz Conjecture.

---

## The 3x + 1 Problem and Its Generalizations

### Title
The 3x + 1 Problem and Its Generalizations

### Author(s)
Jeffrey C. Lagarias

### Year
1985

### Source / Publication
The American Mathematical Monthly 92(1), pp. 3–23

### Link
https://maa.org/node/113776

### Research Question
What is known about the 3x+1 (Collatz) problem and its generalisations, and where did the problem come from?

### Main Idea
A guided tour: the problem, its many names (Collatz, Syracuse, Kakutani, Hasse, Ulam), the history, early computer evidence and partial results, and generalisations.

### Main Result
This is a survey: it collects and organises results known in 1985 rather than proving the conjecture, and it records the many names under which the problem has been studied.

### What It Does NOT Prove
It does not solve the problem, and it is dated — later results (Krasikov–Lagarias, Tao, Hercher, Barina, …) are not in it.

### What We Learned
Good first reading: it explains the vocabulary (trajectory, stopping time, cycles) we use in the program, and shows that the 'easy to state, hard to prove' feeling is shared by professional mathematicians.

---

## A stopping time problem on the positive integers

### Title
A stopping time problem on the positive integers

### Author(s)
Riho Terras

### Year
1976

### Source / Publication
Acta Arithmetica 30, pp. 241–252

### Link
https://www.impan.pl/en/publishing-house/journals-and-series/acta-arithmetica/all/30/3/101028/a-stopping-time-problem-on-the-positive-integers

### Research Question
How long does it take for a Collatz trajectory to drop below its starting value, and how are those times distributed?

### Main Idea
Instead of asking when the trajectory reaches 1, ask when it first goes below n (the stopping time). If every n ≥ 2 had a finite stopping time, an induction argument would give the full conjecture. Terras studied the statistics of stopping times.

### Main Result
As summarised by MathWorld and Tao's paper: the set of integers with stopping time ≤ k has a limiting density that tends to 1 as k grows, so **almost all** integers have a finite stopping time. (C. J. Everett proved a similar result independently in 1977.)

### What It Does NOT Prove
'Almost all' means the exceptions have density zero; it does **not** say that every integer has a finite stopping time, so it does not prove the conjecture.

### What We Learned
This is exactly the 'stopping time' that our program prints for each start value, and it explains why that quantity matters theoretically.

---

## Bounds for the 3x+1 Problem using Difference Inequalities

### Title
Bounds for the 3x+1 Problem using Difference Inequalities

### Author(s)
Ilia Krasikov, Jeffrey C. Lagarias

### Year
2003

### Source / Publication
Acta Arithmetica 109, pp. 237–258 (arXiv:math/0205002)

### Link
https://arxiv.org/abs/math/0205002

### Research Question
How many integers up to x are guaranteed to have 1 in their orbit?

### Main Idea
Count the integers that reach 1 using systems of inequalities (difference inequalities) that relate counts in different residue classes; a computer-aided calculation then gives an explicit exponent.

### Main Result
At least x^0.84 of the integers up to x contain 1 in their forward orbit (for large x), improving earlier exponents (0.81 by Applegate and Lagarias).

### What It Does NOT Prove
x^0.84 is much smaller than x, so it does not show that *most* (let alone all) integers reach 1. It is a lower bound only.

### What We Learned
Shows the style of rigorous partial results: provable *counts* of converging numbers, in contrast to experiments that only test individual numbers.

---

## Almost all orbits of the Collatz map attain almost bounded values

### Title
Almost all orbits of the Collatz map attain almost bounded values

### Author(s)
Terence Tao

### Year
2022 (arXiv 2019)

### Source / Publication
Forum of Mathematics, Pi (2022); arXiv:1909.03562

### Link
https://arxiv.org/abs/1909.03562

### Research Question
For typical starting values, how small does the Collatz orbit get?

### Main Idea
Use probabilistic ideas (an invariant distribution, a modified 'Syracuse' map and careful analysis of how 3-adic structure spreads out) to show that for 'almost all' n the orbit comes below f(n), for any function f that tends to infinity — even extremely slowly.

### Main Result
For almost all n (in the sense of logarithmic density) the minimum value on the orbit is below f(n); an example in the paper is f(n) = log log log log n. Earlier, Terras/Everett gave 'below n' and Korec gave 'below n^θ' for θ > log 3 / log 4.

### What It Does NOT Prove
It does not say that every orbit reaches 1, nor does it exclude a rare divergent orbit or a rare cycle; the exceptional set can be non-empty (it just has logarithmic density zero).

### What We Learned
Shows what the strongest current 'typical case' evidence looks like and why 'almost all' is still not 'all'. It also explains the language of 'density' we use when we discuss our experiment's limits.

---

## There are no Collatz m-Cycles with m ≤ 91

### Title
There are no Collatz m-Cycles with m ≤ 91

### Author(s)
Christian Hercher

### Year
2023

### Source / Publication
Journal of Integer Sequences 26, Article 23.3.5 (with a published corrigendum); arXiv:2201.00406

### Link
https://cs.uwaterloo.ca/journals/JIS/VOL26/Hercher/hercher5.html

### Research Question
Could a non-trivial Collatz cycle (other than 1 → 4 → 2 → 1) exist, and how big would it have to be?

### Main Idea
Use the fact that all numbers up to a large bound are already verified to converge to derive lower bounds on the structure (number of 'local minima', m) of a hypothetical cycle.

### Main Result
No non-trivial cycle with m ≤ 91 exists (earlier work gave m ≥ 76, then m ≥ 83 with newer verification limits). The paper also shows how far the verification must be pushed to improve the bound on the number of odd members of a cycle.

### What It Does NOT Prove
It does not rule out all cycles (larger m remains possible), and it says nothing about divergent trajectories.

### What We Learned
Shows how computer verification (like our small experiment, but much bigger) feeds into theorems, and that a non-trivial cycle would have to be extremely large.

---

## Convergence verification of the Collatz problem

### Title
Convergence verification of the Collatz problem

### Author(s)
David Bařina

### Year
2021

### Source / Publication
The Journal of Supercomputing (2021)

### Link
https://www.fit.vut.cz/research/publication-file/12315/postprint.pdf

### Research Question
How can we verify convergence for huge ranges of start values efficiently?

### Main Idea
Replace huge pre-computed tables (size growing like 2^N) with small look-up tables (size growing like N), and implement it on CPU and on GPU with 128-bit numbers; also check 'path records' (the longest trajectories).

### Main Result
The abstract reports about 4.2×10^9 128-bit numbers per second on one CPU thread and about 2.2×10^11 per second with OpenCL on an NVIDIA RTX 2080.

### What It Does NOT Prove
It is verification (finite computation); it does not prove convergence for any number beyond the range actually tested.

### What We Learned
Shows what a 'professional' version of our experiment looks like and why arithmetic with 128-bit integers matters.

---

## Improved verification limit for the convergence of the Collatz conjecture

### Title
Improved verification limit for the convergence of the Collatz conjecture

### Author(s)
David Bařina

### Year
2025

### Source / Publication
The Journal of Supercomputing 81, article 810 (DOI 10.1007/s11227-025-07337-0)

### Link
https://www.fit.vut.cz/research/publication/c197809

### Research Question
How far can the Collatz conjecture be verified by computer?

### Main Idea
Algorithmic improvements and distribution of work to thousands of parallel workers on European supercomputers; the abstract reports a total GPU-vs-first-CPU speed-up of 1 335×.

### Main Result
The conjecture is verified for all start values up to 2^71 (about 2.4 × 10^21), and four new path records were found. The project web page reports a slightly higher limit (2075 × 2^60) as of August 2026.

### What It Does NOT Prove
This is evidence, not proof: nothing is said about start values above the limit, and no mathematical argument for all integers is given.

### What We Learned
Gives the up-to-date size of the finite evidence and shows that our program's range (up to 10^6 in the recorded run) is a tiny corner of what has been checked.

---

## The Undecidability of the Generalized Collatz Problem (and Conway's 'Unpredictable Iterations', 1972)

### Title
The Undecidability of the Generalized Collatz Problem (and Conway's 'Unpredictable Iterations', 1972)

### Author(s)
Stuart A. Kurtz, Janos Simon (Conway for the 1972 paper)

### Year
2007 (Conway: 1972)

### Source / Publication
TAMC 2007, Lecture Notes in Computer Science 4484, pp. 542–553 (DOI 10.1007/978-3-540-72504-6_49)

### Link
https://link.altmetric.com/details/904862

### Research Question
Is there an algorithm that can decide, for every Collatz-like function, whether all positive integers reach 1?

### Main Idea
Collatz-like functions (different multipliers depending on the remainder) can simulate computation, so questions about them can be as hard as the halting problem. Conway proved such undecidability for related problems in 1972; Kurtz and Simon proved it for a natural generalisation.

### Main Result
No algorithm decides the generalised Collatz problem in general (as summarised on the MathWorld Collatz Problem page).

### What It Does NOT Prove
MathWorld states that the proof does **not** apply to the original 3n+1 problem, so it does not show that the original problem is undecidable, nor does it settle it either way.

### What We Learned
Explains the 'computability' remark in the lab question and why the problem is harder than it looks: it belongs to a family that contains genuinely undecidable members.

---

## Overall lesson
Partial results (almost-all theorems, density bounds, cycle bounds) and huge verification projects both **support** the conjecture, but none of them closes the gap between "almost all" / "up to 2^71" and "every positive integer". That gap is what "open problem" means here.
