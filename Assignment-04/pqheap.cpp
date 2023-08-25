/*
 * Uses a priority queue to create a binary heap.
 */

#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQHeap class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The count of filled slots is initially zero.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // add parens to zero new memory
    _numFilled = 0;
}

/* The destructor is responsible for cleaning up any resources
 * used by this instance of the PQHeap class. The array
 * memory that was allocated for the PQHeap is deleted here.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * Doubles the size of the allocated capacity for the PQHeap class.
 */
void PQHeap::resize() {
    int newAllocated = _numAllocated * 2;
    DataPoint* newElem = new DataPoint[newAllocated]();
    for (int i = 0; i < _numFilled; i++) {
        newElem[i] = _elements[i];
    }
    _numAllocated = newAllocated;
    _elements = newElem;
}

/*
 * Pushes an element to the heap based on its priority. First
 * appends to the end of the heap then compares its value with its parent.
 * If the element is of lower priority, iteratively swaps until element
 * is of higher priority.
 */
void PQHeap::enqueue(DataPoint elem) {
    if (_numAllocated == _numFilled) {
        resize();
    }

    _elements[_numFilled] = elem;
    _numFilled++;

    int childIndex = _numFilled - 1;
    int parentIndex = getParentIndex(childIndex);

    while ((withinBounds(parentIndex) && _elements[parentIndex].priority > _elements[childIndex].priority)) {
        swapElements(parentIndex, childIndex);
        childIndex = parentIndex;
        parentIndex = getParentIndex(childIndex);
    }
}

/*
 * Returns the element with the highest priority.
 */
DataPoint PQHeap::peek() const {
    if (_numFilled > 0) {
        return _elements[0];
    }
    else {
        error("Empty heap");
    }
}

/*
 * Removes the element of highest priority. Swaps the highest priority with the last
 * element in the heap. Returns and deletes that value then iteratively swaps the
 * top of the heap with its highest minimum child.
 */
DataPoint PQHeap::dequeue() {
    if (_numFilled >= 0) {
        DataPoint res = _elements[0]; // pop from top of heap
        _elements[0] = _elements[_numFilled - 1]; // move last elem to top of heap
        _numFilled--; // delete last elem
        int parentIndex = 0;
        int minChildIndex = getMinChild(parentIndex);
        while (withinBounds(minChildIndex) && _elements[minChildIndex].priority < _elements[parentIndex].priority) {
            swapElements(minChildIndex, parentIndex);
            parentIndex = minChildIndex;
            minChildIndex = getMinChild(parentIndex);
        }
        return res;
    }
    return {"", 0};
}

/*
 * Returns true if the heap is empty, otherwise false.
 */
bool PQHeap::isEmpty() const {
    return _numFilled == 0;
}

/*
 * Returns the number of elements currently in our heap.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * Prints out the heap in array format.
 */
void PQHeap::print() {
    for (int i = 0; i < _numFilled; i++) {
        cout << _elements[i] << " ";
    }
}

/*
 * Resets the number of elements in the heap to 0.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * Displays information about debugging.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    for (int i = 0; i < size(); i++) {
        int leftIndex = getLeftChildIndex(i);
        int rightIndex = getRightChildIndex(i);
        if (withinBounds(leftIndex) && _elements[i].priority > _elements[leftIndex].priority) {
            printDebugInfo("validateInternalState");
            error("PQArray has elements out of order at index " + integerToString(i));
        }
        if (withinBounds(rightIndex) && _elements[i].priority > _elements[rightIndex].priority) {
            printDebugInfo("validateInternalState");
            error("PQArray has elements out of order at index " + integerToString(i));
        }
    }
}

/*
 * Checks if the index is within the bounds of the heap.
 */
bool PQHeap::withinBounds(int index) const {
    return (0 <= index) && (index < _numFilled);
}

/*
 * Calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    return (child - 1) / 2;
}

/*
 * Calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    return (2 * parent) + 1;
}

/*
 * Calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    return (2 * parent) + 2;
}

/*
 * Returns the highest priority of two children.
 */
int PQHeap::getMinChild(int parentIndex) {
    int leftIndex = getLeftChildIndex(parentIndex);
    int rightIndex = getRightChildIndex(parentIndex);
    if (withinBounds(leftIndex) && _elements[leftIndex].priority <= _elements[rightIndex].priority) {
        return leftIndex;
    }
    else if (withinBounds(rightIndex) && _elements[rightIndex].priority < _elements[leftIndex].priority) {
        return rightIndex;
    }
    return -1;
}

/*
 * Swaps array elements.
 */
void PQHeap::swapElements(int indexA, int indexB) {
    DataPoint temp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = temp;
}


/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("Peek at elements") {
    PQHeap pq;
    Vector<DataPoint> input = {
        {"C", 3}, {"E", 10}, {"A", 13}, {"B", 2}, {"D", 9} };
    for (DataPoint dp : input) {
        pq.enqueue(dp);
    }
    EXPECT_EQUAL(pq.peek(), {"B", 2});
    EXPECT_EQUAL(pq.size(), 5);

    pq.enqueue({"Z", 7});
    pq.enqueue({"O", 1});

    EXPECT_EQUAL(pq.peek(), {"O", 1});
    EXPECT_EQUAL(pq.size(), 7);
}

STUDENT_TEST("check empty and size") {
    PQHeap pq;
    EXPECT_EQUAL(pq.isEmpty(), true);
    Vector<DataPoint> input = {
                               {"C", 3}, {"E", 10}, {"A", 13}, {"B", 2}, {"D", 9}, {"F", 8}, {"G", 15} };
    for (DataPoint dp : input) {
        pq.enqueue(dp);
    }
    EXPECT_EQUAL(pq.size(), 7);
    EXPECT_EQUAL(pq.isEmpty(), false);

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
    EXPECT_EQUAL(pq.isEmpty(), true);
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("Enqueue and dequeue ex1") {
    PQHeap pq;
    Vector<DataPoint> input = {
        {"a",29}, {"b",12}, {"c", 9}, {"d", 3}, {"e", 19}
    };

    for (DataPoint dp : input) {
        pq.enqueue(dp);
    }

    pq.validateInternalState();
    EXPECT_EQUAL(pq.dequeue(), {"d", 3});

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

STUDENT_TEST("Enqueue and dequeue ex2") {
    PQHeap pq;
    Vector<DataPoint> input = {
        {"A", 3}, {"C", 5}, {"E", 7}, {"B", 4}, {"D", 6},
    };

    for (DataPoint dp : input) {
        pq.enqueue(dp);
    }
    pq.validateInternalState();

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
