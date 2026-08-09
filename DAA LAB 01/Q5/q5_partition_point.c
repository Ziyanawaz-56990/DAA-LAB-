/*
 * DAA Lab-01 | Q5 - Find the Partition Point
 * Finds the first 1 in an array of the form 000...00111...111
 * using binary search.
 */

#include <stdio.h>

static int first_one(const int a[], int n) {
    int low = 0;
    int high = n - 1;
    int answer = n; /* n means that no 1 exists. */

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (a[mid] == 1) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return answer;
}

static void run_test(const int a[], int n) {
    int point = first_one(a, n);

    printf("Array: ");
    for (int i = 0; i < n; ++i) printf("%d", a[i]);

    if (point == n)
        printf("  -> no 1 found (all zeros)\n");
    else
        printf("  -> first 1 at index %d\n", point);
}

int main(void) {
    const int a1[] = {0,0,0,0,0,1,1,1,1,1};
    const int a2[] = {0,0,0,0,0,0,0,1,1,1};
    const int a3[] = {1,1,1,1,1};
    const int a4[] = {0,0,0,0,0};

    run_test(a1, 10);
    run_test(a2, 10);
    run_test(a3, 5);
    run_test(a4, 5);

    return 0;
}
