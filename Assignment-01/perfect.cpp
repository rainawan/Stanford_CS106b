#include "console.h"
#include <iostream>
#include <cmath>
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/*
 * The smarterSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. The square root of
 * the input is the number of times a loop iterates. Upon finding a
 * perfect square, it calculates a corresponding perfect square by
 * dividing the input by that value.
 */

long smarterSum(long n) {
    // Streamlining and more testing
    long res;
    if (n == 1)
        res = 0;
    else
        res = 1;
    long sqroot = sqrt(n);
    for (int i = 2; i <= sqroot; i++) {
        if (n % i == 0) {
            res += i;
            res += (n / i);
        }
    }
    return res;
}

/* The isPerfectSmarter function takes an input 'n' and returns
 * a bool depending on whether 'n' is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */

bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* Use Euclid's approach to find prime numbers and calculate
 * their corresponding perfect number. Returns the nth perfect number.
 */
long findNthPerfectEuclid(long n) {

    int m = 0;
    int perfect = 0;
    int nth_perf = 0;

    for (int k = 1; k < 100; ++k) {
        m = pow(2, k) - 1;

        // prime
        if (smarterSum(m) == 1) {
            perfect = pow(2, k-1) * (pow(2, k) - 1);
            nth_perf += 1;
        }
        if (nth_perf == n)
            return perfect;
    }
    return 0;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}



//STUDENT_TEST("My added cases") {
//    TIME_OPERATION(5000, findPerfects(5000));
//    TIME_OPERATION(10000, findPerfects(10000));
////    TIME_OPERATION(200000, findPerfects(200000));
////    TIME_OPERATION(400000, findPerfects(400000));
//}

//STUDENT_TEST("Confirm negative values are not perfect") {
//    EXPECT(!isPerfect(-6));
//    EXPECT(!isPerfect(-28));
//}

STUDENT_TEST("Confirm smarterSum of small inputs") {
    EXPECT_EQUAL(smarterSum(1), 0);
    EXPECT_EQUAL(smarterSum(6), 6);
    EXPECT_EQUAL(smarterSum(28), 28);
}

//STUDENT_TEST("Confirm perfect and nonperfect inputs") {
//    // Confirm Perfect
//    EXPECT(isPerfectSmarter(6));
//    EXPECT(isPerfectSmarter(28));
//    EXPECT(isPerfectSmarter(496));

//    // Confirm not perfect
//    EXPECT(!isPerfectSmarter(2));
//    EXPECT(!isPerfectSmarter(14));
//    EXPECT(!isPerfectSmarter(25));
//    EXPECT(!isPerfectSmarter(98765));

//    // Oddballs: 0 and 1
//    EXPECT(!isPerfectSmarter(0));
//    EXPECT(!isPerfectSmarter(1));
//}

//STUDENT_TEST("Time trial of findPerfectsSmarter on input size 1000") {
//    TIME_OPERATION(37500, findPerfects(37500));
//    TIME_OPERATION(75000, findPerfects(75000));
//    TIME_OPERATION(150000, findPerfects(150000));
////    TIME_OPERATION(300000, findPerfects(300000));
//}

STUDENT_TEST("Find nth perfect euclide") {
    EXPECT_EQUAL(findNthPerfectEuclid(1),6);
    EXPECT_EQUAL(findNthPerfectEuclid(2),28);
    EXPECT_EQUAL(findNthPerfectEuclid(3),496);
    EXPECT_EQUAL(findNthPerfectEuclid(4),8128);
}

/*
 * Below is a suggestion of how to use a loop to set the input sizes
 * for a sequence of time trials.
 *
 *
STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes") {

    int smallest = 1000, largest = 8000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfects(size));
    }
}

*/
