# Q5 - Candy Distribution (bi-directional greedy)

## 1. Understanding the problem
Array `rating[0..n-1]`. Every child gets >= 1 candy; if rating[i] > rating[i-1] then candy[i] > candy[i-1]; if rating[i] > rating[i+1] then candy[i] > candy[i+1]. Equal ratings place no restriction. Minimise the total.
**Input representation:** integer array of ratings.

## 2. Greedy idea
There are two independent rules (left neighbour and right neighbour). Handle each in one pass:
* **Left-to-right pass:** if rating[i] > rating[i-1], then candy[i] = candy[i-1] + 1 (an *uphill slope* gets 1,2,3,...).
* **Right-to-left pass:** if rating[i] > rating[i+1] and candy[i] <= candy[i+1], then candy[i] = candy[i+1] + 1 (a *downhill slope* seen backwards).
Each pass only raises values when a rule forces it, and a value is never raised more than needed, so the result is the minimum. The peak of a hill ends up with the larger of the two requirements.

## 3. Pseudocode
```
candy[0..n-1] = 1
for i = 1..n-1:   if rating[i] > rating[i-1]: candy[i] = candy[i-1] + 1
for i = n-2..0:   if rating[i] > rating[i+1] and candy[i] <= candy[i+1]: candy[i] = candy[i+1] + 1
return sum(candy)
```

## 4. Dry run (Test 5: 1 3 4 5 2)
After pass 1: 1 2 3 4 1. Pass 2: 5>2 but 4 > 1 already -> no change. Total = **11**.
(Test 3: 1 2 87 87 87 2 1 -> 1 2 3 1 3 2 1 = 13.)

## 5. Complexity
Three linear scans: **O(n) time, O(n) space** (an O(1)-space slope-counting version exists but the two-pass version is clearer).

## 6. Validation done by the program
* Check that the produced distribution satisfies every rule.
* Independent "relaxation" method: start with all 1s and keep fixing violations until nothing changes (this converges to the minimum solution). 6 tests + 1000 random cases (fixed seed): all agree.

## 7. Edge cases
n = 1, all ratings equal (n candies), strictly increasing / decreasing, plateaus.

## 8. Possible viva questions
* Why two passes? (Each pass enforces one neighbour rule; one pass cannot see both.)
* Why take max with the existing value in pass 2? (To keep pass 1 requirements satisfied.)
