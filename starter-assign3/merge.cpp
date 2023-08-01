/*
 * Sort multiple sorted sequences into one larger sequence.
 * Test for non-sorted sequences, perform an iterative binary merge, then
 * merge multiway sequences.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "SimpleTest.h"
using namespace std;

bool checkSorted(Queue<int> q) {
    int prev, curr;
    if (!q.isEmpty()) {
        prev = q.peek();
    }

    while (!q.isEmpty()) {
        curr = q.dequeue();
        if (curr < prev)
            return false;
        prev = curr;
    }
    return true;
}
/*
 * Merge two individually sorted queues into one larger queue.
 * Raise an error for unsorted queues.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    if (!checkSorted(a) || !checkSorted(b))
        error("queue not sorted");

    // add smallest element of both queues
    while (!a.isEmpty() && !b.isEmpty()) {
        if (a.peek() <= b.peek()) {
            result.enqueue(a.dequeue());
        }
        else if (b.peek() < a.peek()) {
            result.enqueue(b.dequeue());
        }
    }

    // add remaining elements
    while (!a.isEmpty()) {
        result.enqueue(a.dequeue());
    }
    while (!b.isEmpty()) {
        result.enqueue(b.dequeue());
    }

    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * Merge k sorted sequences into one sorted output. Recursively
 * divide the vector in half until it reaches a size of 2, then call
 * the naiveMultiMerge function.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result1, result2;
    Vector<Queue<int>> firstHalf, secondHalf, res;

    if (all.size() <= 2)
        return naiveMultiMerge(all);

    else {
        // split vec in half
        firstHalf = all.subList(0, all.size()/2);
        secondHalf = all.subList(all.size()/2);
        result1 = recMultiMerge(firstHalf);
        result2 = recMultiMerge(secondHalf);
        res.add(result1);
        res.add(result2);
        return naiveMultiMerge(res);
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                             };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

STUDENT_TEST("check sorted") {
    Queue<int> q = {1, 2, 3, 4};
    EXPECT_EQUAL(checkSorted(q), true);

    Queue<int> q2 = {1, -2, 3, 4};
    EXPECT_EQUAL(checkSorted(q2), false);

    Queue<int> q3 = {1, 2, 2, 3, 4};
    EXPECT_EQUAL(checkSorted(q3), true);
}

STUDENT_TEST("binary merge") {
    Queue<int> a = {1, 2, 2, 5};
    Queue<int> b = {3, 3};
    EXPECT_EQUAL(binaryMerge(a, b), {1, 2, 2, 3, 3, 5});

    Queue<int> a2 = {};
    Queue<int> b2 = {3, 3, 8, 10};
    EXPECT_EQUAL(binaryMerge(a2, b2), {3, 3, 8, 10});

    Queue<int> a3 = {4, 9, 2}; // not sorted
    Queue<int> b3 = {1, 2};
    EXPECT_ERROR(binaryMerge(a3, b3));
}

STUDENT_TEST("binaryMerge time operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));

//    int n2 = 2000000;
//    Queue<int> a2 = createSequence(n2);
//    Queue<int> b2 = createSequence(n2);
//    TIME_OPERATION(a2.size() + b2.size(), binaryMerge(a2, b2));

//    int n3 = 4000000;
//    Queue<int> a3 = createSequence(n3);
//    Queue<int> b3 = createSequence(n3);
//    TIME_OPERATION(a3.size() + b3.size(), binaryMerge(a3, b3));

//    int n4 = 16000000;
//    Queue<int> a4 = createSequence(n4);
//    Queue<int> b4 = createSequence(n4);
//    TIME_OPERATION(a4.size() + b4.size(), binaryMerge(a4, b4));
}

STUDENT_TEST("naive merge") {
    Vector<Queue<int>> all = {{},{},{}};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);

    Vector<Queue<int>> all2 = {};
    Queue<int> expected2 = {};
    EXPECT_EQUAL(naiveMultiMerge(all2), expected2);
}

STUDENT_TEST("naiveMerge time operation") {
    int n = 10000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));

//    int n2 = 40000;
//    int k2 = n2/10;
//    Queue<int> input2 = createSequence(n2);
//    Vector<Queue<int>> all2(k2);
//    distribute(input2, all2);
//    TIME_OPERATION(input2.size(), naiveMultiMerge(all2));

//    int n3 = 80000;
//    int k3 = n3/10;
//    Queue<int> input3 = createSequence(n3);
//    Vector<Queue<int>> all3(k3);
//    distribute(input3, all3);
//    TIME_OPERATION(input3.size(), naiveMultiMerge(all3));

//    int n4 = 100000;
//    int k4 = n4/10;
//    Queue<int> input4 = createSequence(n4);
//    Vector<Queue<int>> all4(k4);
//    distribute(input4, all4);
//    TIME_OPERATION(input4.size(), naiveMultiMerge(all4));
}

STUDENT_TEST("recMultiMerge time operation") {
    int n = 100000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));

//    int n2 = 200000;
//    int k2 = n2/10;
//    Queue<int> input2 = createSequence(n2);
//    Vector<Queue<int>> all2(k2);
//    distribute(input2, all2);
//    TIME_OPERATION(input2.size(), recMultiMerge(all2));

//    int n3 = 400000;
//    int k3 = n3/10;
//    Queue<int> input3 = createSequence(n3);
//    Vector<Queue<int>> all3(k3);
//    distribute(input3, all3);
//    TIME_OPERATION(input3.size(), recMultiMerge(all3));

//    int n4 = 800000;
//    int k4 = n4/10;
//    Queue<int> input4 = createSequence(n4);
//    Vector<Queue<int>> all4(k4);
//    distribute(input4, all4);
//    TIME_OPERATION(input4.size(), recMultiMerge(all4));
}
