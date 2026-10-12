# Q8 - Minimum Number of Meeting Rooms

## 1. Understanding the problem
Meeting i occupies [s_i, e_i). Two meetings clash if their intervals overlap; a meeting ending at the exact time another starts does **not** clash. Find the minimum number of rooms = the **maximum number of meetings in progress at the same time**.
**Input representation:** two integer arrays `start[]`, `end[]`.

## 2. Greedy idea (sweep line)
Sort all start times and all end times **separately**. Walk through meetings in order of start time. Before counting the new meeting, free every room whose meeting has already ended (`end <= current start`). Then occupy one room and remember the largest occupancy.
Why separate sorting is fine: only *how many* meetings are active matters, not which ones.

## 3. Pseudocode
```
sort S (starts), sort E (ends); rooms = 0; best = 0; j = 0
for i = 0..n-1:
    while E[j] <= S[i]: rooms--; j++
    rooms++; best = max(best, rooms)
return best
```

## 4. Dry run ([0,30],[5,10],[15,20])
S = 0 5 15, E = 10 20 30. i=0: rooms 1. i=1 (start 5): E[0]=10 > 5, rooms 2. i=2 (start 15): E[0]=10 <= 15, free -> 1, then +1 -> 2. best = **2**.

## 5. Complexity
Sorting O(n log n); sweep O(n). **O(n log n) time, O(n) space.**

## 6. Validation done by the program
Independent O(n^2) count of "meetings in progress at each start time". 5 tests + 2000 random cases (fixed seed): all match.

## 7. Edge cases
Back-to-back meetings (share a room), identical meetings, a single meeting, all overlapping.

## 8. Possible viva questions
* Alternative method? (Min-heap of end times of occupied rooms, also O(n log n).)
* Link to interval scheduling? (Minimum rooms = interval graph colouring = maximum clique = max overlap.)
