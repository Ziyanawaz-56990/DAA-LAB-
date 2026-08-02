#include <stdio.h>

int findPartitionPoint(int A[], int n)
{
    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (A[mid] == 0)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return low;
}

int main()
{
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int A[n];

    printf("Enter elements (0s followed by 1s):\n");

    for (int i = 0; i < n; i++)
        scanf("%d", &A[i]);

    int partition = findPartitionPoint(A, n);

    printf("Partition point = %d\n", partition);

    if (partition == 0)
        printf("All elements are 1.\n");
    else if (partition == n)
        printf("All elements are 0.\n");
    else
        printf("Transition occurs between index %d and %d.\n",
               partition - 1, partition);

    return 0;
}