# Design and Analysis of Algorithms (DAA) — Lab 10

**Graphs · Disjoint Set Union (Union-Find) · Connected Components**

| | |
|---|---|
| **Course** | Design and Analysis of Algorithms |
| **Programme** | B.Tech (CS-B and CE), 3rd Semester — IIIT Bhubaneswar |
| **Instructor** | Dr. Ajaya Kumar Dash |
| **Lab date** | October 13, 2026 |
| **Difficulty** | Medium – Hard |
| **Language** | C (C99, compiled with `gcc -O2 -Wall -Wextra`) |
| **Submitted by** | _Ziya Nawaz — \ B125080_ |

---

## Contents

1. [Folder structure](#1-folder-structure)
2. [Problems at a glance](#2-problems-at-a-glance)
3. [Disjoint Set Union — the common toolkit](#3-disjoint-set-union--the-common-toolkit)
4. [How to build and run](#4-how-to-build-and-run)
5. [Input formats](#5-input-formats)
6. [Testing and validation](#6-testing-and-validation)
7. [Conventions and assumptions](#7-conventions-and-assumptions)
8. [Complexity summary](#8-complexity-summary)

---

## 1. Folder structure

```
DAA_Lab_10/
├── README.md                              <- this file
├── Makefile                               <- make all | run | test | clean
├── Question_Paper/
│   └── DAA_LAB_10.pdf                     <- original question sheet
├── Q1_Online_Bipartiteness/
│   ├── README.md                          <- problem, algorithm, proof, complexity, dry run
│   ├── solution.c                         <- C program
│   ├── sample_input.txt
│   └── sample_output.txt
├── Q2_Dynamic_Islands_Grid/               (same four files)
├── Q3_Max_Weight_Component_Size_K/        (same four files)
├── Q4_Reverse_Time_DSU/                   (same four files)
├── Q5_Lex_Smallest_Permutation/           (same four files)
└── tests/
    └── stress_test.py                     <- randomised tests against brute-force references
```

Each question folder is self-contained: open its `README.md` for the full write-up, `solution.c` for the code.

---

## 2. Problems at a glance

| # | Problem | Core idea | Input representation | Time | Space |
|---|---|---|---|---|---|
| 1 | [Online bipartiteness testing](Q1_Online_Bipartiteness/) | DSU **with parity**: an edge inside a component whose endpoints have equal parity closes an odd cycle | Edge stream | O((V+E)·α(V)) | O(V) |
| 2 | [Dynamic connected components in a grid](Q2_Dynamic_Islands_Grid/) | DSU on flattened ids `r·C+c`; +1 island per new cell, −1 per successful merge | Implicit grid (4 neighbours) | O(RC + K·α(RC)) | O(RC) |
| 3 | [Max-weight component with size ≤ K](Q3_Max_Weight_Component_Size_K/) | DSU keeps (size, sum); **max-heap with lazy deletion** because the maximum can drop when a component outgrows K | Weights + edge stream | O((V+E)·α(V) + V log V) | O(V) |
| 4 | [Reverse time-step DSU](Q4_Reverse_Time_DSU/) | DSU cannot delete → process deletions **backwards as insertions** | Adjacency list | O((V+E)·α(V)) | O(V+E) |
| 5 | [Lexicographically smallest permutation](Q5_Lex_Smallest_Permutation/) | Swappable positions form components; sort values inside each component onto its sorted positions (bucket placement, no sort) | Edge list of positions | O((n+m)·α(n)) | O(n) |

**Common thread:** every question is a *connectivity that only grows* (or can be made to grow by reversing time),
which is precisely the situation where DSU beats repeated BFS/DFS.

---

## 3. Disjoint Set Union — the common toolkit

A DSU maintains a partition of `{1..n}` into disjoint sets, supporting:

* `find(x)` — representative (root) of the set containing `x`;
* `union(a, b)` — merge the sets containing `a` and `b`.

```
find(x):                                 // path compression (path halving shown)
    while parent[x] ≠ x:
        parent[x] ← parent[parent[x]]
        x ← parent[x]
    return x

union(a, b):                             // union by size
    ra ← find(a);  rb ← find(b)
    if ra == rb: return false
    if size[ra] < size[rb]: swap(ra, rb)
    parent[rb] ← ra;  size[ra] += size[rb]
    return true
```

**Why it is fast.** Union by size keeps every tree height ≤ log₂ n; path compression flattens trees as a side
effect of `find`. Together, any sequence of `m` operations on `n` elements costs **O((n + m)·α(n))**
(Tarjan, 1975), where α is the inverse Ackermann function — below 5 for any input that fits in the universe.

**What each question adds on top of the plain DSU**

| Question | Extra information stored at the root |
|---|---|
| Q1 | `parity[x]` — colour of `x` relative to its parent (compressed to the root) |
| Q2 | nothing extra; `parent = -1` marks water; a counter tracks islands |
| Q3 | component `size` and total `sum`, plus a lazy-deletion heap of valid candidates |
| Q4 | component `sum`, a `best` running maximum, and `active[]` flags |
| Q5 | component `size` to lay out contiguous buckets |

---

## 4. How to build and run

Requirements: `gcc` (any C99 compiler) and, for the tests only, `python3`.

```bash
# build all five programs
make

# run each program on its sample input
make run

# or one question at a time
cd Q3_Max_Weight_Component_Size_K
gcc -O2 -Wall -o q3 solution.c
./q3 < sample_input.txt

# interactive input also works: run ./q3 and type the input

# randomised verification (see section 6)
make test

# remove binaries
make clean
```

On Windows (MinGW) use `gcc -O2 -o q3.exe solution.c` and `q3.exe < sample_input.txt`.

---

## 5. Input formats

All programs read from **standard input**. Vertices / positions are **1-indexed**; grid cells (Q2) are **0-indexed**.

| Q | Format |
|---|---|
| 1 | `V E` then `E` lines `u v` |
| 2 | `R C K` then `K` lines `r c` |
| 3 | `V K E`, then `w_1 … w_V`, then `E` lines `u v wt` |
| 4 | `V E`, then `w_1 … w_V`, then `E` lines `u v`, then `p_1 … p_V` |
| 5 | `n m`, then `A_1 … A_n`, then `m` lines `u v` |

Invalid input produces a message on `stderr` and exit code `1` (never a crash).

---

## 6. Testing and validation

`tests/stress_test.py` compiles every solution **with AddressSanitizer and UndefinedBehaviorSanitizer** and checks
it against an independent, deliberately simple brute-force reference on thousands of random small inputs:

| Q | Brute-force reference |
|---|---|
| 1 | After each tentative edge, re-run a BFS 2-colouring of the whole graph |
| 2 | After each addition, recount islands with DFS over all land cells |
| 3 | After each edge, recompute every component by BFS and take the best one with size ≤ K |
| 4 | After each deletion, recompute components of the surviving induced subgraph |
| 5 | Per component: sort positions, sort values, zip |

The random generators deliberately include **self-loops, duplicate edges, duplicate land cells, negative weights (Q3),
disconnected graphs, `K` smaller / larger than any component, `E = 0`**.

```
$ make test
Q1: 2000 random tests passed  (Q1_Online_Bipartiteness)
Q2: 2000 random tests passed  (Q2_Dynamic_Islands_Grid)
Q3: 2000 random tests passed  (Q3_Max_Weight_Component_Size_K)
Q4: 2000 random tests passed  (Q4_Reverse_Time_DSU)
Q5: 2000 random tests passed  (Q5_Lex_Smallest_Permutation)
ALL TESTS PASSED
```

---

## 7. Conventions and assumptions

* **Q3** — "size" means number of vertices; the **edge weight is read but not used** because it affects neither
  connectivity nor vertex sums. The answer is printed initially and after every edge; `NONE` means no component fits within `K`.
* **Q4** — vertex values are **non-negative**; after the last deletion the graph is empty, so the answer is `0`.
* **Q2** — adding land on an existing land cell leaves the count unchanged; out-of-range operations are ignored and reported.
* **Q5** — `A` must be a permutation of `1..n` (validated); swaps act on **positions**.
* Sums use `long long`; sizes/indices use `int`.

---

## 8. Complexity summary

α(·) is the inverse Ackermann function (practically ≤ 4).

| Q | Time | Space | Naïve baseline it replaces |
|---|---|---|---|
| 1 | O(V + E·α(V)) | O(V) | BFS 2-colouring after each edge: O(E·(V+E)) |
| 2 | O(R·C + K·α(R·C)) | O(R·C) | BFS per addition: O(K·R·C) |
| 3 | O((V+E)·α(V) + V log V) | O(V) | Rescan all components per edge: O(E·V) |
| 4 | O((V+E)·α(V)) | O(V+E) | BFS per deletion: O(V·(V+E)) |
| 5 | O((n+m)·α(n)) | O(n) | Search over permutation states: exponential |

---

_Lab 10 — DAA, IIIT Bhubaneswar._
