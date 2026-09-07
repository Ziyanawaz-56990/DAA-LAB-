# DAA LAB-6 : Set 1 (Dynamic Programming) — All Program Outputs

Below is the exact console transcript obtained by compiling and running each
program (gcc, Ubuntu 24.04) against the sample test cases described in each
Q*/algorithm.md file. This is the real, verified output — not fabricated.

```
===================================================================
 DAA LAB-6 : SET 1 (DYNAMIC PROGRAMMING) - CONSOLIDATED PROGRAM OUTPUTS
===================================================================

###################################################################
# QUESTION 1 : Nth Fibonacci Number using DP
###################################################################

>>> Test Case 1: n = 10
=========================================
 DAA LAB-6 : Q1 - Nth Fibonacci using DP 
=========================================
Enter n (0 <= n <= 1000): 
--- Results for n = 10 ---
Bottom-Up Tabulation   (O(n) time, O(n) space) : 55
Space-Optimized DP     (O(n) time, O(1) space) : 55
Top-Down Memoization   (O(n) time, O(n) space) : 55

First min(n+1,15) Fibonacci numbers: 0 1 1 2 3 5 8 13 21 34 55 

>>> Test Case 2: n = 20
=========================================
 DAA LAB-6 : Q1 - Nth Fibonacci using DP 
=========================================
Enter n (0 <= n <= 1000): 
--- Results for n = 20 ---
Bottom-Up Tabulation   (O(n) time, O(n) space) : 6765
Space-Optimized DP     (O(n) time, O(1) space) : 6765
Top-Down Memoization   (O(n) time, O(n) space) : 6765

First min(n+1,15) Fibonacci numbers: 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 

###################################################################
# QUESTION 2 : 0/1 Knapsack using DP
###################################################################

>>> Test Case: n=4 items, W=50, (w,p) pairs = (10,60)(20,100)(30,120)(40,200)
=================================================
 DAA LAB-6 : Q2 - 0/1 Knapsack using DP
=================================================
Enter number of items (n): Enter knapsack capacity (W): Enter weight and profit for each item:
  Item 1 - weight profit:   Item 2 - weight profit:   Item 3 - weight profit:   Item 4 - weight profit: 
--- Result ---
Maximum profit achievable within capacity 50 = 260
Items included (1-indexed): Item1(w=10,p=60) Item4(w=40,p=200) 

Complexity Analysis:
  Time  complexity : O(n * W) = O(4 * 50) = O(200)
  Space complexity : O(n * W) for the table (optimizable to O(W) using a 1-D rolling array)

###################################################################
# QUESTION 3 : Longest Common Subsequence (LCS) using DP
###################################################################

>>> Test Case 1: X=AGGTAB, Y=GXTXAYB
=================================================
 DAA LAB-6 : Q3 - Longest Common Subsequence (LCS)
=================================================
Enter first string  X: Enter second string Y: 
--- Result ---
Length of LCS(AGGTAB, GXTXAYB) = 4
LCS string        : GTAB

Complexity Analysis:
  Time  complexity : O(m * n) = O(6 * 7) = O(42)
  Space complexity : O(m * n) for the DP table (optimizable to O(min(m,n)) for length-only)

>>> Test Case 2: X=ABCBDAB, Y=BDCABA
=================================================
 DAA LAB-6 : Q3 - Longest Common Subsequence (LCS)
=================================================
Enter first string  X: Enter second string Y: 
--- Result ---
Length of LCS(ABCBDAB, BDCABA) = 4
LCS string        : BCBA

Complexity Analysis:
  Time  complexity : O(m * n) = O(7 * 6) = O(42)
  Space complexity : O(m * n) for the DP table (optimizable to O(min(m,n)) for length-only)

###################################################################
# QUESTION 4 : Matrix Chain Multiplication using DP
###################################################################

>>> Test Case 1 (from assignment): N=4, arr[]={10,30,5,60}
=================================================
 DAA LAB-6 : Q4 - Matrix Chain Multiplication (DP)
=================================================
Enter N (number of dimension entries, matrices = N-1): Enter the 4 dimension values: 
--- Result ---
Number of matrices        : 3
Dimension array arr[]     : {10, 30, 5, 60}
Minimum scalar multiplications = 4500
Optimal Parenthesization  : ((A1B2)C3)

Complexity Analysis:
  Time  complexity : O(N^3)   (three nested loops: len, i, k)
  Space complexity : O(N^2)   (dp and split tables)

>>> Test Case 2 (classical CLRS example): N=6, arr[]={30,35,15,5,10,20}
=================================================
 DAA LAB-6 : Q4 - Matrix Chain Multiplication (DP)
=================================================
Enter N (number of dimension entries, matrices = N-1): Enter the 6 dimension values: 
--- Result ---
Number of matrices        : 5
Dimension array arr[]     : {30, 35, 15, 5, 10, 20}
Minimum scalar multiplications = 11875
Optimal Parenthesization  : ((A1(B2C3))(D4E5))

Complexity Analysis:
  Time  complexity : O(N^3)   (three nested loops: len, i, k)
  Space complexity : O(N^2)   (dp and split tables)
```
