# Research notes - Greedy Superstring Conjecture (updated 6 Oct 2026)

See `Q10_full_explanation.md` for the full from-scratch explanation. Summary of the research status:

* **Conjecture (Tarhio-Ukkonen 1988):** greedy (merge max-overlap pair) is a 2-approximation for Shortest Common Superstring.
* **Upper bounds on greedy:** 4 (Blum et al. 1994), 3.5 (Kaplan-Shafrir 2005), about 3.396 (Englert-Matsakis-Vesely 2023), and at most 3 per Chukhin-Kulikov-Mihajlin-Smal (ECCC TR26-157, 2026; as cited in Shibata's paper).
* **Lower bounds on greedy:** 2 (classic), 2 already for strings of length 6 (Chukhin et al., arXiv 2608.20018), and **9/4 (Shibata, arXiv 2609.01365, 1 Sept 2026)**, with ratio at least (9k+2)/(4k+4) for all-length-k instances, even k >= 10.
* **Special cases:** conjecture true for strings of length at most 4 (Kulikov et al. 2015); rho_3 = 9/5 (Chukhin et al. 2026).
* **Verification status:** Shibata's paper is a preprint (not peer reviewed at time of writing; AI-assisted discovery disclosed by the author). Our script `verify_shibata_construction.py` reproduces the construction for k = 10, 12, 14 and several t, checking at each merge that greedy's choice is legal; the ratio exceeds 2 (e.g. 2.0643 for k=10, t=30). This supports the construction but does not replace the paper's proof or peer review.
* **Gap now:** the true greedy ratio lies between 9/4 and 3 (3 is the claimed upper bound).

## What our C program can and cannot do
It computes exact optima only for tiny n (bitmask DP), so it cannot see these large constructed instances; the random search finds ratios around 1.3-1.4 on tiny binary inputs, which is consistent with greedy being good on small cases.
