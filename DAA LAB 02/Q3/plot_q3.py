import csv
import math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

rows = list(csv.DictReader(open("q3_k_sorted_arrays.csv")))
ks = sorted(set(int(r["k"]) for r in rows))
ns = sorted(set(int(r["n"]) for r in rows))

# growth vs k, at largest n
n_fixed = max(ns)
sub = [r for r in rows if int(r["n"]) == n_fixed]
sub.sort(key=lambda r: int(r["k"]))
kx = [int(r["k"]) for r in sub]
m1 = [int(r["method1_comparisons"]) for r in sub]
m2 = [int(r["method2_comparisons"]) for r in sub]

fig, ax = plt.subplots(figsize=(7, 5))
ax.plot(kx, m1, marker="o", label="Method 1: sequential (comparisons)")
ax.plot(kx, m2, marker="s", label="Method 2: balanced pairwise (comparisons)")
ax.set_xlabel(f"k (n fixed at {n_fixed})"); ax.set_ylabel("Comparisons")
ax.set_title(f"k-way merge: comparisons vs k (n={n_fixed})")
ax.legend(); ax.grid(True, alpha=0.3)
fig.tight_layout(); fig.savefig("plots/q3_vs_k.png", dpi=150); fig.savefig("plots/q3_vs_k.svg")
plt.close(fig)

# growth vs n, at largest k
k_fixed = max(ks)
sub2 = [r for r in rows if int(r["k"]) == k_fixed]
sub2.sort(key=lambda r: int(r["n"]))
nx = [int(r["n"]) for r in sub2]
m1n = [int(r["method1_comparisons"]) for r in sub2]
m2n = [int(r["method2_comparisons"]) for r in sub2]

fig, ax = plt.subplots(figsize=(7, 5))
ax.plot(nx, m1n, marker="o", label="Method 1: sequential (comparisons)")
ax.plot(nx, m2n, marker="s", label="Method 2: balanced pairwise (comparisons)")
ax.set_xlabel(f"n (k fixed at {k_fixed})"); ax.set_ylabel("Comparisons")
ax.set_title(f"k-way merge: comparisons vs n (k={k_fixed})")
ax.legend(); ax.grid(True, alpha=0.3)
fig.tight_layout(); fig.savefig("plots/q3_vs_n.png", dpi=150); fig.savefig("plots/q3_vs_n.svg")
plt.close(fig)

# log-log: method1 vs k^2 reference, method2 vs k log k reference (n fixed)
fig, ax = plt.subplots(figsize=(7, 5))
ax.plot(kx, m1, marker="o", label="Method 1 (measured)")
ax.plot(kx, m2, marker="s", label="Method 2 (measured)")
k2_ref = [k * k for k in kx]
klogk_ref = [k * math.log2(k) for k in kx]
scale1 = m1[-1] / k2_ref[-1]
scale2 = m2[-1] / klogk_ref[-1]
ax.plot(kx, [scale1 * v for v in k2_ref], "--", color="gray", label="reference: k^2")
ax.plot(kx, [scale2 * v for v in klogk_ref], ":", color="black", label="reference: k log2(k)")
ax.set_xscale("log"); ax.set_yscale("log")
ax.set_xlabel("k (log scale)"); ax.set_ylabel("Comparisons (log scale)")
ax.set_title(f"k-way merge: log-log growth vs k (n={n_fixed})")
ax.legend(fontsize=8); ax.grid(True, which="both", alpha=0.3)
fig.tight_layout(); fig.savefig("plots/q3_loglog_k.png", dpi=150); fig.savefig("plots/q3_loglog_k.svg")
plt.close(fig)

print("Q3 plots written.")
