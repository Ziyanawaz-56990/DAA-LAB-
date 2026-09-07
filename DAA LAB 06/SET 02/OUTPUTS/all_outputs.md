# DAA LAB-6 : Set 2 (Arrays, Matrices, Convolution, Sorting) — All Program Outputs

Below is the exact console transcript obtained by compiling and running each
program (gcc, Ubuntu 24.04) against sample test cases. This is real, verified
output — not fabricated.

```
===================================================================
 DAA LAB-6 : SET 2 - CONSOLIDATED PROGRAM OUTPUTS
===================================================================

###################################################################
# QUESTION 1 : 1D Array Operations and Their Complexities
###################################################################

>>> Test Case 1: n = 10
=========================================================
 DAA LAB-6 (PDF) : Q1 - 1D Array Operations & Complexity
=========================================================
Enter number of elements n: Enter 10 integers: 
Original array: [ 12 45 2 45 88 12 6 100 6 45 ]

(i)    Maximum element              : 100      [O(n)]
(ii)   First largest, Second largest : 100, 88  [O(n)]
(iii)  Mean                          : 36.1000  [O(n)]
(iv)   Median                        : 28.5000  [O(n log n)]
(v)    Standard deviation            : 33.3930  [O(n)]
(vi)   Mode                          : 45      [O(n log n)]
(vii)  After removing duplicates    : [ 2 6 12 45 88 100 ]
       (length reduced from 10 to 6)  [O(n log n)]
(viii) Reversed array                : [ 45 6 100 6 12 88 45 2 45 12 ]
       [O(n)]
(ix)   Partition pivot = 12           : [ 12 45 45 88 12 100 45 2 6 6 ]
       (elements >= pivot occupy indices [0..7), elements < pivot occupy [7..10))  [O(n)]

>>> Test Case 2: n = 6, includes negative and repeated values
=========================================================
 DAA LAB-6 (PDF) : Q1 - 1D Array Operations & Complexity
=========================================================
Enter number of elements n: Enter 6 integers: 
Original array: [ -3 7 7 2 -3 15 ]

(i)    Maximum element              : 15      [O(n)]
(ii)   First largest, Second largest : 15, 7  [O(n)]
(iii)  Mean                          : 4.1667  [O(n)]
(iv)   Median                        : 4.5000  [O(n log n)]
(v)    Standard deviation            : 6.3355  [O(n)]
(vi)   Mode                          : -3      [O(n log n)]
(vii)  After removing duplicates    : [ -3 2 7 15 ]
       (length reduced from 6 to 4)  [O(n log n)]
(viii) Reversed array                : [ 15 -3 2 7 7 -3 ]
       [O(n)]
(ix)   Partition pivot = -3           : [ -3 7 7 2 -3 15 ]
       (elements >= pivot occupy indices [0..6), elements < pivot occupy [6..6))  [O(n)]

###################################################################
# QUESTION 2 : 2D Square Matrix Operations and Their Complexities
###################################################################

>>> Test Case 1: n=3, A symmetric, B = Identity
=========================================================
 DAA LAB-6 (PDF) : Q2 - 2D Square Matrix Ops & Complexity
=========================================================
Enter matrix size n (n x n): Enter 3 x 3 matrix A (row by row):
Enter 3 x 3 matrix B (row by row):

Matrix A:
  [    2.000    1.000    3.000 ]
  [    1.000    4.000    5.000 ]
  [    3.000    5.000    6.000 ]
Matrix B:
  [    1.000    0.000    0.000 ]
  [    0.000    1.000    0.000 ]
  [    0.000    0.000    1.000 ]

(i)   A + B  [O(n^2)] :
  [    3.000    1.000    3.000 ]
  [    1.000    5.000    5.000 ]
  [    3.000    5.000    7.000 ]

(ii)  A x B  [O(n^3)] :
  [    2.000    1.000    3.000 ]
  [    1.000    4.000    5.000 ]
  [    3.000    5.000    6.000 ]

(iii) Is A a zero matrix? [O(n^2)] : NO
(iv)  Is A symmetric?     [O(n^2)] : YES
(v)   Determinant of A    [O(n!) cofactor expansion] : -14.0000
(vi)  Transpose of A (in-place)  [O(n^2)] :
  [    2.000    1.000    3.000 ]
  [    1.000    4.000    5.000 ]
  [    3.000    5.000    6.000 ]
(vii) Dominant eigenvalue of A  [O(k*n^2) power iteration] : 11.073033
      Corresponding eigenvector (normalized, largest component = 1): [ 0.415031 0.765588 1.000000 ]

Complexity Summary (n = 3):
  Addition        : O(n^2) = O(9)
  Multiplication  : O(n^3) = O(27)
  Zero-check      : O(n^2)
  Symmetric-check : O(n^2)
  Determinant     : O(n!) via cofactor expansion (O(n^3) via LU decomposition)
  Transpose       : O(n^2), O(1) extra space (in-place)
  Eigen (power it): O(k*n^2), k = number of iterations to converge

>>> Test Case 2: n=3, A non-symmetric (confirms transpose actually changes A)
=========================================================
 DAA LAB-6 (PDF) : Q2 - 2D Square Matrix Ops & Complexity
=========================================================
Enter matrix size n (n x n): Enter 3 x 3 matrix A (row by row):
Enter 3 x 3 matrix B (row by row):

Matrix A:
  [    2.000    3.000    1.000 ]
  [    4.000    5.000    6.000 ]
  [    7.000    8.000    9.000 ]
Matrix B:
  [    1.000    1.000    1.000 ]
  [    2.000    2.000    2.000 ]
  [    3.000    3.000    3.000 ]

(i)   A + B  [O(n^2)] :
  [    3.000    4.000    2.000 ]
  [    6.000    7.000    8.000 ]
  [   10.000   11.000   12.000 ]

(ii)  A x B  [O(n^3)] :
  [   11.000   11.000   11.000 ]
  [   32.000   32.000   32.000 ]
  [   50.000   50.000   50.000 ]

(iii) Is A a zero matrix? [O(n^2)] : NO
(iv)  Is A symmetric?     [O(n^2)] : NO
(v)   Determinant of A    [O(n!) cofactor expansion] : 9.0000
(vi)  Transpose of A (in-place)  [O(n^2)] :
  [    2.000    4.000    7.000 ]
  [    3.000    5.000    8.000 ]
  [    1.000    6.000    9.000 ]
(vii) Dominant eigenvalue of A  [O(k*n^2) power iteration] : 15.653428
      Corresponding eigenvector (normalized, largest component = 1): [ 0.214703 0.643813 1.000000 ]

Complexity Summary (n = 3):
  Addition        : O(n^2) = O(9)
  Multiplication  : O(n^3) = O(27)
  Zero-check      : O(n^2)
  Symmetric-check : O(n^2)
  Determinant     : O(n!) via cofactor expansion (O(n^3) via LU decomposition)
  Transpose       : O(n^2), O(1) extra space (in-place)
  Eigen (power it): O(k*n^2), k = number of iterations to converge

###################################################################
# QUESTION 3 : Convolution via FFT (Divide & Conquer), O(n log n)
###################################################################

>>> Test Case 1: A (len 3) = [1,2,3], B (len 5) = [4,5,6,7,8]
=========================================================
 DAA LAB-6 (PDF) : Q3 - Convolution via FFT (Divide&Conquer)
=========================================================
Enter length of vector A (m): Enter 3 elements of A: Enter length of vector B (n), where n >= m: Enter 5 elements of B: 
--- Result: C = A convolved with B  (length 7) ---
FFT Divide&Conquer result : [ 4.0000 13.0000 28.0000 34.0000 40.0000 37.0000 24.0000 ]
Naive O(n*m) cross-check  : [ 4.0000 13.0000 28.0000 34.0000 40.0000 37.0000 24.0000 ]
Max absolute difference between FFT and naive result: 0.0000000000 (MATCH)

Complexity Analysis:
  Naive convolution : O(n*m)
  FFT-based D&C     : O(L log L) where L = smallest power of 2 >= (m+n-1)
                      Since n >= m, L = O(n)  =>  Overall = O(n log n)
  Space             : O(L) = O(n)

>>> Test Case 2: A (len 4) = [1,0,-1,2], B (len 6) = [2,1,0,-1,1,3]
=========================================================
 DAA LAB-6 (PDF) : Q3 - Convolution via FFT (Divide&Conquer)
=========================================================
Enter length of vector A (m): Enter 4 elements of A: Enter length of vector B (n), where n >= m: Enter 6 elements of B: 
--- Result: C = A convolved with B  (length 9) ---
FFT Divide&Conquer result : [ 2.0000 1.0000 -2.0000 2.0000 3.0000 4.0000 -3.0000 -1.0000 6.0000 ]
Naive O(n*m) cross-check  : [ 2.0000 1.0000 -2.0000 2.0000 3.0000 4.0000 -3.0000 -1.0000 6.0000 ]
Max absolute difference between FFT and naive result: 0.0000000000 (MATCH)

Complexity Analysis:
  Naive convolution : O(n*m)
  FFT-based D&C     : O(L log L) where L = smallest power of 2 >= (m+n-1)
                      Since n >= m, L = O(n)  =>  Overall = O(n log n)
  Space             : O(L) = O(n)

###################################################################
# QUESTION 4 : Sorting via reverse(p,i,j) — O(n) reversals (Part A)
#              and O(n log^2 n) cost (Part B)
###################################################################

>>> Test Case 1: p = [5,2,8,1,4,7,3,6]  (n=8, random permutation)
=========================================================
 DAA LAB-6 (PDF) : Q4 - Sorting via reverse(p,i,j) operations
=========================================================
NOTE: Part A requires the input to be an actual permutation of
      1..n (each value exactly once) - it looks for value (i+1)
      directly. Part B is a general comparison-sort and works for
      ANY array of comparable elements (duplicates included).

Enter n (size of the permutation of 1..n): Enter the permutation (n distinct integers from 1 to n): 
Original permutation: [ 5 2 8 1 4 7 3 6 ]

--- Part A: Selection-by-Reversal (O(n) reversals) ---
Result           : [ 1 2 3 4 5 6 7 8 ]
Sorted correctly?: YES
Reversals used   : 7   (theoretical bound: O(n) = O(8))
Total reversal cost (sum of segment lengths): 20  (can be Theta(n^2) worst case)

--- Part B: Merge-Sort-by-Rotation (O(n log^2 n) cost) ---
Result           : [ 1 2 3 4 5 6 7 8 ]
Sorted correctly?: YES
Reversals used   : 21
Total reversal cost (sum of segment lengths): 38
Reference bound n*log2(n)^2 (n=8) approx = 72.0

>>> Test Case 2: p = [8,7,6,5,4,3,2,1]  (n=8, worst-case reversed permutation)
=========================================================
 DAA LAB-6 (PDF) : Q4 - Sorting via reverse(p,i,j) operations
=========================================================
NOTE: Part A requires the input to be an actual permutation of
      1..n (each value exactly once) - it looks for value (i+1)
      directly. Part B is a general comparison-sort and works for
      ANY array of comparable elements (duplicates included).

Enter n (size of the permutation of 1..n): Enter the permutation (n distinct integers from 1 to n): 
Original permutation: [ 8 7 6 5 4 3 2 1 ]

--- Part A: Selection-by-Reversal (O(n) reversals) ---
Result           : [ 1 2 3 4 5 6 7 8 ]
Sorted correctly?: YES
Reversals used   : 1   (theoretical bound: O(n) = O(8))
Total reversal cost (sum of segment lengths): 8  (can be Theta(n^2) worst case)

--- Part B: Merge-Sort-by-Rotation (O(n log^2 n) cost) ---
Result           : [ 1 2 3 4 5 6 7 8 ]
Sorted correctly?: YES
Reversals used   : 21
Total reversal cost (sum of segment lengths): 48
Reference bound n*log2(n)^2 (n=8) approx = 72.0
```
