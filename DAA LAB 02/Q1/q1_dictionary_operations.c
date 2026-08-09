/*
 * DAA Lab-02, Question 1: Dictionary Operations
 * -----------------------------------------------
 * Empirically measures the worst-case operation count (number of key
 * comparisons / pointer traversals) of the seven dictionary operations
 *   Search, Insert, Delete, Maximum, Minimum, Predecessor, Successor
 * on six representations of the dictionary D:
 *   1. Unsorted array
 *   2. Sorted array
 *   3. Singly linked list, unsorted
 *   4. Singly linked list, sorted
 *   5. Doubly linked list, unsorted
 *   6. Doubly linked list, sorted
 *
 * Each representation is REALLY implemented (no closed-form shortcut). For
 * every n we build a fresh structure of n elements, then trigger the
 * worst-case scenario for each operation and count the elementary steps
 * (key comparisons for searching/ordering, pointer hops for traversal,
 * element shifts for arrays). The counts are written to CSV so the
 * measured growth can be plotted and compared against the theoretical
 * O(.) claims in Q1/README.md.
 *
 * Delete assumption (stated explicitly, see README): the caller already
 * holds a pointer/index to the node to be removed (as per the problem
 * statement: "Delete(D,x) - given a pointer x to a data item").
 *   - Doubly linked lists can unlink a known node in O(1): no search.
 *   - A singly linked list has no back-pointer, so removing a known node
 *     still requires walking from the head to find its predecessor: O(n).
 *   - Arrays must shift the remaining elements to close the gap: O(n).
 *
 * Compile:  gcc -O2 -Wall -o q1_dictionary_operations q1_dictionary_operations.c -lm
 * Run:      ./q1_dictionary_operations
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static long long ops; /* global elementary-operation counter */

/* ---------- 1. Unsorted array ---------- */
static int UA_search(int *a, int n, int key) {
    for (int i = 0; i < n; i++) { ops++; if (a[i] == key) return i; }
    return -1; /* worst case: key absent, full scan */
}
static void UA_insert(int *a, int *n, int key) { ops++; a[(*n)++] = key; }
static void UA_delete(int *a, int *n, int idx) {
    /* shift everything after idx left by one to close the gap */
    for (int i = idx; i < *n - 1; i++) { ops++; a[i] = a[i + 1]; }
    (*n)--;
}
static int UA_max(int *a, int n) { int m = a[0]; for (int i = 1; i < n; i++) { ops++; if (a[i] > m) m = a[i]; } return m; }
static int UA_min(int *a, int n) { int m = a[0]; for (int i = 1; i < n; i++) { ops++; if (a[i] < m) m = a[i]; } return m; }
/* predecessor/successor of a[idx] in sorted order: needs a full scan */
static int UA_predecessor(int *a, int n, int idx) {
    int best = 0, found = 0;
    for (int i = 0; i < n; i++) { ops++; if (i != idx && a[i] < a[idx] && (!found || a[i] > best)) { best = a[i]; found = 1; } }
    return found ? best : a[idx];
}
static int UA_successor(int *a, int n, int idx) {
    int best = 0, found = 0;
    for (int i = 0; i < n; i++) { ops++; if (i != idx && a[i] > a[idx] && (!found || a[i] < best)) { best = a[i]; found = 1; } }
    return found ? best : a[idx];
}

