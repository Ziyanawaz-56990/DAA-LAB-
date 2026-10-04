/*
 * DAA Lab 08 - Q9: Collatz Conjecture (3n+1 problem) - computational experiment
 *
 *   T(n) = n / 2      if n is even
 *   T(n) = 3n + 1     if n is odd
 *
 * IMPORTANT: this program EXPERIMENTS with the Collatz map. It does NOT prove
 * the Collatz Conjecture, which is still an open problem. Checking a finite
 * range of starting values can never prove the statement for ALL positive
 * integers.
 *
 * INPUT FORMAT (standard input):
 *     n      -> a starting value, n >= 1       (Part A: single trajectory)
 *     a b    -> interval, 1 <= a <= b          (Part B: interval analysis)
 *
 * PART A prints the full trajectory of n and: total number of steps, number
 *        of even/odd steps, the maximum value reached (the "peak"), and the
 *        stopping time (steps until the value first drops below n).
 * PART B analyses every starting value in [a, b] and reports the starting
 *        value with the longest trajectory, the one with the highest peak,
 *        the average number of steps, etc.
 *
 * INTEGER OVERFLOW: values are stored in 'unsigned long long' (at least 64
 * bits, maximum ULLONG_MAX = 18446744073709551615 on common systems). Before
 * every 3n+1 we check that the result still fits. If it would not, the
 * trajectory is stopped and reported as "OVERFLOW" - we never print a
 * silently wrong answer.
 *
 * Program structure (functional decomposition):
 *   collatzNext()      - one safe application of T, with overflow check
 *   analyzeStart()     - statistics of one trajectory (no storage needed)
 *   buildTrajectory()  - stores the whole trajectory in a dynamic array
 *   printTrajectory()  - prints a stored trajectory
 *   analyzeInterval()  - runs analyzeStart() for every n in [a, b]
 *   main()             - reads input and calls the parts above
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long long ull;

#define MAX_INTERVAL_SIZE 100000000ULL  /* keep run time reasonable in Part B */

/* Statistics of one trajectory. */
typedef struct {
    ull start;          /* starting value n                                  */
    ull steps;          /* total stopping time: applications of T to reach 1 */
    ull oddSteps;       /* how many of those steps were 3n+1                 */
    ull evenSteps;      /* how many of those steps were n/2                  */
    ull peak;           /* largest value seen (including the start)          */
    ull stoppingTime;   /* steps until value < start (0 for start = 1)       */
    int overflow;       /* 1 if the computation had to stop (overflow)       */
} Stats;

/* Applies T once. Returns 1 on success and stores the result in *next.
 * Returns 0 if 3n+1 would not fit in an unsigned long long. */
int collatzNext(ull n, ull *next)
{
    if (n % 2 == 0) {
        *next = n / 2;
        return 1;
    }
    if (n > (ULLONG_MAX - 1) / 3) {      /* 3n + 1 would exceed ULLONG_MAX */
        return 0;
    }
    *next = 3 * n + 1;
    return 1;
}

/* Computes the statistics of the trajectory starting at n. */
Stats analyzeStart(ull n)
{
    Stats s;
    ull current = n, next;
    int stoppedBelow = (n == 1);          /* n = 1 has stopping time 0 */

    s.start = n; s.steps = 0; s.oddSteps = 0; s.evenSteps = 0;
    s.peak = n; s.stoppingTime = 0; s.overflow = 0;

    while (current != 1) {
        int odd = (current % 2 == 1);
        if (!collatzNext(current, &next)) {
            s.overflow = 1;
            return s;
        }
        current = next;
        s.steps++;
        if (odd) s.oddSteps++; else s.evenSteps++;
        if (current > s.peak) s.peak = current;
        if (!stoppedBelow && current < n) {
            s.stoppingTime = s.steps;
            stoppedBelow = 1;
        }
    }
    return s;
}

/* Stores the whole trajectory (start ... 1) in a dynamically allocated
 * array that grows with realloc. Returns the array and sets *length.
 * Returns NULL on overflow or if memory runs out (*overflowFlag tells which). */
