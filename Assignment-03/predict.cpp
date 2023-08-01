/*
 * Provide a set of word suggestions upon a user input of
 * numbers typed through a keypad.
 */
#include <iostream>
#include "prototypes.h"
#include "lexicon.h"
#include "set.h"
#include "SimpleTest.h"
#include "strlib.h"
using namespace std;

// keypad is a program-wide constant that stores the Map from integer to
// its associated set of possible letters
static const Map<int,Set<char>> keypad = {
    {2, {'a','b','c'} }, {3, {'d','e','f'} }, {4, {'g','h','i'} },
    {5, {'j','k','l'} }, {6, {'m','n','o'} }, {7, {'p','q','r','s'} },
    {8, {'t','u','v'} }, {9, {'w','x','y','z'} } };

void helpPredict(string digits, string curr, Set<string>& suggestions, Lexicon& lex) {
    if (curr.length() == digits.length() && lex.contains(toLowerCase(curr))) {
        suggestions.add(toLowerCase(curr));
        return;
    }

    if (!lex.containsPrefix(curr)) {
        return;
    }
    Set<char> s = keypad[digits[curr.size()] - 48];
    for (char letter : s) {
        helpPredict(digits, curr + letter, suggestions, lex);
    }
}

/*
 * Given digit string, map each character to the associated keypad, comparing
 * valid words to a lexicon and adding to a set of suggested words.
 */
void predict(string digits, Set<string>& suggestions, Lexicon& lex)
{
    helpPredict(digits, "", suggestions, lex);
}


/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Predict intended words for digit sequence 6263, example from writeup") {
    string digits = "6263";
    Set<string> expected = {"name", "mane", "oboe"};
    Set<string> suggestions;

    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);
}

STUDENT_TEST("Testing predict function") {
    string digits = "283";
    Set<string> expected = {"ate", "ave", "bud", "cud", "cue"};
    Set<string> suggestions;
    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);

    string digits2 = "427746377";
    Set<string> expected2 = {"happiness", "harshness"};
    Set<string> suggestions2;
    predict(digits2, suggestions2, sharedLexicon());
    EXPECT_EQUAL(suggestions2, expected2);
}