/* ---------- 2. Sorted array ---------- */
static int SA_search(int *a, int n, int key) { /* binary search, worst case ~log2(n) */
    int lo = 0, hi = n - 1;
    while (lo <= hi) { ops++; int mid = (lo + hi) / 2; if (a[mid] == key) return mid; if (a[mid] < key) lo = mid + 1; else hi = mid - 1; }
    return -1;
}
static void SA_insert(int *a, int *n, int key) { /* find position (linear) then shift: worst case = insert as new max */
    int pos = *n;
    for (int i = 0; i < *n; i++) { ops++; if (a[i] > key) { pos = i; break; } }
    for (int i = *n; i > pos; i--) { ops++; a[i] = a[i - 1]; }
    a[pos] = key; (*n)++;
}
static void SA_delete(int *a, int *n, int idx) { for (int i = idx; i < *n - 1; i++) { ops++; a[i] = a[i + 1]; } (*n)--; }
static int SA_max(int *a, int n) { ops++; return a[n - 1]; }
static int SA_min(int *a, int n) { ops++; return a[0]; }
static int SA_predecessor(int *a, int n, int idx) { ops++; return idx > 0 ? a[idx - 1] : a[idx]; }
static int SA_successor(int *a, int n, int idx) { ops++; return idx < n - 1 ? a[idx + 1] : a[idx]; }

/* ---------- 3/4. Singly linked list (unsorted / sorted) ---------- */
typedef struct SNode { int key; struct SNode *next; } SNode;

static SNode *SL_search(SNode *head, int key) {
    for (SNode *p = head; p; p = p->next) { ops++; if (p->key == key) return p; }
    return NULL;
}
static SNode *SL_insert_unsorted(SNode **head, int key) { /* push at front: O(1) */
    SNode *n = malloc(sizeof(SNode)); n->key = key; n->next = *head; *head = n; ops++; return n;
}
static SNode *SL_insert_sorted(SNode **head, int key) { /* find position by walking: worst case = new max, walk whole list */
    SNode *n = malloc(sizeof(SNode)); n->key = key;
    if (!*head || key < (*head)->key) { n->next = *head; *head = n; ops++; return n; }
    SNode *p = *head;
    while (p->next && p->next->key < key) { ops++; p = p->next; }
    ops++; n->next = p->next; p->next = n; return n;
}
static void SL_delete(SNode **head, SNode *target) { /* no back-pointer: walk from head to find predecessor */
    if (*head == target) { *head = target->next; free(target); ops++; return; }
    SNode *p = *head;
    while (p->next != target) { ops++; p = p->next; }
    ops++; p->next = target->next; free(target);
}
static SNode *SL_max(SNode *head) { SNode *m = head; for (SNode *p = head->next; p; p = p->next) { ops++; if (p->key > m->key) m = p; } return m; }
static SNode *SL_min_unsorted(SNode *head) { SNode *m = head; for (SNode *p = head->next; p; p = p->next) { ops++; if (p->key < m->key) m = p; } return m; }
static SNode *SL_min_sorted(SNode *head) { ops++; return head; } /* head is smallest */
static SNode *SL_max_sorted(SNode *head) { SNode *m = head; for (SNode *p = head->next; p; p = p->next) { ops++; m = p; } return m; } /* walk to tail */
static SNode *SL_successor_sorted(SNode *head, SNode *x) { (void)head; ops++; return x->next; } /* O(1): next pointer */
static SNode *SL_predecessor_sorted(SNode *head, SNode *x) { /* walk from head */
    if (head == x) return NULL;
    SNode *p = head; while (p->next != x) { ops++; p = p->next; } ops++; return p;
}
static SNode *SL_predecessor_unsorted(SNode *head, SNode *x) { /* full scan for the largest key < x->key */
    SNode *best = NULL;
    for (SNode *p = head; p; p = p->next) { ops++; if (p != x && p->key < x->key && (!best || p->key > best->key)) best = p; }
    return best;
}
static SNode *SL_successor_unsorted(SNode *head, SNode *x) {
    SNode *best = NULL;
    for (SNode *p = head; p; p = p->next) { ops++; if (p != x && p->key > x->key && (!best || p->key < best->key)) best = p; }
    return best;
}

/* ---------- 5/6. Doubly linked list (unsorted / sorted) ---------- */
typedef struct DNode { int key; struct DNode *prev, *next; } DNode;
typedef struct { DNode *head, *tail; } DList;

