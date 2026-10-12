# Q4 - Minimum Cost to Connect Sticks

## 1. Understanding the problem
Join sticks two at a time; each join costs the sum of the two lengths and creates one longer stick. Minimise the total cost to end with one stick.
**Input representation:** array `L[0..n-1]` of positive integers (64-bit for the cost).

## 2. Greedy idea
A stick that is joined early gets **counted again** every time the resulting stick is joined later. So short sticks should be joined first and long ones last: always join the **two shortest** sticks (min-heap). This is exactly the Huffman merge pattern, so the optimality proof is the same as Huffman's.

## 3. Pseudocode
```
min-heap H with all lengths; cost = 0
while |H| > 1:
    a = extract-min(H); b = extract-min(H)
    cost += a + b
    insert (a + b) into H
return cost
```

## 4. Dry run (Test 2: 1, 8, 3, 5)
join 1+3=4 (cost 4); heap {4,8,5}: join 4+5=9 (cost 9); heap {8,9}: join 8+9=17 (cost 17). Total 4+9+17 = **30**.

## 5. Complexity
n-1 iterations x O(log n) heap operations = **O(n log n)** time, O(n) space. Building the heap can be done in O(n).

## 6. Validation done by the program
Exponential brute force trying **every pair at every step** (n <= 7). 5 hand tests + 500 random cases (fixed seed): all match.

## 7. Edge cases
n = 1 gives cost 0; all equal sticks; very large totals (use 64-bit integers).

## 8. Possible viva questions
* How is this related to Huffman coding? (Same algorithm: total cost = weighted external path length.)
* What goes wrong if you join the two longest sticks? (Large values are added repeatedly.)
