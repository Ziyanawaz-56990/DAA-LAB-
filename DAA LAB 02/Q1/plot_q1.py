import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from collections import defaultdict

rows = list(csv.DictReader(open("q1_operation_growth.csv")))
structures = sorted(set(r["structure"] for r in rows))
ops_to_plot = ["Search", "Insert", "Delete"]

for op in ops_to_plot:
    fig, ax = plt.subplots(figsize=(7, 5))
    for s in structures:
        xs = sorted(set(int(r["n"]) for r in rows if r["structure"] == s and r["operation"] == op))
        ys = [int([r for r in rows if r["structure"] == s and r["operation"] == op and int(r["n"]) == x][0]["worst_case_count"]) for x in xs]
        ax.plot(xs, ys, marker="o", label=s)
    ax.set_xlabel("n (number of elements)")
    ax.set_ylabel("Worst-case operation count")
    ax.set_title(f"{op}: measured worst-case growth by representation")
    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    fig.savefig(f"plots/q1_{op.lower()}_growth.png", dpi=150)
    fig.savefig(f"plots/q1_{op.lower()}_growth.svg")
    plt.close(fig)

# combined figure: all seven operations for two contrasting structures
fig, axes = plt.subplots(1, 2, figsize=(12, 5))
for ax, s in zip(axes, ["Unsorted Array", "Doubly Linked Sorted"]):
    all_ops = sorted(set(r["operation"] for r in rows))
    for op in all_ops:
        xs = sorted(set(int(r["n"]) for r in rows if r["structure"] == s and r["operation"] == op))
        ys = [int([r for r in rows if r["structure"] == s and r["operation"] == op and int(r["n"]) == x][0]["worst_case_count"]) for x in xs]
        ax.plot(xs, ys, marker=".", label=op)
    ax.set_title(s)
    ax.set_xlabel("n")
    ax.set_ylabel("Worst-case operation count")
    ax.legend(fontsize=7)
    ax.grid(True, alpha=0.3)
fig.tight_layout()
fig.savefig("plots/q1_all_operations_comparison.png", dpi=150)
fig.savefig("plots/q1_all_operations_comparison.svg")
plt.close(fig)

print("Q1 plots written.")
