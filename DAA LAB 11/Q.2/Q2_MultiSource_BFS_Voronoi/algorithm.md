# Q2 - Multi-Source BFS for Radius-Constrained Voronoi Cells

## Key idea
Instead of running BFS from each of the `K` hubs (which would cost `K * (V+E)`),
start **one** BFS with **all hubs in the queue at distance 0**. The wave that
reaches a vertex first is the nearest hub, so the vertex is owned by it.

## Input representation
**Adjacency list** of the undirected graph (each edge stored in both
directions), plus the list of hubs in index order.

## Radius threshold R
A vertex at distance `R` is **not expanded**, so no vertex farther than `R` is
ever reached. Vertices never reached keep `owner = -1`, `dist = -1`.

## Tie-breaking (smallest hub index)
Hubs are pushed in the order `h1, h2, ..., hK`.
Claim: at every BFS level, the queue is ordered by owner index (non-decreasing).
- Level 0: true by construction.
- If it holds at level `d`, a vertex `v` at level `d+1` is first discovered by
  the earliest-queued neighbour `u` of level `d`, which has the **smallest owner
  among all level-`d` neighbours** of `v`. Those owners are exactly the hubs at
  distance `d+1` from `v` (by induction), so `v` gets the smallest index.
- Children are appended in the order of their parents, so the order is kept.

If the same vertex is listed as a hub twice, the smaller index keeps it.

## Algorithm
```
owner[*] = -1;  dist[*] = -1
for i = 1..K:  if owner[h_i] == -1: owner[h_i] = i; dist[h_i] = 0; enqueue h_i
while queue not empty:
    u = dequeue
    if dist[u] == R: continue            // radius reached
    for each neighbour v of u:
        if owner[v] == -1:
            owner[v] = owner[u];  dist[v] = dist[u] + 1;  enqueue v
print owner[v], dist[v] for all v
```

## Why it is correct
BFS discovers vertices in non-decreasing distance order, so the first claim on
`v` carries `d_min(v)`. If `d_min(v) > R`, no wave reaches it, giving `-1`.
Ties are resolved by the ordering claim above.

## Complexity
- Every vertex is enqueued at most once; every adjacency entry is scanned at
  most once.
- **Time: O(V + E)** (independent of K, apart from reading the K hubs: O(K)).
- **Space: O(V + E)**.

## Sample 1 (see `input_1.txt`)
Hubs 1 and 5, `R = 2`. Vertex 3 is at distance 2 from hub 1 and 2 from hub 5,
a tie, so hub **index 1** wins. Vertices 9 and 10 are farther than 2 from both
hubs, so they get `-1`.
