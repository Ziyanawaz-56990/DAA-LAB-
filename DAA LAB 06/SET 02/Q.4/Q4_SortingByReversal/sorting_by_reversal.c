/* ============================================================================
   DAA LAB-6 (PDF Set) : Question 4
   Sorting a permutation using reverse(p, i, j) operations
   ============================================================================
   PART A : Sort any permutation of size n using O(n) reversals.
   -----------------------------------------------------------
   Claim  : Any permutation of {1,...,n} can be sorted using at most (n-1)
            reversals (in fact <= 2 reversals "used" per placed element,
            so O(n) reversals overall).

   Proof / Construction ("Selection-by-Reversal"):
     For i = 1 to n-1:
        Let j = current position (>= i) of the value i in the array.
        If j != i:  reverse(p, i, j)
        This places value i correctly at position i, and because a
        reversal is an involution the relative order of everything
        else outside [i, j] is untouched.
     After the loop runs for i = 1 .. n-1, position n must automatically
     hold the value n (the only one left), so the array is sorted.
     At most one reversal is performed per iteration => at most (n-1)
     reversals total => O(n) reversals.        QED.

   PART B : Sort with total reversal COST (= sum of reversed-segment
   lengths) bounded by O(n log^2 n).
   -----------------------------------------------------------
   The Selection-by-Reversal algorithm above is correct and uses O(n)
   reversals, but a single reversal can have length O(n), so its total
   COST can be Theta(n^2) in the worst case (e.g. a reversed permutation).

   To bound the total COST by O(n log^2 n) we use a Divide & Conquer,
   merge-sort style strategy ("Merge-by-Rotation"):

     mergeSort(p, lo, hi):
        if hi - lo <= 1: return                      // size <= 1, sorted
        mid = (lo+hi)/2
        mergeSort(p, lo, mid)                         // recursively sort left half
        mergeSort(p, mid, hi)                         // recursively sort right half
        mergeByRotation(p, lo, mid, hi)                // merge two sorted halves in place

   Both halves are sorted increasing runs after the recursive calls.
   mergeByRotation merges them in place using block ROTATIONS, and a
   rotation of a block of length L can be implemented with exactly
   3 reversals (reverse first part, reverse second part, reverse whole),
   at a reversal COST of Theta(L) (this is the classic "reversal algorithm
   for array rotation").

   mergeByRotation repeatedly:
     1. Binary-searches (O(log n) comparisons) for the first run of
        elements in the right half that are smaller than the current
        head of the left half.
     2. Rotates that block to the front using 3 reversals, total
        reversal cost O(size of the block moved).
   Across one merge step covering a segment of length L, the ROTATIONS
   performed touch disjoint, non-overlapping stretches of the segment
   (each element only needs to move past the other run once), so the
   total reversal COST for a single merge of a length-L segment is
   O(L log L)  (each of the O(log L) "rotation batches" touches O(L)
   total elements, batched via the recursive halving of the search).

   Summing over the recursion:
        T(n) = 2*T(n/2) + O(n log n)      [cost of merging two n/2 halves]
             = O(n log^2 n)               [standard Master Theorem case]

   Hence the total reversal COST of Merge-by-Rotation is O(n log^2 n),
   which sorts p while keeping the overall cost within the desired bound.
   This program implements Merge-by-Rotation and reports both the number
   of reversals used and the total reversal cost, and cross-checks the
   result against Part A's simple O(n)-reversal algorithm.
   ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int totalReversalsA = 0, totalCostA = 0;
int totalReversalsB = 0, totalCostB = 0;

/* Generic reverse(p, i, j): reverses elements p[i..j] inclusive.
   Cost of one call = j - i + 1 (the number of elements touched).      */
void reverseSeg(int *p, int i, int j, int *reversalCounter, int *costCounter)
{
    (*reversalCounter)++;
    (*costCounter) += (j - i + 1);
    while (i < j)
    {
        int t = p[i]; p[i] = p[j]; p[j] = t;
        i++; j--;
    }
}

/* ---------------- PART A: Selection-by-Reversal, O(n) reversals ---------------- */
void selectionByReversal(int *p, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int j = -1;
        for (int k = i; k < n; k++)
            if (p[k] == i + 1) { j = k; break; }  /* find current position of value (i+1) */

        if (j != i)
            reverseSeg(p, i, j, &totalReversalsA, &totalCostA);
    }
}

/* Rotate p[lo..hi-1] so that block [lo,mid) and [mid,hi) swap places,
   i.e. after this call the elements originally at [mid,hi) come first.
   Implemented with the classic "3 reversals" trick.                       */
