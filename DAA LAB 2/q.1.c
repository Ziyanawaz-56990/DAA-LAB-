#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 10000
#define STEP 1000

typedef struct SNode {
    int key;
    struct SNode *next;
} SNode;

typedef struct DNode {
    int key;
    struct DNode *prev;
    struct DNode *next;
} DNode;

/* ---------- Utility Functions ---------- */

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ---------- Unsorted Array ---------- */

void testUnsortedArray(int n, double *result)
{
    int *a = (int *)malloc(n * sizeof(int));
    int i, key = n - 1;
    clock_t start, end;

    for (i = 0; i < n; i++)
        a[i] = i;

    start = clock();

    /* Search */
    for (i = 0; i < n; i++)
        if (a[i] == key)
            break;

    /* Insert */
    if (n > 0)
        a[0] = key + 1;

    /* Delete */
    for (i = 0; i < n - 1; i++)
        a[i] = a[i + 1];

    /* Maximum */
    int max = a[0];
    for (i = 1; i < n - 1; i++)
        if (a[i] > max)
            max = a[i];

    /* Minimum */
    int min = a[0];
    for (i = 1; i < n - 1; i++)
        if (a[i] < min)
            min = a[i];

    /* Predecessor */
    for (i = 0; i < n - 1; i++)
        if (a[i] == key)
            break;

    /* Successor */
    for (i = 0; i < n - 1; i++)
        if (a[i] == key)
            break;

    end = clock();

    *result = (double)end / CLOCKS_PER_SEC -
              (double)start / CLOCKS_PER_SEC;

    free(a);
}

/* ---------- Sorted Array ---------- */

