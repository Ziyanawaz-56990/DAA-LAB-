# Q7 - Minimise Deviation in Array

## 1. Understanding the problem
Operations: odd x -> 2x, even x -> x/2. An element x = o * 2^e (o odd) can only become o*2^k for k in [0, max(e,1)]. Minimise max(A) - min(A).
**Input representation:** array of positive integers (64-bit); a max-heap of current values; a variable `mn` for the current minimum.

## 2. Greedy idea (two-way -> one-way)
Multiplying an odd number by 2 is its **only** change, and it can be undone by division. So first **double every odd number**. Now each element sits at its *largest* possible value and from here we only ever need to **divide the maximum by 2** (the only way to lower the max).
Repeat: take the maximum; record deviation = max - min; if max is even, halve it, push it back, update min; if max is odd, stop (the maximum can no longer decrease, so deviation cannot improve).

## 3. Pseudocode
```
for each a[i]: if odd, a[i] *= 2; push a[i]; mn = min(mn, a[i])
best = top - mn
while top is even:
    x = pop(); push(x/2); mn = min(mn, x/2)
    best = min(best, top - mn)
return best
```

## 4. Dry run (1 2 3 4)
Double odds: 2 2 6 4, min 2, max 6, dev 4. Halve 6->3: {4,3,2,2}, min 2, dev 2. Halve 4->2: {3,2,2,2}, dev 1. Max 3 is odd -> stop. **Answer 1.**

## 5. Complexity
Each element can be halved at most log2(M) times (M = largest value), so at most n*log M heap operations, each O(log n): **O(n log n log M)** time, O(n) space.

## 6. Validation done by the program
Exhaustive search over every combination of reachable values for each element (n <= 5). 5 tests + 1000 random cases (fixed seed): all match.

## 7. Edge cases
Single element (0), all equal (0), already-odd arrays, powers of two.

## 8. Possible viva questions
* Why double the odds first? (Reduces two-way moves to one direction without losing any reachable configuration.)
* Why stop when the max is odd? (An odd max cannot be reduced; every other element's max-value state can only be >= current max.)
