#ifndef HEAP_H
#define HEAP_H

/**
 *
 * Very simple data structure for keeping a list of values so that a running median can be calculated easily.
 * A comparison function must be passed on to the init function so we can have reuse code and implement a min and
 * max heaps.
 */

struct Heap_
{
    double* data;
    int (*less)(double a, double b);

    unsigned size;
    unsigned allocSize;
};

typedef struct Heap_ Heap;

/**
 * Initialise a heap.
 *
 * @param heap Pointer to the heap data structure
 * @param nElements Number of elements
 * @return 0 on Success; 1 otherwise
 */
int HeapInit(Heap* heap, unsigned nElements, int (*less)(double a, double b));

/**
 * Deletes the internal data of a heap. It must always be called
 * if heap was initialised.
 * @param heap Pointer to the heap data structure
 */
void HeapFree(Heap* heap);


/**
 * Returns the number of elements present in the Heap.

 * @param Pointer to the heap data structure
 * @return Number of elements in the heap (0 if empty).
 */
unsigned HeapSize(Heap* heap);

/**
 * Pushes new element onto the heap and forces heapify operation.
 * If heap is full then no push operation if performed.

 * @param heap
 * @param val
 */
int HeapPush(Heap* heap, double val);

/**
 * Pop the top of the heap (max value for a max-heap and a min value for a min-heap)
 * @param heap
 * @param val
 * @return 0 on success, 1 on failure
 */
int HeapPop(Heap* heap, double* val);

#endif //HEAP_H

