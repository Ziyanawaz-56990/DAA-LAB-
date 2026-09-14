/*
 * Q6 - The Best Time to Be Alive
 * -------------------------------------------------------------------
 * Given (birth, death) years for n scientists, find the year(s) in
 * which the most scientists were alive simultaneously. If A dies the
 * same year B is born, A's death is treated as happening first.
 *
 * Sweep-line over +1 (birth) / -1 (death) events, sorted by year with
 * deaths breaking ties before births.
 *
 * Compile : gcc -O2 -Wall -o solution solution.c
 * Run     : ./solution        (prompts for n, then each birth/death pair)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year;
    int type;   /* -1 = death (processed first on ties), +1 = birth */
} Event;

int cmpEvent(const void *a, const void *b) {
    const Event *e1 = (const Event *)a;
    const Event *e2 = (const Event *)b;
    if (e1->year != e2->year) return e1->year - e2->year;
    return e1->type - e2->type;   /* -1 (death) sorts before +1 (birth) */
}

int main(void) {
    int n;
    printf("=== The Best Time to Be Alive ===\n");
    printf("Enter number of scientists: ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input.\n"); return 1; }

    Event *events = malloc(sizeof(Event) * 2 * n);
    printf("Enter birth and death year for each scientist:\n");
    for (int i = 0; i < n; i++) {
        int b, d;
        printf("  Scientist %d (birth death): ", i + 1);
        if (scanf("%d %d", &b, &d) != 2 || b > d) {
            printf("Invalid input.\n");
            free(events);
            return 1;
        }
        events[2 * i].year = b;     events[2 * i].type = +1;
        events[2 * i + 1].year = d; events[2 * i + 1].type = -1;
    }

    qsort(events, 2 * n, sizeof(Event), cmpEvent);

    int alive = 0, best = 0;
    int *bestYears = malloc(sizeof(int) * 2 * n);
    int bestCount = 0;

    int i = 0;
    while (i < 2 * n) {
        int y = events[i].year;
        while (i < 2 * n && events[i].year == y) {
            alive += events[i].type;
            i++;
        }
        if (alive > best) {
            best = alive;
            bestCount = 0;
            bestYears[bestCount++] = y;
        } else if (alive == best && best > 0) {
            bestYears[bestCount++] = y;
        }
    }

    printf("\nMaximum number of scientists alive simultaneously: %d\n", best);
    printf("Achieved starting in year(s): ");
    for (int k = 0; k < bestCount; k++) {
        printf("%d", bestYears[k]);
        printf(k == bestCount - 1 ? "\n" : ", ");
    }

    free(events);
    free(bestYears);
    return 0;
}
