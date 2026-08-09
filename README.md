# Design & Analysis of Algorithms — Laboratory

> **From Algorithmic Ideas to Measured Evidence**

This repository is my academic record for the **Design and Analysis of Algorithms Laboratory** at **IIIT Bhubaneswar**.

The repository contains the algorithms, implementations, analyses, experimental data, plots, sample executions, and observations developed throughout the laboratory sessions.

The main objective is not only to write programs that produce the correct output, but also to understand **how and why their computational cost changes as the input grows**.

Each experiment therefore follows a simple progression:

```text
Problem
   ↓
Algorithmic Approach
   ↓
Implementation
   ↓
Complexity Analysis
   ↓
Experimental Measurement
   ↓
Data / Plots
   ↓
Observation
   ↓
Learning
````

Where experimental analysis is relevant, the programs record measurable quantities such as comparisons, operations, moves, or growth values. These measurements are stored as datasets and represented through tables or plots so that theoretical complexity can be compared with observed behaviour.

---

## Student Information

| Field          | Details                                      |
| -------------- | -------------------------------------------- |
| **Name**       | Ziya Nawaz                                   |
| **Student ID** | B125080                                      |
| **Branch**     | Computer Science and Engineering (CSE-B)     |
| **Institute**  | IIIT Bhubaneswar                             |
| **Course**     | Design and Analysis of Algorithms Laboratory |
| **Semester**   | B.Tech 3rd Semester                          |
| **Instructor** | Dr. Ajaya Kumar Dash                         |

---

# Repository at a Glance

This repository is organised laboratory-wise.

Each laboratory contains:

* The corresponding question paper
* One folder for every question
* A separate README for every question
* C implementations
* Sample executions
* Experimental datasets where applicable
* Graphs and visualisations where applicable

The structure is designed so that a question can be understood independently without having to search through the rest of the repository.

---

# Repository Structure

```text
DAA LAB/
│
├── README.md
│
├── WEEK 1/
│   ├── 2026_Week1_DAA_Lab_01.pdf
│   │
│   ├── Q1/
│   │   ├── README.md
│   │   ├── q1_growth_order.c
│   │   ├── sample.txt
│   │   └── plots/
│   │
│   ├── Q2/
│   │   ├── README.md
│   │   ├── q2_coin_toss.c
│   │   ├── sample.txt
│   │   ├── coin_toss_analysis.csv
│   │   └── coin_toss_analysis.svg
│   │
│   ├── Q3/
│   │   ├── README.md
│   │   ├── q3_bubble_sort_performance_analysis.c
│   │   ├── sample.txt
│   │   ├── bubble_sort_analysis.csv
│   │   └── bubble_sort_analysis.svg
│   │
│   ├── Q4/
│   │   ├── README.md
│   │   ├── q4_towers_of_hanoi.c
│   │   ├── sample.txt
│   │   ├── toh.csv
│   │   └── toh.svg
│   │
│   ├── Q5/
│   │   ├── README.md
│   │   ├── q5_partition_point.c
│   │   └── sample.txt
│   │
│   └── Q6/
│       ├── README.md
│       ├── q6_element_uniqueness.c
│       └── sample.txt
│
├── WEEK 2/
│   ├── 2026_Week2_DAA_Lab_02.pdf
│   │
│   ├── Q1/
│   │   ├── README.md
│   │   ├── dictionary.h
│   │   ├── unsorted_array.c
│   │   ├── sorted_array.c
│   │   ├── singly_unsorted.c
│   │   ├── singly_sorted.c
│   │   ├── doubly_unsorted.c
│   │   ├── doubly_sorted.c
│   │   ├── q1_dictionary_operations.c
│   │   ├── q1_operation_growth.csv
│   │   ├── q1_operation_complexity.csv
│   │   ├── sample.txt
│   │   └── plots/
│   │
│   ├── Q2/
│   │   ├── README.md
│   │   ├── q2_merge_sort_variants.c
│   │   ├── q2_merge_sort.csv
│   │   ├── sample.txt
│   │   └── plots/
│   │
│   └── Q3/
│       ├── README.md
│       ├── q3_merge_k_sorted_arrays.c
│       ├── q3_k_sorted_arrays.csv
│       ├── sample.txt
│       └── plots/
│
└── ...
```

---

# Laboratory Index

| Laboratory | Theme                                                            |           Date | Questions |
| ---------- | ---------------------------------------------------------------- | -------------: | --------: |
| **Lab 01** | Growth of Functions, Empirical Analysis & Fundamental Algorithms |   28 July 2026 |         6 |
| **Lab 02** | Data Structure Trade-offs & Divide-and-Conquer Analysis          | 04 August 2026 |         3 |

---

# WEEK 1 — Lab 01

### Focus: Growth, Behaviour and Experimental Analysis

The first laboratory introduces the fundamental idea behind algorithm analysis: **the amount of computational work depends on the size of the input**.

The questions explore different growth rates, randomised experiments, sorting behaviour, recursion, searching, and uniqueness testing.

| #      | Question                 | Main Concept                                    |
| ------ | ------------------------ | ----------------------------------------------- |
| **Q1** | Put Them in Order        | Ordering functions by rate of growth            |
| **Q2** | Fair vs Biased Coin      | Randomised simulation and empirical probability |
| **Q3** | Bubble Sort Performance  | Best-case and worst-case behaviour              |
| **Q4** | Towers of Hanoi          | Recursion and exponential growth                |
| **Q5** | Find the Partition Point | Binary search and logarithmic complexity        |
| **Q6** | Element Uniqueness       | Brute-force comparison and quadratic growth     |

The laboratory establishes the connection between mathematical growth functions and the behaviour of actual programs.

---

# WEEK 2 — Lab 02

### Focus: Representation, Decomposition and Scalability

The second laboratory studies an important idea in algorithm design:

> **The way a problem is represented or divided can significantly change its computational cost.**

Three different situations are considered.

| #      | Question                           | Main Concept                   | Complexity            |
| ------ | ---------------------------------- | ------------------------------ | --------------------- |
| **Q1** | Dictionary Operations              | Data-structure trade-offs      | Θ(1) to Θ(n)          |
| **Q2** | Merge Sort vs Three-Way Merge Sort | Divide and conquer             | Θ(n log n)            |
| **Q3** | Merging k Sorted Arrays            | Sequential vs balanced merging | Θ(nk²) vs Θ(nk log k) |

---

## Q1 — Dictionary Operations

The first problem examines seven dictionary operations across six different representations:

* Unsorted Array
* Sorted Array
* Singly Linked Unsorted List
* Singly Linked Sorted List
* Doubly Linked Unsorted List
* Doubly Linked Sorted List

The operations considered are:

* Search
* Insert
* Delete
* Maximum
* Minimum
* Predecessor
* Successor

The purpose is to understand how the same abstract operation can have different worst-case costs depending on the underlying data structure.

The experiment records the expected operation growth and provides data that can be examined independently from the theoretical complexity table.

---

## Q2 — Merge Sort vs Three-Way Merge Sort

Standard Merge Sort divides the input into two parts.

The modified version divides the input into three parts:

```text
Standard Merge Sort

             n
            / \
          n/2  n/2
