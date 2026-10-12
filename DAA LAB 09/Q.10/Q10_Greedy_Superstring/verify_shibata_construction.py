"""
Independent computational check of the instance in Shibata (arXiv:2609.01365, Sept 2026).

What this script does (for small k and t):
  1. builds the instance S_{k,t} exactly as defined in the paper (Section 3.1),
  2. finds an optimal superstring by an Eulerian circuit in the de Bruijn graph (paper's Lemma 8/9)
     and checks it really contains every input string -> an UPPER bound on OPT,
     while the number of distinct strings gives the matching LOWER bound |S|+k-1,
  3. runs the greedy algorithm following the merge order described in Section 5, and at EVERY merge
     checks that the chosen pair really has the maximum overlap in the current set
     (i.e. that this run is a legal execution of greedy under some tie-breaking),
  4. compares the greedy length with the paper's formula t(9s+1)+1.

It checks specific small cases only. It is NOT a proof for all k and t.
Run: python3 verify_shibata_construction.py
"""
import sys
from collections import defaultdict

def X(n):                                    # X_n = (yx)^{n/2} (n even) or x(yx)^{(n-1)/2} (n odd)
    return "yx" * (n // 2) if n % 2 == 0 else "x" + "yx" * ((n - 1) // 2)

def csub(T, j, l):                           # CSubstr_l(T, j): 1-indexed circular substring of length l
    return "".join(T[(j - 1 + p) % len(T)] for p in range(l))

def build(k, t):
    s = k // 2
    S, parts = [], {}
    for i in range(1, t + 1):
        b = chr(0x100 + i)                   # distinct character b_i
        U = "x" + "a" + X(s - 3) + b + "x" + "a" + X(s - 3) + "y"
        V = "x" + "a" + X(s - 3) + b
        W = "x" + "a" + X(s - 1) + b
        assert (len(U), len(V), len(W)) == (k, s, s + 2)
        parts[i] = (U, V, W)
        S += [csub(U, j, k) for j in range(1, k + 1)]
        S += [csub(V, j, k) for j in range(1, s + 1)]
        S += [csub(W, j, k) for j in range(1, s + 3)]
    return S, parts

def ov(a, b):                                # longest overlap: suffix of a == prefix of b
    for L in range(min(len(a), len(b)) - 1, 0, -1):
        if a.endswith(b[:L]): return L
    return 0

def maxov(strs):
    best = 0
    for L in range(max(map(len, strs)) - 1, 0, -1):
        pref = defaultdict(set)
        for idx, x in enumerate(strs):
            if len(x) > L: pref[x[:L]].add(idx)
        for idx, x in enumerate(strs):
            if len(x) > L and any(j != idx for j in pref.get(x[-L:], ())):
                return L
    return best

def euler_superstring(S, k):                 # Hierholzer on the de Bruijn graph of order k-1
    adj = defaultdict(list)
    for x in S: adj[x[:k-1]].append(x)
    start = S[0][:k-1]; stack, circuit = [(start, None)], []
    while stack:
        v, e = stack[-1]
        if adj[v]: x = adj[v].pop(); stack.append((x[1:], x))
        else: stack.pop(); circuit.append(e)
    edges = [e for e in reversed(circuit) if e is not None]
    return start + "".join(e[-1] for e in edges)

class Pool:                                  # set of current strings, with checked merging
    def __init__(self, strs): self.s = list(strs)
    def merge(self, a, b):                   # merge strings a then b (must be present)
        o = ov(a, b); m = maxov(self.s)
        assert o == m, f"merge overlap {o} is not the maximum {m}!"
        self.s.remove(a); self.s.remove(b)
        c = a + b[o:]; self.s.append(c); return c

def run(k, t):
    s = k // 2; S, parts = build(k, t)
    assert len(set(S)) == len(S) == t * (4 * s + 2), "strings not distinct"
    opt_lb = len(S) + k - 1
    T = euler_superstring(S, k)
    assert all(x in T for x in S), "Euler string misses an input"
    opt = len(T); assert opt == opt_lb, (opt, opt_lb)
    P = Pool(S); hats = {}
    def chain(T_, start, count):             # merge CSubstr_k(T_, start), ..., left to right
        cur = csub(T_, start, k)
        for step in range(1, count):
            cur = P.merge(cur, csub(T_, (start - 1 + step) % len(T_) + 1, k))
        return cur
    for i in range(1, t + 1):
        U, V, W = parts[i]
        hats[i] = (chain(U, 4, k), chain(V, 2, s), chain(W, 6, s + 2))   # U^, V^, W^
    M = []
    for i in range(1, t + 1):
        Uh, Vh, _ = hats[i]; M.append(P.merge(Vh, Uh))                    # M_i = V^_i (+) U^_i
    cur = M[0]
    for i in range(1, t): cur = P.merge(cur, M[i])                         # M = M_1 (+) ... (+) M_t
    for i in range(1, t + 1): cur = P.merge(cur, hats[i][2])               # then W^_1 ... W^_t (overlap 0)
    assert len(P.s) == 1 and all(x in cur for x in S)
    formula = t * (9 * s + 1) + 1
    print(f"k={k:2d} t={t:2d} |S|={len(S):4d}  OPT={opt:5d}  greedy={len(cur):5d} (paper formula {formula})  ratio={len(cur)/opt:.4f}  "
          f"{'OK' if len(cur)==formula else 'FORMULA MISMATCH'}")
    return len(cur) / opt

if __name__ == "__main__":
    print("Every merge below was checked to have the MAXIMUM overlap (legal greedy step).\n")
    for k in (10, 12, 14):
        for t in (1, 2, 3, 6):
            run(k, t)
    print("\nPaper's limit for t -> infinity: (9k+2)/(4k+4):")
    for k in (10, 12, 14): print(f"  k={k}: {(9*k+2)/(4*k+4):.4f}")
