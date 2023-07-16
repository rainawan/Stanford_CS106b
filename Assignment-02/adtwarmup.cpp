/*
 *  Practice debugging data structures including stacks and queues.
 */

#include "queue.h"
#include "stack.h"
#include "SimpleTest.h"
using namespace std;

/* This function correctly reverses the elements of a queue.
 */
void reverse(Queue<int>& q) {
    Stack<int> s;
    int num;

    while (!q.isEmpty()) {
        num = q.dequeue();
        s.push(num);
    }
    while (!s.isEmpty()) {
        num = s.pop();
        q.enqueue(num);
    }
}

/* This function is intended to modify a queue of characters to duplicate
 * any negative numbers.
 */
void duplicateNegatives(Queue<int>& q) {
    int size = q.size();
    for (int i = 0; i < size; i++) {
        int val = q.dequeue();
        q.enqueue(val);
        if (val < 0) {
            q.enqueue(val);   // double up on negative numbers
        }
    }
}

// This function is intended to return the sum of all values in
// the stack
// WARNING: the given code is buggy. See exercise writeup for more
// information on how to test and diagnose.
int sumStack(Stack<int> s) {
    int total = 0;
    while (!s.isEmpty()) {
        total += s.pop();
    }
    return total;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("reverse queue") {
    Queue<int> q = {1, 2, 3, 4, 5};
    Queue<int> expected = {5, 4, 3, 2, 1};

    reverse(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has no negatives") {
    Queue<int> q = {2, 10};
    Queue<int> expected = q;

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has one negative, one positive") {
    Queue<int> q = {-7, 9};
    Queue<int> expected = {-7, -7, 9};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has negatives interspersed with positives") {
    Queue<int> q = {-3, 4, -5, 10};
    Queue<int> expected = {-3, -3, 4, -5, -5, 10};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("sumStack, 3 elements") {
    Stack<int> s = {1, 8, -5};

    EXPECT_EQUAL(sumStack(s), 4);
}

PROVIDED_TEST("sumStack, empty stack") {
    Stack<int> empty;

    EXPECT_EQUAL(sumStack(empty), 0);
}

STUDENT_TEST("infinite loop") {
    Queue<int> q = {10, -2, 1, 6, 6, 6, -1, -1};
    Queue<int> expected = {10, -2, -2, 1, 6, 6, 6, -1, -1, -1, -1};

//    Queue<int> q = {10, 1, 6, 6, 6};
//    Queue<int> expected = {10, 1, 6, 6, 6};

//    Queue<int> q = {1, 2, -3};
//    Queue<int> expected = {1, 2, -3, -3};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}