ull *buildTrajectory(ull n, size_t *length, int *overflowFlag)
{
    size_t capacity = 64, count = 0;
    ull *path = (ull *)malloc(capacity * sizeof(ull));
    ull current = n, next;

    *overflowFlag = 0;
    if (path == NULL) return NULL;
    path[count++] = current;

    while (current != 1) {
        if (!collatzNext(current, &next)) {
            *overflowFlag = 1;
            *length = count;              /* keep what we have so far */
            return path;
        }
        current = next;
        if (count == capacity) {          /* grow the array */
            ull *bigger;
            capacity *= 2;
            bigger = (ull *)realloc(path, capacity * sizeof(ull));
            if (bigger == NULL) { free(path); return NULL; }
            path = bigger;
        }
        path[count++] = current;
    }
    *length = count;
    return path;
}

void printTrajectory(const ull *path, size_t length)
{
    size_t i;
    for (i = 0; i < length; i++) {
        printf("%llu", path[i]);
        if (i + 1 < length) printf(" -> ");
        if ((i + 1) % 8 == 0 && i + 1 < length) printf("\n  ");
    }
    printf("\n");
}

/* ---------- Part A ---------- */
void partA(ull n)
{
    size_t length = 0;
    int overflow;
    ull *path = buildTrajectory(n, &length, &overflow);
    Stats s = analyzeStart(n);

    printf("=== PART A: trajectory of n = %llu ===\n", n);
    if (path == NULL) {
        printf("Could not allocate memory.\n");
        return;
    }
    printf("Trajectory (%lu values):\n  ", (unsigned long)length);
    printTrajectory(path, length);

    if (s.overflow) {
        printf("OVERFLOW: the next 3n+1 value would exceed %llu, so the\n"
               "computation was stopped. No conclusion about this n is made.\n",
               ULLONG_MAX);
    } else {
        printf("Reached 1 after (total stopping time) : %llu steps\n", s.steps);
        printf("  n/2 steps (even)                    : %llu\n", s.evenSteps);
        printf("  3n+1 steps (odd)                    : %llu\n", s.oddSteps);
        printf("Maximum value reached (peak)          : %llu\n", s.peak);
        if (n == 1)
            printf("Stopping time                         : 0 (n is already 1)\n");
        else
            printf("Stopping time (first value < n)       : %llu steps\n", s.stoppingTime);
    }
    free(path);
}

/* ---------- Part B ---------- */
void analyzeInterval(ull a, ull b)
{
    ull count = b - a + 1, i;
    Stats longest, highest;
    int haveBest = 0;
    ull overflowCount = 0, completed = 0;
    unsigned long long totalSteps = 0;

    printf("=== PART B: interval [%llu, %llu] (%llu starting values) ===\n", a, b, count);
    if (count > MAX_INTERVAL_SIZE) {
        printf("Interval too large for this program (limit %llu values).\n", MAX_INTERVAL_SIZE);
        return;
    }
    if (count <= 20) {
        printf("  n    steps   odd   even   peak           stopping time\n");
    }
    longest = highest = analyzeStart(a);   /* placeholders, replaced below */

    for (i = 0; i < count; i++) {
        Stats s = analyzeStart(a + i);
        if (s.overflow) {
            overflowCount++;
            printf("  n = %llu: OVERFLOW (skipped in the statistics)\n", s.start);
            continue;
        }
        completed++;
        totalSteps += s.steps;
        if (!haveBest || s.steps > longest.steps) longest = s;
        if (!haveBest || s.peak > highest.peak) highest = s;
        haveBest = 1;
        if (count <= 20) {
            printf("  %-4llu %-7llu %-5llu %-6llu %-14llu %llu\n", s.start, s.steps,
                   s.oddSteps, s.evenSteps, s.peak, s.stoppingTime);
        }
    }

    printf("Starting values that reached 1       : %llu of %llu\n", completed, count);
    printf("Starting values stopped by overflow  : %llu\n", overflowCount);
    if (completed > 0) {
        printf("Longest trajectory                   : n = %llu with %llu steps\n",
               longest.start, longest.steps);
        printf("Highest peak value                   : n = %llu reaches %llu\n",
               highest.start, highest.peak);
        printf("Average number of steps              : %.3f\n",
               (double)totalSteps / (double)completed);
    }
}

int main(void)
{
    ull n, a, b;

    printf("Collatz experiment (this program does NOT prove the conjecture)\n");
    if (scanf("%llu", &n) != 1 || n < 1) {
        printf("Invalid input: the starting value n must be an integer >= 1.\n");
        return 1;
    }
    if (scanf("%llu %llu", &a, &b) != 2 || a < 1 || a > b) {
        printf("Invalid interval: need integers with 1 <= a <= b.\n");
        return 1;
    }
    partA(n);
    printf("\n");
    analyzeInterval(a, b);
    return 0;
}
