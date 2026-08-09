# Q2 — Fair vs Biased Coin

## Objective

Use simulation in C to demonstrate that the probability of obtaining HEAD from a fair coin approaches `0.5`, and then compare this behaviour with coins having different biases.

## Experimental Design

The experiment contains two parts.

### Fair-coin convergence

A fair coin with `P(HEAD) = 0.50` is simulated at increasing numbers of tosses:

`10, 100, 1,000, 10,000, 100,000, 1,000,000`

At each checkpoint, the program records the number of heads, tails, observed probability, and absolute error from `0.50`.

### Fair vs biased comparison

Five programmed HEAD probabilities are compared:

`30%, 50%, 60%, 70%, 90%`

Each coin is tossed `100,000` times. The observed frequency is compared with the expected probability.

## Method

Each toss is generated using a uniform pseudo-random value. A HEAD is recorded whenever that value falls below the chosen probability. The counters are accumulated throughout the experiment, so the observed probability is obtained directly from the running number of heads.

A fixed seed is used for the committed experimental run so that the CSV data and the accompanying plots correspond to a reproducible dataset.

## Observations

- With only a few tosses, the observed fair-coin probability can deviate noticeably from `0.5`.
- As the number of trials increases, random fluctuations become smaller and the observed probability stabilises around `0.5`.
- A biased coin does not converge to `0.5`; it converges to its own programmed probability.
- Increasing the sample size improves the reliability of an empirical probability estimate.

## Complexity

For `N` tosses, the simulation performs one constant amount of work per toss, giving **Theta(N)** time. Only counters and fixed-size experiment data are maintained, so the auxiliary space is **Theta(1)** with respect to the number of tosses.

## Learning Outcome

This experiment connects probability theory with computation. It demonstrates the practical effect of the **Law of Large Numbers**: repeated independent trials make the observed frequency approach the underlying probability. It also shows why a short random experiment should not be treated as an exact representation of the theoretical probability.

## Files

- `q2_coin_simulation.c` — simulation program.
- `q2_fair_convergence.csv` — fair-coin convergence measurements.
- `q2_bias_comparison.csv` — observed vs expected probabilities for the five biases.
- `plots/` — convergence and comparison graphs in PNG and SVG formats.
- `sample_output.txt` — recorded output.

## Conclusion

The simulation experimentally verifies that a fair coin approaches a HEAD probability of `0.5` as the number of tosses grows. The biased-coin comparison further shows that repeated trials converge toward the probability built into the experiment rather than toward a universal value.