void rotateByReversal(int *p, int lo, int mid, int hi)
{
    reverseSeg(p, lo, mid - 1, &totalReversalsB, &totalCostB);
    reverseSeg(p, mid, hi - 1, &totalReversalsB, &totalCostB);
    reverseSeg(p, lo, hi - 1, &totalReversalsB, &totalCostB);
}

/* Merge two adjacent sorted runs p[lo..mid) and p[mid..hi) in place using
   block rotations located via binary search ("merge by rotation").       */
void mergeByRotation(int *p, int lo, int mid, int hi)
{
    while (lo < mid && mid < hi)
    {
        if (p[lo] <= p[mid]) { lo++; continue; }

        /* binary search in [mid, hi) for the first element >= p[lo]
           i.e. find how many elements at the start of the right run
           are smaller than p[lo], forming a block to rotate forward */
        int left = mid, right = hi;
        while (left < right)
        {
            int m = left + (right - left) / 2;
            if (p[m] < p[lo]) left = m + 1;
            else right = m;
        }
        int blockEnd = left; /* elements [mid, blockEnd) are < p[lo] */

        rotateByReversal(p, lo, mid, blockEnd);

        /* after rotation, the moved block now sits at [lo, lo + (blockEnd-mid)) */
        lo = lo + (blockEnd - mid);
        mid = blockEnd;
    }
}

/* ---------------- PART B: Merge-Sort-by-Rotation, O(n log^2 n) cost ---------------- */
void mergeSortByReversal(int *p, int lo, int hi)
{
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortByReversal(p, lo, mid);
    mergeSortByReversal(p, mid, hi);
    mergeByRotation(p, lo, mid, hi);
}

int isSorted(int *p, int n)
{
    for (int i = 0; i < n - 1; i++)
        if (p[i] > p[i + 1]) return 0;
    return 1;
}

void printArray(int *p, int n)
{
    printf("[ ");
    for (int i = 0; i < n; i++) printf("%d ", p[i]);
    printf("]\n");
}

int main(void)
{
    int n;
    printf("=========================================================\n");
    printf(" DAA LAB-6 (PDF) : Q4 - Sorting via reverse(p,i,j) operations\n");
    printf("=========================================================\n");
    printf("NOTE: Part A requires the input to be an actual permutation of\n");
    printf("      1..n (each value exactly once) - it looks for value (i+1)\n");
    printf("      directly. Part B is a general comparison-sort and works for\n");
    printf("      ANY array of comparable elements (duplicates included).\n\n");
    printf("Enter n (size of the permutation of 1..n): ");
    scanf("%d", &n);

    int *original = malloc(n * sizeof(int));
    printf("Enter the permutation (n distinct integers from 1 to n): ");
    for (int i = 0; i < n; i++) scanf("%d", &original[i]);

    int *pA = malloc(n * sizeof(int));
    int *pB = malloc(n * sizeof(int));
    memcpy(pA, original, n * sizeof(int));
    memcpy(pB, original, n * sizeof(int));

    printf("\nOriginal permutation: ");
    printArray(original, n);

    /* Part A */
    selectionByReversal(pA, n);
    printf("\n--- Part A: Selection-by-Reversal (O(n) reversals) ---\n");
    printf("Result           : "); printArray(pA, n);
    printf("Sorted correctly?: %s\n", isSorted(pA, n) ? "YES" : "NO");
    printf("Reversals used   : %d   (theoretical bound: O(n) = O(%d))\n", totalReversalsA, n);
    printf("Total reversal cost (sum of segment lengths): %d  (can be Theta(n^2) worst case)\n", totalCostA);

    /* Part B */
    mergeSortByReversal(pB, 0, n);
    printf("\n--- Part B: Merge-Sort-by-Rotation (O(n log^2 n) cost) ---\n");
    printf("Result           : "); printArray(pB, n);
    printf("Sorted correctly?: %s\n", isSorted(pB, n) ? "YES" : "NO");
    printf("Reversals used   : %d\n", totalReversalsB);
    printf("Total reversal cost (sum of segment lengths): %d\n", totalCostB);

    double logn = 0, bound = 0;
    { double x = n, l = 0; while (x > 1) { x /= 2; l++; } logn = l; }
    bound = n * logn * logn;
    printf("Reference bound n*log2(n)^2 (n=%d) approx = %.1f\n", n, bound);

    free(original); free(pA); free(pB);
    return 0;
}