```

```text
Three-Way Merge Sort

              n
           /  |  \
         n/3 n/3 n/3
```

The corresponding recurrences are:

```text
Two-way:
T(n) = 2T(n/2) + Θ(n)

Three-way:
T(n) = 3T(n/3) + Θ(n)
```

The important result is that the logarithm has a different base, but the asymptotic class remains:

```text
Θ(n log₃ n) = Θ(n log n)
```

The implementation and measurements provide an experimental view of this theoretical result.

---

## Q3 — Merging k Sorted Arrays

The third problem compares two approaches for merging `k` sorted arrays, each containing `n` elements.

### Sequential strategy

```text
A1 + A2
   ↓
result + A3
   ↓
result + A4
   ↓
...
```

The merged result becomes larger after every step, causing later merges to process increasingly large arrays.

Its asymptotic cost is:

```text
Θ(nk²)
```

### Balanced pairwise strategy

```text
A1 + A2      A3 + A4      A5 + A6
     ↓            ↓            ↓
      pairwise merged arrays
                 ↓
          continue merging
                 ↓
          final sorted array
```

The work is distributed more evenly across logarithmic levels.

Its asymptotic cost is:

```text
Θ(nk log k)
```

This problem demonstrates how **balancing the decomposition can improve scalability without changing the fundamental merging operation**.

---

# What the Laboratory Work Covers

## Asymptotic Analysis

The repository includes practical work involving:

* Big-O notation
* Big-Theta notation
* Big-Omega notation
* Rate of growth
* Logarithmic growth
* Polynomial growth
* Exponential growth
* Best-case analysis
* Worst-case analysis
* Constant factors
* Crossover behaviour
* Operation counting

---

## Algorithms

The laboratory implementations include concepts such as:

* Bubble Sort
* Merge Sort
* Three-Way Merge Sort
* Binary Search
* Partition Point Search
* Towers of Hanoi
* Merging Sorted Arrays
* Pairwise Comparison
* Recursive Algorithms
* Divide-and-Conquer Algorithms

---

## Data Structures

The repository also explores:

* Arrays
* Sorted arrays
* Unsorted arrays
* Singly linked lists
* Doubly linked lists
* Sorted linked representations
* Dictionary operations
* Data-structure-dependent operation costs

---

# Experimental Analysis

A major part of this repository is the attempt to connect **theoretical analysis with measurable behaviour**.

Where appropriate, programs produce numerical data such as:

* Number of comparisons
* Number of operations
* Number of moves
* Input size
* Number of arrays
* Growth values

The data can then be represented through:

```text
Program
   ↓
Measurements
   ↓
CSV
   ↓
Plot / Graph
   ↓
