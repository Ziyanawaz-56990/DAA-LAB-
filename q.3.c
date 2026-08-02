#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void copyArray(int a[], int b[], int n)
{
    for (int i = 0; i < n; i++)
        b[i] = a[i];
}

long long bubbleSortEarly(int a[], int n)
{
    long long comparisons = 0;

    for (int i = 0; i < n - 1; i++)
    {
        int swapped = 0;

        for (int j = 0; j < n - 1 - i; j++)
        {
            comparisons++;

            if (a[j] > a[j + 1])
            {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;

                swapped = 1;
            }
        }

        if (!swapped)
            break;
    }

    return comparisons;
}

long long bubbleSortAlways(int a[], int n)
{
    long long comparisons = 0;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            comparisons++;

            if (a[j] > a[j + 1])
            {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }

    return comparisons;
}

int main()
{
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int *original = malloc(n * sizeof(int));
    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));

    srand((unsigned int)time(NULL));

    for (int i = 0; i < n; i++)
        original[i] = rand();

    copyArray(original, a, n);
    copyArray(original, b, n);

    long long early = bubbleSortEarly(a, n);
    long long always = bubbleSortAlways(b, n);

    printf("\nNumber of comparisons:\n");
    printf("Early termination version = %lld\n", early);
    printf("Always n-1 passes version = %lld\n", always);

    FILE *fp = fopen("bubble_sort_comparisons.csv", "w");

    if (fp != NULL)
    {
        fprintf(fp, "Version,Comparisons\n");
        fprintf(fp, "Early Termination,%lld\n", early);
        fprintf(fp, "Always n-1 Passes,%lld\n", always);
        fclose(fp);
    }

    free(original);
    free(a);
    free(b);

    return 0;
}