import csv
import math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

rows = list(csv.DictReader(open("q2_merge_sort.csv")))
n = [int(r["n"]) for r in rows]
c2 = [int(r["two_way_comparisons"]) for r in rows]
c3 = [int(r["three_way_comparisons"]) for r in rows]
w2 = [int(r["two_way_writes"]) for r in rows]
w3 = [int(r["three_way_writes"]) for r in rows]

# comparisons
fig, ax = plt.subplots(figsize=(7, 5))
ax.plot(n, c2, marker="o", label="2-way merge sort (comparisons)")
ax.plot(n, c3, marker="s", label="3-way merge sort (comparisons)")
ax.set_xlabel("n"); ax.set_ylabel("Comparisons"); ax.set_title("Merge Sort: comparisons vs n")
ax.legend(); ax.grid(True, alpha=0.3)
fig.tight_layout(); fig.savefig("plots/q2_comparisons.png", dpi=150); fig.savefig("plots/q2_comparisons.svg")
plt.close(fig)

# writes
fig, ax = plt.subplots(figsize=(7, 5))
ax.plot(n, w2, marker="o", label="2-way merge sort (writes)")
ax.plot(n, w3, marker="s", label="3-way merge sort (writes)")
ax.set_xlabel("n"); ax.set_ylabel("Array writes"); ax.set_title("Merge Sort: writes vs n")
ax.legend(); ax.grid(True, alpha=0.3)
fig.tight_layout(); fig.savefig("plots/q2_writes.png", dpi=150); fig.savefig("plots/q2_writes.svg")
plt.close(fig)

# log-log growth check against n*log(n)
fig, ax = plt.subplots(figsize=(7, 5))
ax.plot(n, c2, marker="o", label="2-way comparisons (measured)")
ax.plot(n, c3, marker="s", label="3-way comparisons (measured)")
nlogn = [x * math.log2(x) for x in n]
scale2 = c2[-1] / nlogn[-1]
ax.plot(n, [scale2 * v for v in nlogn], "--", color="gray", label="reference: n log2(n)")
ax.set_xscale("log"); ax.set_yscale("log")
ax.set_xlabel("n (log scale)"); ax.set_ylabel("Comparisons (log scale)")
ax.set_title("Merge Sort: log-log growth vs n log n reference")
ax.legend(); ax.grid(True, which="both", alpha=0.3)
fig.tight_layout(); fig.savefig("plots/q2_loglog.png", dpi=150); fig.savefig("plots/q2_loglog.svg")
plt.close(fig)

print("Q2 plots written.")
