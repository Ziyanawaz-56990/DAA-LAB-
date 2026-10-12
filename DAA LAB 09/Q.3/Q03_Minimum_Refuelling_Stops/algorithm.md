# Q3 - Minimum Number of Refuelling Stops (reverse / look-back greedy)

## 1. Understanding the problem
Car starts with F units of fuel, 1 unit of fuel = 1 unit of distance. Station i is at distance d_i and gives f_i fuel (the whole amount when you stop). Find the **minimum number of stops** to reach distance D, or -1 if impossible.
**Input representation:** integers `D`, `F` and arrays `d[]`, `f[]` (use 64-bit integers because distances can be large). Stations are sorted by distance first.

## 2. Greedy idea ("reverse" greedy)
Drive as far as possible without stopping. We **do not decide** where to refuel in advance. Whenever we cannot reach the next place, we *look back* and pretend we had refuelled at the **station with the most fuel among all stations we already passed** and have not used. A max-heap stores the fuel amounts of passed stations.
*Why correct:* every stop we make is only made when forced; at that moment the biggest passed station gives the farthest reach, so no other choice of the same number of stops reaches farther (exchange argument).

## 3. Pseudocode
```
sort stations by distance
reach = F; stops = 0; i = 0; max-heap H = empty
while reach < D:
    while i < n and d[i] <= reach: push f[i] into H; i++
    if H empty: return -1
    reach += extract-max(H); stops++
return stops
```

## 4. Dry run (Test 1: D=100, F=10, stations (10,60),(20,30),(30,30),(60,40))
reach=10: push 60 -> pop 60, reach 70, stops 1. Push 30,30,40 (stations at 20,30,60). Pop 40 -> reach 110 >= 100, stops 2. **Answer 2.**

## 5. Complexity
Each station is pushed once and popped at most once: **O(n log n)** time (sorting + heap), O(n) space.

## 6. Validation done by the program
Independent O(n^2) DP: dp[j] = farthest distance reachable with j stops. Results compared on 4 hand tests + 1000 random cases (fixed seed): all match.

## 7. Edge cases
F >= D (0 stops), no station within reach (-1), unsorted input, stations beyond D.

## 8. Possible viva questions
* Why a max-heap and not the nearest station? (The farthest-reaching choice minimises stops.)
* Why is it called reverse greedy? (Decision to stop at a station is made *later*, looking back.)
