/*
 * Program that detects whether bracketing characters are balanced.
 * First removes any characters that are not parenthesis, braces, nor brackets
 * then verifies if the operation is correctly balanced. Not balanced for missing
 * open parenthesis, a closing parenthesis comes before open, or incorrectly nested chars.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)

using namespace std;

// Takes in a string and modifies it so that only parenthesis, brackets,
// and braces are left in the string.
string operatorsFrom(string str) {
    if (str.empty())
        return "";
    else if (str[0] == '(' || str[0] == ')' || str[0] == '{' ||
             str[0] == '}' || str[0] == '[' || str[0] == ']') {
        return str[0] + operatorsFrom(str.substr(1));
    }
    else {
        return operatorsFrom(str.substr(1));
    }
}

/*
 * Recursively checks if a string of bracketing characters is valid.
 * Removes a verified set of brackets, removes it from the string, then
 * calls upon the same function with the new string.
 */
bool operatorsAreMatched(string ops) {

    vector<string> paren = {"()", "{}", "[]"};

    if (ops.empty()) {
        return true;
    }
    if (ops.size() == 1) {
        return false;
    }

    for (int i = 0; i < ops.size(); ++i) {
        string curr;
        curr += ops[i];
        curr += ops[i+1];

        for (string s : paren) {
            if (curr == s) {
                ops.replace(i, 2, "");
                return operatorsAreMatched(ops);
            }
        }
    }

    return false;
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}



STUDENT_TEST("operatorsFrom testing") {
    EXPECT_EQUAL(operatorsFrom("ar{(22)}"), "{()}");
    EXPECT_EQUAL(operatorsFrom(""), "");
    EXPECT_EQUAL(operatorsFrom("[[82b)}{"), "[[)}{");
}

STUDENT_TEST("operatorsAreMatched testing") {
    EXPECT(operatorsAreMatched("([]){}"));
    EXPECT(operatorsAreMatched("[{}()]"));

    EXPECT(!operatorsAreMatched("([]){"));
    EXPECT(!operatorsAreMatched("][()"));
}
