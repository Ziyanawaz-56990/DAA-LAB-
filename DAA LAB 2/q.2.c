#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 100000
#define STEP 5000

/* ---------- Normal Merge Sort ---------- */

void merge(int a[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    int i, j, k;

    for (i = 0; i < n1; i++)
        L[i] = a[left + i];

    for (j = 0; j < n2; j++)
        R[j] = a[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            a[k++] = L[i++];
        else
            a[k++] = R[j++];
    }

    while (i < n1)
        a[k++] = L[i++];

    while (j < n2)
        a[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int a[], int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);

    merge(a, left, mid, right);
}

/* ---------- Three-Way Merge ---------- */

void threeWayMerge(
    int a[],
    int left,
    int mid1,
    int mid2,
    int right)
{
    int n1 = mid1 - left + 1;
    int n2 = mid2 - mid1;
    int n3 = right - mid2;

    int *A = (int *)malloc(n1 * sizeof(int));
    int *B = (int *)malloc(n2 * sizeof(int));
    int *C = (int *)malloc(n3 * sizeof(int));

    int i, j, k, p;

    for (i = 0; i < n1; i++)
        A[i] = a[left + i];

    for (i = 0; i < n2; i++)
        B[i] = a[mid1 + 1 + i];

    for (i = 0; i < n3; i++)
        C[i] = a[mid2 + 1 + i];

    i = j = k = 0;
    p = left;

    while (i < n1 || j < n2 || k < n3)
    {
        int minValue;

        if (i < n1)
            minValue = A[i];
        else if (j < n2)
            minValue = B[j];
        else
            minValue = C[k];

        if (j < n2 && B[j] < minValue)
            minValue = B[j];

        if (k < n3 && C[k] < minValue)
            minValue = C[k];

        if (i < n1 && A[i] == minValue)
            a[p++] = A[i++];

        else if (j < n2 && B[j] == minValue)
            a[p++] = B[j++];

        else if (k < n3 && C[k] == minValue)
            a[p++] = C[k++];
    }

    free(A);
    free(B);
    free(C);
}

/* ---------- Modified Three-Way Merge Sort ---------- */

void threeWayMergeSort(int a[], int left, int right)
{
    if (left >= right)
        return;

    int length = right - left + 1;

    int third = length / 3;

    int mid1 = left + third - 1;
    int mid2 = left + 2 * third - 1;

    /*
       Handles cases where n is not perfectly
       divisible by 3.
    */

    if (third == 0)
    {
        if (left < right)
        {
            if (a[left] > a[right])
            {
                int temp = a[left];
                a[left] = a[right];
                a[right] = temp;
            }
        }

        return;
    }

    threeWayMergeSort(a, left, mid1);

    threeWayMergeSort(a, mid1 + 1, mid2);

    threeWayMergeSort(a, mid2 + 1, right);

    threeWayMerge(a, left, mid1, mid2, right);
}

/* ---------- Generate Data ---------- */

void generateArray(int a[], int n)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = rand() % 1000000;
}

/* ---------- Main ---------- */

int main()
{
    FILE *fp;

    int n;
    int *a;
    int *b;

    clock_t start, end;

    double mergeTime;
    double modifiedTime;

    srand((unsigned int)time(NULL));

    fp = fopen("q2_results.csv", "w");

    if (fp == NULL)
    {
        printf("Error creating output file.\n");
        return 1;
    }

    fprintf(fp,
            "n,Merge_Sort,Modified_Three_Way_Merge_Sort\n");

    printf("Q2: Merge Sort vs Modified Merge Sort\n");
    printf("=====================================\n");

    for (n = STEP; n <= MAX_N; n += STEP)
    {
        a = (int *)malloc(n * sizeof(int));
        b = (int *)malloc(n * sizeof(int));

        generateArray(a, n);

        for (int i = 0; i < n; i++)
            b[i] = a[i];

        /* Normal Merge Sort */

        start = clock();

        mergeSort(a, 0, n - 1);

        end = clock();

        mergeTime =
            (double)(end - start) / CLOCKS_PER_SEC;

        /* Modified Three-Way Merge Sort */

        start = clock();

        threeWayMergeSort(b, 0, n - 1);

        end = clock();

        modifiedTime =
            (double)(end - start) / CLOCKS_PER_SEC;

        fprintf(fp, "%d,%lf,%lf\n",
                n, mergeTime, modifiedTime);

        printf("n = %d completed\n", n);

        free(a);
        free(b);
    }

    fclose(fp);

    printf("\nResults saved in q2_results.csv\n");

    printf("\nWorst-case complexities:\n");
    printf("Normal Merge Sort: O(n log n)\n");
    printf("Modified 3-way Merge Sort: O(n log_3 n)\n");

    return 0;
}