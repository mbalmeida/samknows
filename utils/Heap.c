#include "Heap.h"

#include <stdlib.h>
//#include <string.h>

void SwapElements(double* a, double* b)
{
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

int HeapInit(Heap* heap, unsigned nElements, int (*less)(double a, double b))
{
    if (heap)
    {
        heap->data = (double*)malloc(sizeof(double) * nElements);
        heap->size = 0;
        heap->allocSize = nElements;
        heap->less = less;
    }

    return heap->data ? 0 : 1;
}

void HeapFree(Heap* h)
{
    if (h)
        free(h->data);
}

int HeapPush(Heap* h, double val)
{
    if (h->size >= h->allocSize - 1)
        return 1;

    // Add to the end and swap elements until we find the right place for it
    int idx = h->size;
    h->data[h->size++] = val;
    while (1)
    {
        int parent = (idx - 1) / 2 ;
        if ((idx == parent) || !(h->less(val, h->data[parent])))
            break;
        SwapElements(&h->data[parent], &h->data[idx]);
        idx = parent;
    }

    return 0;
}

int HeapPop(Heap* h, double* val)
{
    // Empty heap
    if (!h->size)
        return 1;

    *val = h->data[0];
    h->data[0] = h->data[h->size - 1];
    h->size--;
    int idx = 0; // Start from root
    int n = h->size;
    while (1)
    {
        // Check left child
        int lChild = 2 * idx + 1;
        if (lChild >= n)
            break;
        int selectedIndex = lChild;
        // Check right child
        int rightChild = selectedIndex + 1;
        if ((rightChild < n) && h->less(h->data[rightChild], h->data[selectedIndex]))
            selectedIndex = rightChild;

        if (!h->less(h->data[selectedIndex], h->data[idx]))
            break;

        SwapElements(&h->data[idx], &h->data[selectedIndex]);
        idx = selectedIndex;
    }

    return 0;
}