void testSortedArray(int n, double *result)
{
    int *a = (int *)malloc(n * sizeof(int));
    int i, key = n / 2;
    clock_t start, end;

    for (i = 0; i < n; i++)
        a[i] = i;

    start = clock();

    /* Binary Search */
    int low = 0, high = n - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        if (a[mid] == key)
            break;
        else if (a[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }

    /* Insert */
    for (i = n - 1; i > key; i--)
        a[i] = a[i - 1];

    /* Delete */
    for (i = key; i < n - 1; i++)
        a[i] = a[i + 1];

    /* Maximum */
    int max = a[n - 2];

    /* Minimum */
    int min = a[0];

    /* Predecessor */
    if (key > 0)
        i = a[key - 1];

    /* Successor */
    if (key < n - 1)
        i = a[key + 1];

    end = clock();

    *result = (double)(end - start) / CLOCKS_PER_SEC;

    free(a);
}

/* ---------- Singly Linked Unsorted ---------- */

void testSinglyUnsorted(int n, double *result)
{
    SNode *head = NULL, *temp, *newNode;
    int i, key = n / 2;
    clock_t start, end;

    for (i = 0; i < n; i++)
    {
        newNode = (SNode *)malloc(sizeof(SNode));
        newNode->key = i;
        newNode->next = head;
        head = newNode;
    }

    start = clock();

    /* Search */
    temp = head;

    while (temp != NULL)
    {
        if (temp->key == key)
            break;

        temp = temp->next;
    }

    /* Insert */
    newNode = (SNode *)malloc(sizeof(SNode));
    newNode->key = n + 1;
    newNode->next = head;
    head = newNode;

    /* Delete */
    if (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }

    /* Maximum */
    temp = head;
    int max = temp->key;

    while (temp != NULL)
    {
        if (temp->key > max)
            max = temp->key;

        temp = temp->next;
    }

    /* Minimum */
    temp = head;
    int min = temp->key;

    while (temp != NULL)
    {
        if (temp->key < min)
            min = temp->key;

        temp = temp->next;
    }

    /* Predecessor / Successor */
    temp = head;

    while (temp != NULL && temp->key != key)
        temp = temp->next;

    end = clock();

    *result = (double)(end - start) / CLOCKS_PER_SEC;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* ---------- Singly Linked Sorted ---------- */

void testSinglySorted(int n, double *result)
{
    SNode *head = NULL, *temp, *newNode;
    int i, key = n / 2;
    clock_t start, end;

    for (i = n - 1; i >= 0; i--)
    {
        newNode = (SNode *)malloc(sizeof(SNode));
        newNode->key = i;
        newNode->next = head;
        head = newNode;
    }

    start = clock();

    /* Search */
    temp = head;

    while (temp != NULL && temp->key < key)
        temp = temp->next;

    /* Insert */
    newNode = (SNode *)malloc(sizeof(SNode));
    newNode->key = key;

    temp = head;

    while (temp->next != NULL &&
           temp->next->key < key)
    {
        temp = temp->next;
    }

    newNode->next = temp->next;
    temp->next = newNode;

    /* Delete */
    if (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }

    /* Minimum */
    int min = head->key;

    /* Maximum */
    temp = head;

    while (temp->next != NULL)
        temp = temp->next;

    int max = temp->key;

    /* Predecessor / Successor */
    temp = head;

    while (temp != NULL && temp->key < key)
        temp = temp->next;

    end = clock();

    *result = (double)(end - start) / CLOCKS_PER_SEC;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* ---------- Doubly Linked Unsorted ---------- */

void testDoublyUnsorted(int n, double *result)
{
    DNode *head = NULL, *temp, *newNode;
    int i, key = n / 2;
    clock_t start, end;

    for (i = 0; i < n; i++)
    {
        newNode = (DNode *)malloc(sizeof(DNode));

        newNode->key = i;
        newNode->prev = NULL;
        newNode->next = head;

        if (head != NULL)
            head->prev = newNode;

        head = newNode;
    }

    start = clock();

    /* Search */
    temp = head;

    while (temp != NULL && temp->key != key)
        temp = temp->next;

    /* Insert */
    newNode = (DNode *)malloc(sizeof(DNode));

    newNode->key = n + 1;
    newNode->prev = NULL;
    newNode->next = head;

    if (head != NULL)
        head->prev = newNode;

    head = newNode;

    /* Delete */
    if (head != NULL)
    {
        temp = head;
        head = head->next;

        if (head != NULL)
            head->prev = NULL;

        free(temp);
    }

    /* Maximum */
    temp = head;
    int max = temp->key;

    while (temp != NULL)
    {
        if (temp->key > max)
            max = temp->key;

        temp = temp->next;
    }

    /* Minimum */
    temp = head;
    int min = temp->key;

    while (temp != NULL)
    {
        if (temp->key < min)
            min = temp->key;

        temp = temp->next;
    }

    /* Predecessor / Successor */
    temp = head;

    while (temp != NULL && temp->key != key)
        temp = temp->next;

    end = clock();

    *result = (double)(end - start) / CLOCKS_PER_SEC;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* ---------- Doubly Linked Sorted ---------- */

void testDoublySorted(int n, double *result)
{
    DNode *head = NULL, *temp, *newNode;
    int i, key = n / 2;
    clock_t start, end;

    for (i = n - 1; i >= 0; i--)
    {
        newNode = (DNode *)malloc(sizeof(DNode));

        newNode->key = i;
        newNode->prev = NULL;
        newNode->next = head;

        if (head != NULL)
            head->prev = newNode;

        head = newNode;
    }

    start = clock();

    /* Search */
    temp = head;

    while (temp != NULL && temp->key < key)
        temp = temp->next;

    /* Insert */
    newNode = (DNode *)malloc(sizeof(DNode));
    newNode->key = key;

    temp = head;

    while (temp->next != NULL &&
           temp->next->key < key)
    {
        temp = temp->next;
    }

    newNode->next = temp->next;
    newNode->prev = temp;

    if (temp->next != NULL)
        temp->next->prev = newNode;

    temp->next = newNode;

    /* Delete */
    if (head != NULL)
    {
        temp = head;
        head = head->next;

        if (head != NULL)
            head->prev = NULL;

        free(temp);
    }

    /* Minimum */
    int min = head->key;

    /* Maximum */
    temp = head;

    while (temp->next != NULL)
        temp = temp->next;

    int max = temp->key;

    /* Predecessor / Successor */
    temp = head;

    while (temp != NULL && temp->key < key)
        temp = temp->next;

    end = clock();

    *result = (double)(end - start) / CLOCKS_PER_SEC;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* ---------- Main ---------- */

int main()
{
    FILE *fp;
    int n;
    double t1, t2, t3, t4, t5, t6;

    fp = fopen("q1_results.csv", "w");

    if (fp == NULL)
    {
        printf("Error creating output file.\n");
        return 1;
    }

    fprintf(fp,
            "n,Unsorted_Array,Sorted_Array,"
            "Singly_Unsorted,Singly_Sorted,"
            "Doubly_Unsorted,Doubly_Sorted\n");

    printf("Q1: Dictionary Operations\n");
    printf("==========================\n");

    for (n = STEP; n <= MAX_N; n += STEP)
    {
        testUnsortedArray(n, &t1);
        testSortedArray(n, &t2);
        testSinglyUnsorted(n, &t3);
        testSinglySorted(n, &t4);
        testDoublyUnsorted(n, &t5);
        testDoublySorted(n, &t6);

        fprintf(fp, "%d,%lf,%lf,%lf,%lf,%lf,%lf\n",
                n, t1, t2, t3, t4, t5, t6);

        printf("n = %d completed\n", n);
    }

    fclose(fp);

    printf("\nResults saved in q1_results.csv\n");
    printf("Open the CSV file in Excel to plot the order of growth.\n");

    return 0;
}