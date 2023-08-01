/*
 * A Boggle game board has a random set of characters in a grid. This program
 * traces paths of neighboring characters to find valid words. Words must be
 * 4 characters long and cannot be repeated. A 4-letter word earns 1 pt, 5-letter
 * word earns 2 pts, etc.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "SimpleTest.h"
using namespace std;

/*
 * Provided helper function to score a single word.
 */
int points(string str) {
    return max((int)str.length() - 3, 0);
}

/*
 * Uses backtracking to traverse different paths on board, updating a string with new characters.
 */
void pathsHelper(GridLocation loc, string word, Set<GridLocation> visited, Set<string>& valid, Grid<char>& board, Lexicon& lex) {
    // location out of bounds
    if (visited.contains(loc) || loc.row < 0 || loc.col < 0 || loc.row >= board.numRows() || loc.col >= board.numCols())
        return;
    // word not in dictionary
    if (!lex.containsPrefix(word))
        return;

    word += board[loc];

    // valid word with size of four or more
    if (lex.contains(toLowerCase(word)) && word.size() >= 4) {
        valid.add(toLowerCase(word));
    }

    // (1) choice to add location to path
    visited.add(loc);

    pathsHelper({loc.row + 1, loc.col}, word, visited, valid, board, lex);
    pathsHelper({loc.row - 1, loc.col}, word, visited, valid, board, lex);
    pathsHelper({loc.row, loc.col + 1}, word, visited, valid, board, lex);
    pathsHelper({loc.row, loc.col - 1}, word, visited, valid, board, lex);
    pathsHelper({loc.row + 1, loc.col + 1}, word, visited, valid, board, lex);
    pathsHelper({loc.row - 1, loc.col - 1}, word, visited, valid, board, lex);
    pathsHelper({loc.row + 1, loc.col - 1}, word, visited, valid, board, lex);
    pathsHelper({loc.row - 1, loc.col + 1}, word, visited, valid, board, lex);

    // (2) choice to remove location from path
    visited.remove(loc);
}

/*
 * Compute the total score for all words found in a boggle board.
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    int score = 0;
    Set<GridLocation> visited;
    Set<string> valid;
    for (int r = 0; r < board.numRows(); r++) {
        for (int c = 0; c < board.numCols(); c++) {
            pathsHelper({r, c}, "", visited, valid, board, lex);
        }
    }

    for (string s : valid) {
        score += points(s);
    }

    return score;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}


// STUDENT TEST CASES

STUDENT_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'E','S','_','_'},
                        {'K','A','_','_'},
                        {'_','C','_','_'},
                        {'_','_','_','_'}};
    Set<string> valid = {};
    Set<GridLocation> visited;

    for (int r = 0; r < board.numRows(); r++) {
        for (int c = 0; c < board.numCols(); c++) {
            pathsHelper({r, c}, "", visited, valid, board, sharedLexicon());
        }
    }
    EXPECT_EQUAL(valid, {"cake", "cakes", "case", "cask", "kaes", "keas", "sack", "sake"});


    Grid<char> board2 = {{'_','_','_','_'},
                         {'I','L','I','R'},
                         {'C','E','B','D'},
                         {'_','_','_','_'}};
    Set<string> valid2 = {};
    Set<GridLocation> visited2;
    for (int r = 0; r < board2.numRows(); r++) {
        for (int c = 0; c < board2.numCols(); c++) {
            pathsHelper({r, c}, "", visited2, valid2, board2, sharedLexicon());
        }
    }
    EXPECT_EQUAL(valid2, {"bile", "bird", "brie", "cebid", "ceil", "diel", "drib", "lice", "riel", "rile"});
}
