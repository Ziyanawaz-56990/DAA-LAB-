#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int n;
    int fairHeads = 0;
    int biasedHeads = 0;

    double biasedProbability = 0.75;

    printf("Enter number of tosses: ");
    scanf("%d", &n);

    srand((unsigned int)time(NULL));

    for (int i = 0; i < n; i++)
    {
        double r = (double)rand() / RAND_MAX;

        if (r < 0.5)
            fairHeads++;
    }

    for (int i = 0; i < n; i++)
    {
        double r = (double)rand() / RAND_MAX;

        if (r < biasedProbability)
            biasedHeads++;
    }

    printf("\nFair Coin:\n");
    printf("Heads = %d\n", fairHeads);
    printf("Probability of Head = %.4f\n",
           (double)fairHeads / n);

    printf("\nBiased Coin:\n");
    printf("Heads = %d\n", biasedHeads);
    printf("Probability of Head = %.4f\n",
           (double)biasedHeads / n);

    return 0;
}