Comparison with expected growth
```

This makes the experimental portion reproducible and easier to inspect.

---

# Why CSV and Graphs Are Included

The numerical dataset and the graph serve different purposes.

### CSV

The CSV preserves the actual numerical observations.

It allows the values to be inspected directly instead of relying only on the visual appearance of a graph.

### SVG

SVG plots provide a scalable representation of the experiment and remain clear when enlarged.

### PNG

PNG versions provide convenient previews directly in common file browsers and repository interfaces.

Together:

```text
CSV → numerical evidence
SVG → scalable visualisation
PNG → quick visual inspection
```

---

# Documentation Approach

Each question has its **own README**.

The question-level documentation is designed to explain:

1. Problem statement
2. Objective
3. Algorithmic approach
4. Implementation idea
5. Complexity analysis
6. Experimental methodology where applicable
7. Sample execution
8. Observations
9. Learning outcomes
10. Conclusion
11. Files associated with the question

This keeps each experiment self-contained.

---

# Implementation Principles

The programs in this repository follow a few consistent principles.

### Clear implementations

The algorithmic logic is kept visible in the source code rather than hiding the important work behind high-level library functions.

### Complexity-conscious design

Each implementation is accompanied by an analysis of the relevant time and space requirements.

### Experimental evidence

When a problem involves empirical analysis, measurements are retained as data rather than only being described verbally.

### Reproducibility

Sample inputs, outputs and generated artefacts are kept with their respective questions wherever applicable.

### Consistent organisation

Every laboratory follows the same broad structure:

```text
Lab
 └── Question
      ├── README
      ├── Source
      ├── Sample
      ├── Data
      └── Visualisation
```

---

# Tools Used

| Tool                | Purpose                  |
| ------------------- | ------------------------ |
| **C / C11**         | Algorithm implementation |
| **GCC / MinGW-w64** | Compilation              |
| **Git**             | Version control          |
| **GitHub**          | Repository management    |
| **CSV**             | Experimental data        |
| **SVG / PNG**       | Graphical representation |

The algorithm implementations primarily rely on the C standard library.

---

# Compilation

A standard C program can be compiled using:

```bash
gcc -std=c11 -Wall -Wextra program.c -o program
```

For programs using mathematical functions:

```bash
gcc -std=c11 -Wall -Wextra program.c -o program -lm
```

On Windows with MinGW-w64:

```bash
gcc -std=c11 -Wall -Wextra program.c -o program.exe
```

Recommended compilation flags:

```bash
gcc -std=c11 -Wall -Wextra -O2 program.c -o program
```

---

# Learning Progress

The laboratory work currently covers the following areas.

### Analysis

* [x] Asymptotic notation
* [x] Ordering functions by growth
* [x] Best-case analysis
* [x] Worst-case analysis
* [x] Empirical analysis
* [x] Operation counting
* [x] Logarithmic growth
* [x] Polynomial growth
* [x] Exponential growth
* [x] Constant factors
* [x] Recurrence relations

### Algorithms

* [x] Bubble Sort
* [x] Merge Sort
* [x] Three-Way Merge Sort
* [x] Binary Search
* [x] Partition Point Search
* [x] Towers of Hanoi
* [x] Merging Sorted Arrays
* [x] Recursive Algorithms
* [x] Divide and Conquer

### Data Structures

* [x] Arrays
* [x] Sorted Arrays
* [x] Unsorted Arrays
* [x] Singly Linked Lists
* [x] Doubly Linked Lists
* [x] Dictionary Operations
* [x] Data-structure-dependent complexity

### Experimental Methods

* [x] Operation counting
* [x] Growth tables
* [x] CSV datasets
* [x] PNG visualisations
* [x] SVG visualisations
* [x] Comparison of theoretical and observed growth

---

# Repository Conventions

To maintain consistency throughout the coursework:

* Each laboratory is stored in a separate `WEEK` folder.
* Each question receives a separate `Q<number>` folder.
* Every question has its own README.
* Source files use descriptive names.
* Sample executions are kept alongside their programs.
* Experimental data remains with the question that generated it.
* Graphs are stored inside the corresponding question folder.
* The main README provides navigation and an overall picture of the laboratory.
* Question-level READMEs contain the detailed explanation of individual experiments.

---

# From Theory to Evidence

A recurring theme throughout these laboratories is that **algorithm correctness and algorithm efficiency are not the same thing**.

Two algorithms may produce identical outputs while behaving very differently as the input becomes large.

Similarly, two algorithms can belong to the same asymptotic class while still differing in practical cost because of constants, implementation choices, memory operations, or the structure of their recursion.

The laboratory therefore treats algorithm analysis as more than assigning a Big-O label.

It is a combination of:

```text
Mathematical Reasoning
        +
Algorithm Design
        +
Implementation
        +
Measurement
        +
Interpretation
```

That is the perspective this repository is intended to document.

---

# Academic Note

This repository is maintained as a personal coursework record for the **Design and Analysis of Algorithms Laboratory**.

The implementations, analyses, experiments, datasets, and observations are maintained for academic learning and reference.

Please use the material responsibly and do not submit it as someone else's coursework.

---

## Author

**Ziya Nawaz**

**B125080**

Computer Science and Engineering — CSE-B
IIIT Bhubaneswar

---

> **Analyse the growth. Understand the trade-off. Verify the result.*
