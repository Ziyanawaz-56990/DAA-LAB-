#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int hasDuplicate(int A[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (A[i] == A[j])
                return 1;
        }
    }

    return 0;
}

int main()
{
    int n;

    printf("Enter number of random numbers: ");
    scanf("%d", &n);

    int A[n];

    srand((unsigned int)time(NULL));

    printf("Random numbers:\n");

    for (int i = 0; i < n; i++)
    {
        A[i] = rand() % 100;
        printf("%d ", A[i]);
    }

    printf("\n");

    if (hasDuplicate(A, n))
        printf("Duplicate elements are present.\n");
    else
        printf("All elements are unique.\n");

    return 0;
}