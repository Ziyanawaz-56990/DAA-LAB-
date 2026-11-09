# Q3 - Bidirectional BFS on a Word Graph

## Graph model
Vertices are words; two words are adjacent if they differ in exactly one
letter. The number of edges can be huge (up to about `N^2`), so the graph is
**never stored**.

## Input representation
- Dictionary words stored in an array.
- A **hash table** (open addressing, FNV-1a hash) maps a word to its index, so
  "does this word exist in D?" takes O(L).
- Neighbours of a word are generated on the fly: replace each of the `L`
  positions by each of the 25 other letters and look the result up.
  That is at most `25 * L` candidates per word.

## Why bidirectional?
Ordinary BFS from `A` explores about `b^k` words (`b` = branching factor,
`k` = answer). Two searches meeting in the middle explore about `2 * b^(k/2)`,
which is far smaller.

## Algorithm
```
if B not in D: print -1
if A == B: print 0
add A to the word table if it is not already there
front[0] = {A}, dist0[A] = 0       // search from A
front[1] = {B}, dist1[B] = 0       // search from B
while both frontiers are non-empty:
    pick side with the SMALLER frontier (other = the opposite side)
    for each word u in that frontier:
        for each position i, for each letter c != u[i]:
            v = u with u[i] = c
            if v not in D or v already seen by this side: continue
            if v already seen by the other side:
                return dist_side[u] + 1 + dist_other[v]
            dist_side[v] = dist_side[u] + 1;  add v to the next frontier
    replace the frontier of this side by the next frontier
print -1
```

## Why the first meeting is optimal
Before a level is expanded, the two searches have not met, so the true distance
exceeds `df + db` (the current depths). Expanding one level can only create a
path of length at most `df + db + 1`. Hence the first meeting found has exactly
the shortest length. (Full levels are expanded, so no depth is skipped.)

`A` need not be in `D` (it is added as the start). `B` must be in `D`,
otherwise the answer is `-1`. The answer `k` counts **mutations** (edges), so
`hit -> hot -> dot -> dog -> cog` gives `k = 4`.

## Complexity
Let `N` = dictionary size, `L` = word length.
- Each word is expanded at most once per side.
- Expanding one word: `25 * L` candidates, each a hash lookup costing `O(L)`.
- **Time: O(N * L^2 * 26)**, i.e. about `26 * N * L^2` = `2.6 * 10^8` basic
  steps in the very worst case (`N = 10^5`, `L = 10`); the bidirectional
  search usually touches far fewer words.
- **Space: O(N * L)** for the words, `O(N)` for hash table, distances, frontiers.

## Samples
- `input_1.txt`: `hit -> cog` gives `4`.
- `input_2.txt`: `cog` is not in the dictionary, so the answer is `-1`.
