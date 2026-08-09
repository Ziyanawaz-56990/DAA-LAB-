#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------- Merge Two Sorted Arrays ---------- */

int *mergeArrays(
    int a[],
    int sizeA,
    int b[],
    int sizeB)
{
    int *result =
        (int *)malloc((sizeA + sizeB) * sizeof(int));

    int i = 0;
    int j = 0;
    int k = 0;

    while (i < sizeA && j < sizeB)
    {
        if (a[i] <= b[j])
            result[k++] = a[i++];
        else
            result[k++] = b[j++];
    }

    while (i < sizeA)
        result[k++] = a[i++];

    while (j < sizeB)
        result[k++] = b[j++];

    return result;
}

/* ---------- Generate Sorted Arrays ---------- */

void generateSortedArrays(
    int **arrays,
    int k,
    int n)
{
    int i, j;

    for (i = 0; i < k; i++)
    {
        for (j = 0; j < n; j++)
        {
            arrays[i][j] =
                i * n + j;
        }
    }
}

/* ================================================= */
/* METHOD 1                                       */
/* ================================================= */

int *method1(
    int **arrays,
    int k,
    int n,
    int *finalSize)
{
    int *result;
    int currentSize;

    result =
        (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        result[i] = arrays[0][i];

    currentSize = n;

    for (int i = 1; i < k; i++)
    {
        int *newResult;

        newResult =
            mergeArrays(
                result,
                currentSize,
                arrays[i],
                n);

        free(result);

        result = newResult;

        currentSize += n;
    }

    *finalSize = currentSize;

    return result;
}

/* ================================================= */
/* METHOD 2                                       */
/* ================================================= */

int *method2(
    int **arrays,
    int k,
    int n,
    int *finalSize)
{
    int currentK = k;
    int currentSize = n;

    int **currentArrays =
        (int **)malloc(
            currentK * sizeof(int *));

    for (int i = 0; i < currentK; i++)
    {
        currentArrays[i] =
            (int *)malloc(
                n * sizeof(int));

        for (int j = 0; j < n; j++)
            currentArrays[i][j] =
                arrays[i][j];
    }

    while (currentK > 1)
    {
        int newK =
            (currentK + 1) / 2;

        int **newArrays =
            (int **)malloc(
                newK * sizeof(int *));

        int index = 0;

        for (int i = 0;
             i < currentK;
             i += 2)
        {
            if (i + 1 < currentK)
            {
                newArrays[index] =
                    mergeArrays(
                        currentArrays[i],
                        currentSize,
                        currentArrays[i + 1],
                        currentSize);

                free(currentArrays[i]);
                free(currentArrays[i + 1]);

                index++;
            }
            else
            {
                newArrays[index] =
                    currentArrays[i];

                index++;
            }
        }

        free(currentArrays);

        currentArrays = newArrays;

        currentK = newK;

        currentSize *= 2;
    }

    *finalSize = currentSize;

    int *result = currentArrays[0];

    free(currentArrays);

    return result;
}

/* ---------- Print Array ---------- */

void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");
}

/* ---------- Main ---------- */

int main()
{
    int k, n;

    printf("Q3: Merging k Sorted Arrays\n");
    printf("============================\n");

    printf("Enter number of sorted arrays (k): ");
    scanf("%d", &k);

    printf("Enter number of elements in each array (n): ");
    scanf("%d", &n);

    if (k <= 0 || n <= 0)
    {
        printf("Invalid input.\n");
        return 1;
    }

    int **arrays =
        (int **)malloc(
            k * sizeof(int *));

    for (int i = 0; i < k; i++)
    {
        arrays[i] =
            (int *)malloc(
                n * sizeof(int));
    }

    generateSortedArrays(
        arrays,
        k,
        n);

    printf("\nInput sorted arrays:\n");

    for (int i = 0; i < k; i++)
    {
        printf("Array %d: ", i + 1);

        printArray(
            arrays[i],
            n);
    }

    /* ---------- Method 1 ---------- */

    clock_t start, end;

    start = clock();

    int size1;

    int *result1 =
        method1(
            arrays,
            k,
            n,
            &size1);

    end = clock();

    double time1 =
        (double)(end - start) /
        CLOCKS_PER_SEC;

    printf("\nMethod 1 Result:\n");

    printArray(
        result1,
        size1);

    printf("Method 1 Time: %lf seconds\n",
           time1);

    /* ---------- Method 2 ---------- */

    start = clock();

    int size2;

    int *result2 =
        method2(
            arrays,
            k,
            n,
            &size2);

    end = clock();

    double time2 =
        (double)(end - start) /
        CLOCKS_PER_SEC;

    printf("\nMethod 2 Result:\n");

    printArray(
        result2,
        size2);

    printf("Method 2 Time: %lf seconds\n",
           time2);

    /* ---------- Complexity ---------- */

    printf("\nWorst-case running times:\n");

    printf("Method 1: O(n * k^2)\n");
    printf("Method 2: O(n * k * log k)\n");

    /* ---------- Free Memory ---------- */

    free(result1);
    free(result2);

    for (int i = 0; i < k; i++)
        free(arrays[i]);

    free(arrays);

    return 0;
}