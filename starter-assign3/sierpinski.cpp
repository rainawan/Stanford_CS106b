/*
 * Draw a self-similar Sierpinski fractal triangle.
 */
#include <iostream>    // for cout, endl
#include <cmath>
#include "recursion.h"
#include "SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * Takes in the three corner points of a triangle and the order. Returns
 * the total number of black triangles drawn, recursively.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    else {
        GPoint newOne, newTwo, newThree;
        newOne = {(one.x + two.x) / 2, (one.y + two.y) / 2};
        newTwo = {(one.x + three.x) / 2, (one.y + three.y) / 2};
        newThree = {(two.x + three.x) / 2, (two.y + three.y) / 2};
        drawSierpinskiTriangle(window, one, newOne, newTwo, order - 1);
        drawSierpinskiTriangle(window, newOne, two, newThree, order - 1);
        drawSierpinskiTriangle(window, newTwo, newThree, three, order - 1);

    }
    return pow(3, order);
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

