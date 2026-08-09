#include <stdio.h>

unsigned long long moves;

void towerOfHanoi(int n, char source, char auxiliary, char destination)
{
    if (n == 0)
        return;

    towerOfHanoi(n - 1, source, destination, auxiliary);

    moves++;

    towerOfHanoi(n - 1, auxiliary, source, destination);
}

int main()
{
    int n;

    printf("Enter number of discs: ");
    scanf("%d", &n);

    moves = 0;

    towerOfHanoi(n, 'A', 'B', 'C');

    printf("Total number of moves = %llu\n", moves);

    FILE *fp = fopen("hanoi_moves.csv", "w");

    if (fp != NULL)
    {
        fprintf(fp, "Discs,Moves\n");

        for (int i = 1; i <= n; i++)
        {
            moves = 0;
            towerOfHanoi(i, 'A', 'B', 'C');

            fprintf(fp, "%d,%llu\n", i, moves);
        }

        fclose(fp);
    }

    return 0;
}