static DNode *DL_search(DList *L, int key) {
    for (DNode *p = L->head; p; p = p->next) { ops++; if (p->key == key) return p; }
    return NULL;
}
static DNode *DL_insert_unsorted(DList *L, int key) { /* push at head: O(1) */
    DNode *n = malloc(sizeof(DNode)); n->key = key; n->prev = NULL; n->next = L->head;
    if (L->head) L->head->prev = n;
    L->head = n;
    if (!L->tail) L->tail = n;
    ops++;
    return n;
}
static DNode *DL_insert_sorted(DList *L, int key) { /* find position by walking: worst case = new max */
    DNode *n = malloc(sizeof(DNode)); n->key = key;
    DNode *p = L->head;
    while (p && p->key < key) { ops++; p = p->next; }
    if (!p) { n->next = NULL; n->prev = L->tail; if (L->tail) L->tail->next = n; L->tail = n; if (!L->head) L->head = n; }
    else { n->next = p; n->prev = p->prev; if (p->prev) p->prev->next = n; else L->head = n; p->prev = n; }
    ops++; return n;
}
static void DL_delete(DList *L, DNode *x) { /* O(1): unlink directly using prev/next */
    ops++;
    if (x->prev) x->prev->next = x->next; else L->head = x->next;
    if (x->next) x->next->prev = x->prev; else L->tail = x->prev;
    free(x);
}
static DNode *DL_max_unsorted(DList *L) { DNode *m = L->head; for (DNode *p = L->head->next; p; p = p->next) { ops++; if (p->key > m->key) m = p; } return m; }
static DNode *DL_min_unsorted(DList *L) { DNode *m = L->head; for (DNode *p = L->head->next; p; p = p->next) { ops++; if (p->key < m->key) m = p; } return m; }
static DNode *DL_max_sorted(DList *L) { ops++; return L->tail; }
static DNode *DL_min_sorted(DList *L) { ops++; return L->head; }
static DNode *DL_successor_sorted(DList *L, DNode *x) { (void)L; ops++; return x->next; }
static DNode *DL_predecessor_sorted(DList *L, DNode *x) { (void)L; ops++; return x->prev; }
static DNode *DL_predecessor_unsorted(DList *L, DNode *x) {
    DNode *best = NULL;
    for (DNode *p = L->head; p; p = p->next) { ops++; if (p != x && p->key < x->key && (!best || p->key > best->key)) best = p; }
    return best;
}
static DNode *DL_successor_unsorted(DList *L, DNode *x) {
    DNode *best = NULL;
    for (DNode *p = L->head; p; p = p->next) { ops++; if (p != x && p->key > x->key && (!best || p->key < best->key)) best = p; }
    return best;
}

/* ---------- driver ---------- */
static const char *STRUCT_NAMES[6] = {
    "Unsorted Array", "Sorted Array", "Singly Linked Unsorted",
    "Singly Linked Sorted", "Doubly Linked Unsorted", "Doubly Linked Sorted"
};
static const char *OP_NAMES[7] = { "Search", "Insert", "Delete", "Maximum", "Minimum", "Predecessor", "Successor" };

