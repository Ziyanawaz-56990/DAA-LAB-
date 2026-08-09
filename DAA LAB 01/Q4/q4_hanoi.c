/*
 * DAA Lab-01 | Q4 - Towers of Hanoi
 * Counts the total number of moves required for n discs.
 */

#include <stdio.h>

static unsigned long long move_count;

static void hanoi(int n, char source, char auxiliary, char destination) {
    if (n == 0) return;

    hanoi(n - 1, source, destination, auxiliary);
    ++move_count;
    hanoi(n - 1, auxiliary, source, destination);
}

int main(void) {
    FILE *fp = fopen("q4_hanoi.csv", "w");
    if (!fp) {
        perror("q4_hanoi.csv");
        return 1;
    }

    fprintf(fp, "n,moves\n");

    for (int n = 1; n <= 20; ++n) {
        move_count = 0;
        hanoi(n, 'A', 'B', 'C');
        fprintf(fp, "%d,%llu\n", n, move_count);
        printf("n=%d -> moves=%llu\n", n, move_count);
    }

    fclose(fp);
    printf("CSV written to q4_hanoi.csv\n");
    return 0;
}
