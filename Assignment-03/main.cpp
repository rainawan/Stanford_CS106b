#include "console.h"
#include "SimpleTest.h"
#include "vector.h"
#include "recursion.h"
#include "backtracking.h"
#include "prototypes.h"
#include <iostream>
using namespace std;

/*
 * You are free to edit the main in any way that works
 * for your testing/debugging purposes.
 * We will supply our own main() during grading
 */

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    cout << "All done, exiting" << endl;
    return 0;
}

// Do not remove or edit below this line. It is here to confirm that your code
// conforms to the expected function prototypes needed for grading
void confirmFunctionPrototypes() {
    string str;
    int i = 0;
    bool b;
    GPoint pt;
    GWindow win;
    drawSierpinskiTriangle(win, pt, pt, pt, i);
    Queue<int> q;
    Vector<Queue<int>> v;
    q = binaryMerge(q, q);
    q = naiveMultiMerge(v);
    q = recMultiMerge(v);
}
