# Q1 - Parity-Constrained Shortest Path via 0-1 BFS

## Key observation
Every weight is 0 or 1, so `W(P) = k1(P)`. A path therefore matters only through
two numbers: its weight `W` and whether `W` is even or odd.

```
Cost(P) = W + C   if W is odd
Cost(P) = W       if W is even
```

For a fixed parity, a smaller `W` is always better. So for each vertex we only
need the **smallest even `W`** and the **smallest odd `W`**.

## Input representation
**Adjacency list** (stored as arrays `head/nxt/to/wt`). The graph is sparse in
general, and the algorithm needs to walk the out-edges of each vertex once.

## State graph
Make two copies of every vertex: `(v, 0)` = "reached with an even number of
weight-1 edges", `(v, 1)` = "odd". Each original edge `u -> v` of weight `w`
becomes two edges:

| original edge | state edge | cost |
|---------------|------------|------|
| `w = 0` | `(u,p) -> (v,p)`   | 0 |
| `w = 1` | `(u,p) -> (v,1-p)` | 1 |

Now the problem is an ordinary shortest-path problem with edge costs 0 or 1,
which **0-1 BFS** solves with a deque.

## 0-1 BFS
- Cost-0 edge: push the new state at the **front** of the deque.
- Cost-1 edge: push the new state at the **back** of the deque.
- The deque always stays sorted by distance (front = smallest), exactly like
  Dijkstra but without a heap.

## Algorithm
```
dist[all states] = INF;  dist[(S,0)] = 0;  deque = [(S,0)]
while deque not empty:
    s = pop front;  if s already processed: continue;  mark processed
    for each edge (u -> v, w) leaving s = (u,p):
        t = (v, p XOR w)
        if dist[s] + w < dist[t]:
            dist[t] = dist[s] + w
            push t to front if w == 0 else to back
for each vertex v:
    answer[v] = min( dist[(v,0)],  dist[(v,1)] + C )
    if both are INF: UNREACHABLE
```

## Why it is correct
- Any S -> v path ends in exactly one state `(v,p)`, and the state graph keeps
  `p` equal to the parity of the number of weight-1 edges used.
- 0-1 BFS gives the true shortest distance to every state.
- For each parity, the smallest `W` gives the smallest `Cost`, so
  `min(dist[v][0], dist[v][1] + C)` is the minimum over all paths.
- The empty path gives `Cost(S) = 0`.
- Paths are allowed to revisit vertices (walks). This matters: going around a
  cycle can change the parity (see sample 1, vertex 4).

## Complexity
- States: `2V`. State edges: `2E`.
- Each state is processed once and scans its out-edges once.
- **Time: O(V + E)**  (deque push/pop are O(1)).
- **Space: O(V + E)**  (adjacency list + `2V` distances + deque of size O(E)).

## Sample 1 (see `input_1.txt`)
Vertex 4 costs 2: `1 -3(w0)-> 3 -4(w1)-> 4 -5(w0)-> 5 -6(w1)-> 6 -4(w0)-> 4`
uses two weight-1 edges: `W = 2`, even, so `Cost = 2`. The direct route
`1 -> 3 -> 4` has `W = 1` (odd) and costs `1 + 5 = 6`.
