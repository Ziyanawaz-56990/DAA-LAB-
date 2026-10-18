#!/usr/bin/env python3
"""
Randomised stress test for DAA Lab-10.

For every question, a brute-force (obviously-correct, slow) reference is run
on thousands of small random inputs and compared with the C program.

Usage:  python3 tests/stress_test.py [iterations]      (run from Lab-10 root)
Needs : gcc, python3
"""
import random, re, subprocess, sys, os
from collections import deque

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(ROOT, "tests", "bin")
os.makedirs(BIN, exist_ok=True)
DIRS = {1: "Q1_Online_Bipartiteness", 2: "Q2_Dynamic_Islands_Grid",
        3: "Q3_Max_Weight_Component_Size_K", 4: "Q4_Reverse_Time_DSU",
        5: "Q5_Lex_Smallest_Permutation"}

def build():
    for q, d in DIRS.items():
        subprocess.check_call(["gcc", "-O2", "-Wall", "-Wextra", "-fsanitize=address,undefined",
                               "-o", f"{BIN}/q{q}", f"{ROOT}/{d}/solution.c"])

def run(q, text):
    p = subprocess.run([f"{BIN}/q{q}"], input=text, capture_output=True, text=True, timeout=10)
    assert p.returncode == 0, f"Q{q} crashed:\n{p.stderr}\ninput:\n{text}"
    return p.stdout

def components(n, adj, alive=None):
    seen = [False] * (n + 1); comps = []
    for s in range(1, n + 1):
        if seen[s] or (alive is not None and not alive[s]): continue
        seen[s] = True; q = deque([s]); c = []
        while q:
            x = q.popleft(); c.append(x)
            for y in adj[x]:
                if not seen[y] and (alive is None or alive[y]):
                    seen[y] = True; q.append(y)
        comps.append(c)
    return comps

# ---------------- Q1 ----------------
def bipartite(n, adj):
    col = [-1] * (n + 1)
    for s in range(1, n + 1):
        if col[s] != -1: continue
        col[s] = 0; q = deque([s])
        while q:
            x = q.popleft()
            for y in adj[x]:
                if col[y] == -1: col[y] = col[x] ^ 1; q.append(y)
                elif col[y] == col[x]: return False
    return True

def test1():
    n = random.randint(1, 9); m = random.randint(0, 20)
    edges = [(random.randint(1, n), random.randint(1, n)) for _ in range(m)]
    inp = f"{n} {m}\n" + "".join(f"{u} {v}\n" for u, v in edges)
    out = run(1, inp)
    got = re.findall(r"-> (ACCEPTED|REJECTED)", out)
    adj = [[] for _ in range(n + 1)]; exp = []
    for u, v in edges:
        adj[u].append(v); adj[v].append(u)
        if bipartite(n, adj): exp.append("ACCEPTED")
        else: exp.append("REJECTED"); adj[u].pop(); adj[v].pop()
    assert got == exp, (inp, out, exp)

# ---------------- Q2 ----------------
def test2():
    R = random.randint(1, 5); C = random.randint(1, 5); K = random.randint(0, 20)
    ops = [(random.randint(0, R - 1), random.randint(0, C - 1)) for _ in range(K)]
    inp = f"{R} {C} {K}\n" + "".join(f"{r} {c}\n" for r, c in ops)
    got = list(map(int, re.findall(r"islands = (\d+)", run(2, inp))))
    land = set(); exp = []
    for r, c in ops:
        land.add((r, c)); seen = set(); cnt = 0
        for cell in land:
            if cell in seen: continue
            cnt += 1; st = [cell]; seen.add(cell)
            while st:
                x, y = st.pop()
                for d in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    nb = (x + d[0], y + d[1])
                    if nb in land and nb not in seen: seen.add(nb); st.append(nb)
        exp.append(cnt)
    assert got == exp, (inp, got, exp)

# ---------------- Q3 ----------------
def test3():
    n = random.randint(1, 8); k = random.randint(0, n + 1); m = random.randint(0, 12)
    w = [random.randint(-5, 20) for _ in range(n)]
    edges = [(random.randint(1, n), random.randint(1, n), random.randint(1, 9)) for _ in range(m)]
    inp = f"{n} {k} {m}\n{' '.join(map(str, w))}\n" + "".join(f"{u} {v} {x}\n" for u, v, x in edges)
    got = re.findall(r"max = (-?\d+|NONE)", run(3, inp))
    adj = [[] for _ in range(n + 1)]
    def best():
        b = None
        for c in components(n, adj):
            if len(c) <= k:
                s = sum(w[x - 1] for x in c)
                b = s if b is None or s > b else b
        return "NONE" if b is None else str(b)
    exp = [best()]
    for u, v, _ in edges:
        adj[u].append(v); adj[v].append(u); exp.append(best())
    assert got == exp, (inp, got, exp)

# ---------------- Q4 ----------------
def test4():
    n = random.randint(1, 8); m = random.randint(0, 12)
    w = [random.randint(0, 20) for _ in range(n)]
    edges = [(random.randint(1, n), random.randint(1, n)) for _ in range(m)]
    P = random.sample(range(1, n + 1), n)
    inp = f"{n} {m}\n{' '.join(map(str, w))}\n" + "".join(f"{u} {v}\n" for u, v in edges) + " ".join(map(str, P)) + "\n"
    got = list(map(int, re.findall(r"sum = (\d+)", run(4, inp))))
    adj = [[] for _ in range(n + 1)]
    for u, v in edges: adj[u].append(v); adj[v].append(u)
    alive = [True] * (n + 1); exp = []
    for p in P:
        alive[p] = False
        cs = components(n, adj, alive)
        exp.append(max((sum(w[x - 1] for x in c) for c in cs), default=0))
    assert got == exp, (inp, got, exp)

# ---------------- Q5 ----------------
def test5():
    n = random.randint(1, 9); m = random.randint(0, 10)
    A = random.sample(range(1, n + 1), n)
    edges = [(random.randint(1, n), random.randint(1, n)) for _ in range(m)]
    inp = f"{n} {m}\n{' '.join(map(str, A))}\n" + "".join(f"{u} {v}\n" for u, v in edges)
    got = list(map(int, run(5, inp).split(":")[1].split()))
    adj = [[] for _ in range(n + 1)]
    for u, v in edges: adj[u].append(v); adj[v].append(u)
    res = A[:]
    for c in components(n, adj):
        pos = sorted(c); vals = sorted(A[p - 1] for p in c)
        for p, v in zip(pos, vals): res[p - 1] = v
    assert got == res, (inp, got, res)

if __name__ == "__main__":
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 2000
    build(); random.seed(2026)
    for q, t in enumerate([test1, test2, test3, test4, test5], 1):
        for _ in range(iters): t()
        print(f"Q{q}: {iters} random tests passed  ({DIRS[q]})")
    print("ALL TESTS PASSED")