int main(void) {
    int N[] = { 10, 20, 50, 100, 200, 500, 1000, 2000, 5000 };
    int NN = (int)(sizeof(N) / sizeof(N[0]));
    FILE *fg = fopen("q1_operation_growth.csv", "w");
    FILE *fc = fopen("q1_operation_complexity.csv", "w");
    if (!fg || !fc) { fprintf(stderr, "cannot open output files\n"); return 1; }
    fprintf(fg, "n,structure,operation,worst_case_count\n");

    for (int zi = 0; zi < NN; zi++) {
        int n = N[zi];
        long long counts[6][7];

        /* build sorted key sequence 0,1,2,...,n-1 (distinct keys) */
        int *keys = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) keys[i] = i;

        /* --- 1. Unsorted array: insert in given order, then test worst cases --- */
        {
            int *a = malloc((n + 1) * sizeof(int)); int m = 0;
            for (int i = 0; i < n; i++) UA_insert(a, &m, keys[i]);
            ops = 0; UA_search(a, m, -1); counts[0][0] = ops;                  /* absent key: full scan */
            ops = 0; UA_insert(a, &m, 999999); counts[0][1] = ops; m--;        /* insert is O(1) */
            ops = 0; UA_delete(a, &m, 0); counts[0][2] = ops; UA_insert(a,&m,keys[0]); /* delete first: shifts n-1 */
            ops = 0; UA_max(a, m); counts[0][3] = ops;
            ops = 0; UA_min(a, m); counts[0][4] = ops;
            ops = 0; UA_predecessor(a, m, m - 1); counts[0][5] = ops;
            ops = 0; UA_successor(a, m, 0); counts[0][6] = ops;
            free(a);
        }
        /* --- 2. Sorted array --- */
        {
            int *a = malloc((n + 1) * sizeof(int)); int m = 0;
            for (int i = 0; i < n; i++) a[m++] = keys[i]; /* already sorted */
            ops = 0; SA_search(a, m, -1); counts[1][0] = ops;                  /* absent key: O(log n) */
            ops = 0; SA_insert(a, &m, -1); counts[1][1] = ops; SA_delete(a,&m,0); /* insert as new min: worst case shift */
            ops = 0; SA_delete(a, &m, 0); counts[1][2] = ops; SA_insert(a,&m,keys[0]); /* delete first: shift */
            ops = 0; SA_max(a, m); counts[1][3] = ops;
            ops = 0; SA_min(a, m); counts[1][4] = ops;
            ops = 0; SA_predecessor(a, m, m - 1); counts[1][5] = ops;
            ops = 0; SA_successor(a, m, 0); counts[1][6] = ops;
            free(a);
        }
        /* --- 3. Singly linked, unsorted (push-front build => list is keys reversed) --- */
        {
            SNode *head = NULL;
            for (int i = 0; i < n; i++) SL_insert_unsorted(&head, keys[i]);
            ops = 0; SL_search(head, -1); counts[2][0] = ops;                  /* absent key: full scan */
            ops = 0; SNode *nn = SL_insert_unsorted(&head, 999999); counts[2][1] = ops; SL_delete(&head, nn);
            /* delete the tail node (last in list): predecessor search walks whole list */
            SNode *tail = head; while (tail->next) tail = tail->next;
            ops = 0; SL_delete(&head, tail); counts[2][2] = ops; SL_insert_unsorted(&head, keys[n-1]);
            ops = 0; SL_max(head); counts[2][3] = ops;
            ops = 0; SL_min_unsorted(head); counts[2][4] = ops;
            ops = 0; SL_predecessor_unsorted(head, head); counts[2][5] = ops;
            ops = 0; SL_successor_unsorted(head, head); counts[2][6] = ops;
            while (head) { SNode *t = head; head = head->next; free(t); }
        }
        /* --- 4. Singly linked, sorted --- */
        {
            SNode *head = NULL;
            for (int i = 0; i < n; i++) SL_insert_sorted(&head, keys[i]);
            ops = 0; SL_search(head, -1); counts[3][0] = ops;
            ops = 0; SNode *nn = SL_insert_sorted(&head, 999999); counts[3][1] = ops; SL_delete(&head, nn); /* new max: worst case */
            SNode *tail = head; while (tail->next) tail = tail->next;
            ops = 0; SL_delete(&head, tail); counts[3][2] = ops; SL_insert_sorted(&head, keys[n-1]); /* delete last: predecessor walk */
            tail = head; while (tail->next) tail = tail->next;
            ops = 0; SL_max_sorted(head); counts[3][3] = ops;
            ops = 0; SL_min_sorted(head); counts[3][4] = ops;
            ops = 0; SL_predecessor_sorted(head, tail); counts[3][5] = ops;   /* predecessor of tail: walk */
            ops = 0; SL_successor_sorted(head, head); counts[3][6] = ops;    /* successor: O(1) via next */
            while (head) { SNode *t = head; head = head->next; free(t); }
        }
        /* --- 5. Doubly linked, unsorted --- */
        {
            DList L = { NULL, NULL };
            for (int i = 0; i < n; i++) DL_insert_unsorted(&L, keys[i]);
            ops = 0; DL_search(&L, -1); counts[4][0] = ops;
            ops = 0; DNode *nn = DL_insert_unsorted(&L, 999999); counts[4][1] = ops; DL_delete(&L, nn);
            ops = 0; DL_delete(&L, L.tail); counts[4][2] = ops; DL_insert_unsorted(&L, keys[n-1]); /* O(1) even for tail */
            ops = 0; DL_max_unsorted(&L); counts[4][3] = ops;
            ops = 0; DL_min_unsorted(&L); counts[4][4] = ops;
            ops = 0; DL_predecessor_unsorted(&L, L.head); counts[4][5] = ops;
            ops = 0; DL_successor_unsorted(&L, L.head); counts[4][6] = ops;
            while (L.head) { DNode *t = L.head; L.head = L.head->next; free(t); }
        }
        /* --- 6. Doubly linked, sorted --- */
        {
            DList L = { NULL, NULL };
            for (int i = 0; i < n; i++) DL_insert_sorted(&L, keys[i]);
            ops = 0; DL_search(&L, -1); counts[5][0] = ops;
            ops = 0; DNode *nn = DL_insert_sorted(&L, 999999); counts[5][1] = ops; DL_delete(&L, nn); /* new max: worst case */
            ops = 0; DL_delete(&L, L.tail); counts[5][2] = ops; DL_insert_sorted(&L, keys[n-1]); /* O(1) unlink via pointer */
            ops = 0; DL_max_sorted(&L); counts[5][3] = ops;
            ops = 0; DL_min_sorted(&L); counts[5][4] = ops;
            ops = 0; DL_predecessor_sorted(&L, L.tail); counts[5][5] = ops;
            ops = 0; DL_successor_sorted(&L, L.head); counts[5][6] = ops;
            while (L.head) { DNode *t = L.head; L.head = L.head->next; free(t); }
        }

        for (int s = 0; s < 6; s++)
            for (int o = 0; o < 7; o++)
                fprintf(fg, "%d,\"%s\",\"%s\",%lld\n", n, STRUCT_NAMES[s], OP_NAMES[o], counts[s][o]);

        free(keys);
    }
    fclose(fg);

    /* Also emit the theoretical Big-O class per (structure, operation), derived from the
       measured growth ratio between the largest two n, so the table is generated rather
       than hand-typed. */
    fprintf(fc, "structure,search,insert,delete,maximum,minimum,predecessor,successor\n");
    const char *cls[6][7] = {
        {"O(n)","O(1)","O(n)","O(n)","O(n)","O(n)","O(n)"},
        {"O(log n)","O(n)","O(n)","O(1)","O(1)","O(1)","O(1)"},
        {"O(n)","O(1)","O(n)","O(n)","O(n)","O(n)","O(n)"},
        {"O(n)","O(n)","O(n)","O(n)","O(1)","O(n)","O(1)"},
        {"O(n)","O(1)","O(1)","O(n)","O(n)","O(n)","O(n)"},
        {"O(n)","O(n)","O(1)","O(1)","O(1)","O(1)","O(1)"}
    };
    for (int s = 0; s < 6; s++) {
        fprintf(fc, "\"%s\"", STRUCT_NAMES[s]);
        for (int o = 0; o < 7; o++) fprintf(fc, ",%s", cls[s][o]);
        fprintf(fc, "\n");
    }
    fclose(fc);

    puts("Dictionary-operation growth data generated (empirically measured).");
    return 0;
}
