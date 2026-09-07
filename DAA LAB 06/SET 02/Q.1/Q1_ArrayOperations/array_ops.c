/* ============================================================================
   DAA LAB-6 (PDF Set) : Question 1
   1-D Array Operations and their worst-case complexities
   ============================================================================
   Given an unsorted array of n integers, this program implements and times:
     (i)    Finding the maximum element                 -> O(n)
     (ii)   Finding first and second largest elements    -> O(n)
     (iii)  Finding the mean                             -> O(n)
     (iv)   Finding the median                            -> O(n log n)  (sort based)
     (v)    Finding the standard deviation                -> O(n)
     (vi)   Finding the mode                               -> O(n log n) (sort based) 
                                                              / O(n) with hashing
     (vii)  Removing all duplicates                        -> O(n log n) (sort based)
     (viii) Reversing the elements of the array             -> O(n)
     (ix)   Partitioning around a random pivot so that all
            elements < pivot appear AFTER all elements >= pivot -> O(n)
   ============================================================================
   Input representation: a statically-declared / dynamically-allocated 1-D
   array of type int, read from standard input, with its length n.
   ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* ---------------- (i) Maximum element : O(n) ---------------- */
int findMax(int a[], int n)
{
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > max) max = a[i];
    return max;
}

/* ---------------- (ii) First and second largest : O(n) ---------------- */
void firstAndSecondLargest(int a[], int n, int *first, int *second)
{
    *first = *second = -__INT_MAX__;
    for (int i = 0; i < n; i++)
    {
        if (a[i] > *first)
        {
            *second = *first;
            *first = a[i];
        }
        else if (a[i] > *second && a[i] != *first)
        {
            *second = a[i];
        }
    }
}

/* ---------------- (iii) Mean : O(n) ---------------- */
double findMean(int a[], int n)
{
    long long sum = 0;
    for (int i = 0; i < n; i++) sum += a[i];
    return (double)sum / n;
}

/* comparator used by qsort for median / mode / duplicate removal */
int cmp(const void *x, const void *y) { return (*(int *)x - *(int *)y); }

/* ---------------- (iv) Median : O(n log n) via sorting ---------------- */
double findMedian(int a[], int n)
{
    int *b = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) b[i] = a[i];
    qsort(b, n, sizeof(int), cmp);

    double median;
    if (n % 2 == 1)
        median = b[n / 2];
    else
        median = (b[n / 2 - 1] + b[n / 2]) / 2.0;

    free(b);
    return median;
}

/* ---------------- (v) Standard deviation : O(n) ---------------- */
double findStdDev(int a[], int n, double mean)
{
    double sumSquares = 0;
    for (int i = 0; i < n; i++)
        sumSquares += (a[i] - mean) * (a[i] - mean);
    return sqrt(sumSquares / n);
}

/* ---------------- (vi) Mode : O(n log n) via sorting ---------------- */
int findMode(int a[], int n)
{
    int *b = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) b[i] = a[i];
    qsort(b, n, sizeof(int), cmp);

    int mode = b[0], modeCount = 1, curCount = 1;
    for (int i = 1; i < n; i++)
    {
        if (b[i] == b[i - 1]) curCount++;
        else curCount = 1;

        if (curCount > modeCount)
        {
            modeCount = curCount;
            mode = b[i];
        }
    }
    free(b);
    return mode;
}

/* ---------------- (vii) Remove all duplicates : O(n log n) via sorting -----
   Returns the new length; result stored in 'out' (caller-allocated, size n) */
int removeDuplicates(int a[], int n, int out[])
{
    int *b = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) b[i] = a[i];
    qsort(b, n, sizeof(int), cmp);

    int k = 0;
    for (int i = 0; i < n; i++)
        if (i == 0 || b[i] != b[i - 1])
            out[k++] = b[i];

    free(b);
    return k;
}

/* ---------------- (viii) Reverse the array : O(n) ---------------- */
void reverseArray(int a[], int n)
{
    int i = 0, j = n - 1;
    while (i < j)
    {
        int t = a[i]; a[i] = a[j]; a[j] = t;
        i++; j--;
    }
}

/* ---------------- (ix) Partition around a random pivot : O(n) ------------
   Rearranges 'a' in place so that ALL elements < pivot appear AFTER all
   elements >= pivot (i.e. the reverse of the usual Lomuto partition order).
   Returns the boundary index: a[0..boundary-1] holds elements >= pivot,
   a[boundary..n-1] holds elements < pivot.                                  */
int partitionReversed(int a[], int n, int pivotValue)
{
    int i = -1; /* boundary of the ">= pivot" region */
    for (int j = 0; j < n; j++)
    {
        if (a[j] >= pivotValue)
        {
            i++;
            int t = a[i]; a[i] = a[j]; a[j] = t;
        }
    }
    return i + 1; /* number of elements >= pivot, i.e. the boundary index */
}

void printArray(int a[], int n)
{
    printf("[ ");
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("]\n");
}

int main(void)
{
    int n;
    printf("=========================================================\n");
    printf(" DAA LAB-6 (PDF) : Q1 - 1D Array Operations & Complexity\n");
    printf("=========================================================\n");
    printf("Enter number of elements n: ");
    scanf("%d", &n);

    int *a = malloc(n * sizeof(int));
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    printf("\nOriginal array: ");
    printArray(a, n);

    /* (i) Max */
    printf("\n(i)    Maximum element              : %d      [O(n)]\n", findMax(a, n));

    /* (ii) First & second largest */
    int first, second;
    firstAndSecondLargest(a, n, &first, &second);
    printf("(ii)   First largest, Second largest : %d, %d  [O(n)]\n", first, second);

    /* (iii) Mean */
    double mean = findMean(a, n);
    printf("(iii)  Mean                          : %.4f  [O(n)]\n", mean);

    /* (iv) Median */
    double median = findMedian(a, n);
    printf("(iv)   Median                        : %.4f  [O(n log n)]\n", median);

    /* (v) Standard deviation */
    double stddev = findStdDev(a, n, mean);
    printf("(v)    Standard deviation            : %.4f  [O(n)]\n", stddev);

    /* (vi) Mode */
    int mode = findMode(a, n);
    printf("(vi)   Mode                          : %d      [O(n log n)]\n", mode);

    /* (vii) Remove duplicates */
    int *dedup = malloc(n * sizeof(int));
    int newLen = removeDuplicates(a, n, dedup);
    printf("(vii)  After removing duplicates    : ");
    printArray(dedup, newLen);
    printf("       (length reduced from %d to %d)  [O(n log n)]\n", n, newLen);

    /* (viii) Reverse (operate on a copy so original stays available) */
    int *rev = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) rev[i] = a[i];
    reverseArray(rev, n);
    printf("(viii) Reversed array                : ");
    printArray(rev, n);
    printf("       [O(n)]\n");

    /* (ix) Partition around a pivot (choose a[0] as the 'random' pivot here) */
    int *part = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) part[i] = a[i];
    int pivotValue = part[0];
    int boundary = partitionReversed(part, n, pivotValue);
    printf("(ix)   Partition pivot = %d           : ", pivotValue);
    printArray(part, n);
    printf("       (elements >= pivot occupy indices [0..%d), elements < pivot occupy [%d..%d))  [O(n)]\n",
           boundary, boundary, n);

    free(a); free(dedup); free(rev); free(part);
    return 0;
}
