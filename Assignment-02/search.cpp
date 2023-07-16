/*
 *  Search engine that matches an input text with a corresponding  webpage
 *  URL. Uses query matching to produce a unique id and match webpages.
 */

#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include <string>
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


// Takes in a string and returns a "cleaned" version that contains
// only numbers and lowercase letters.
string cleanToken(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i]) || isdigit(s[i])) {
            result += tolower(s[i]);
        }
    }
    return result;
}

// Separates a given string into a set of words. Each word is cleaned
// using the cleanToken function before being added to a set.
Set<string> gatherTokens(string text) {
    Set<string> tokens;
    Vector<string> words;
    string clean;
    words = stringSplit(text, " ");
    for (int i = 0; i < words.size(); i++) {
        clean = cleanToken(words[i]);
        if (!clean.empty())
            tokens.add(clean);
    }
    return tokens;
}

// Extract set of unique tokens. Store each token as a key and its
// associated website URL as a value. Returns the number of website URLs,
// and modifies the index map.
int buildIndex(Vector<string>& lines, Map<string, Set<string>>& index) {
    Set<string> tokens;
    int nPages = 0;
    for (int i = 0; i < lines.size(); i++) {
        if (i % 2 != 0) { // get words
            tokens = gatherTokens(lines[i]);
            for (string t : tokens) {
                index[t].add(lines[i-1]);
            }
        }
        else { // get website URL
            nPages++;
        }
    }
    return nPages;
}

// Takes in a dictionary mapping words to their website URLs, along with
// a specific query with rules. + represents union and - represents intersection.
// Returns a set of valid matches.
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;
    Vector<string>words;
    words = stringSplit(query, " ");
    for (int i = 0; i < words.size(); i++) {
        if (words[i][0] == '-') {
            Set<string> phrase1, phrase2;
            phrase1 = index[words[i-1]];
            words[i].erase(0,1);
            phrase2 = index[words[i]];
            // first, remove previous set b/c we want to find difference
            for (string s : phrase1) {
                result.remove(s);
            }
            // then, add differing elements
            phrase1.difference(phrase2);
            for (string s: phrase1) {
                result.add(s);
            }
        }
        else if (words[i][0] == '+') {
            Set<string> phrase1, phrase2;
            phrase1 = index[words[i-1]];
            words[i].erase(0,1);
            phrase2 = index[words[i]];
            // first, remove previous set b/c we want to find intersection
            for (string s : phrase1) {
                result.remove(s);
            }
            // then, add intersecting elements
            phrase1.intersect(phrase2);
            for (string s : phrase1) {
                result.add(s);
            }
        }
        else {
            for (string s : index[words[i]]) {
                result.add(s);
            }
        }
    }
    return result;
}

// Rearranges a vector of data into an inverted index. Retrieves
// a website URL through query matching, upon entering an input string.
void searchEngine(Vector<string>& lines) {
    string input = "";
    Map<string, Set<string>> index;
    Set<string> matches;

    do {
        input = getLine("\nEnter query sentence (RETURN/ENTER to quit): ");
        int snPages = buildIndex(lines, index);
        matches = findQueryMatches(index, input);
        cout << "Found " << matches.size() << " matching pages\n" ;
        cout << matches;
    } while (input != "");
}

/*
 * The given readDatabaseFile function correctly reads a well-formed
 * website database from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readDatabaseFile(string filename, Vector<string>& lines) {
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    readEntireFile(in, lines);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings of letters and digits") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS106B"), "cs106b");
}

PROVIDED_TEST("cleanToken on strings containing punctuation") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
    EXPECT_EQUAL(cleanToken("they're"), "theyre");
}

PROVIDED_TEST("cleanToken on string of only punctuation") {
    EXPECT_EQUAL(cleanToken("#$^@@.;"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 6 unique tokens, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 12 unique tokens") {
    Vector<string> lines;
    Map<string, Set<string>> index;
    readDatabaseFile("res/tiny.txt", lines);
    int nPages = buildIndex(lines, index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 12);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Vector<string> lines;
    Map<string, Set<string>> index;
    readDatabaseFile("res/tiny.txt", lines);
    buildIndex(lines, index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Vector<string> lines;
    Map<string, Set<string>> index;
    readDatabaseFile("res/tiny.txt", lines);
    buildIndex(lines, index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


STUDENT_TEST("Verify clean token") {
    EXPECT_EQUAL(cleanToken("*#(@&$"), "");
    EXPECT_EQUAL(cleanToken("__TREE"),"tree");
    EXPECT_EQUAL(cleanToken("19He_29b+*"), "19he29b");
}

STUDENT_TEST("Verify gather tokens") {
    Set<string> expected = {"hello", "world"};
    EXPECT_EQUAL(gatherTokens("He**llo World!"), expected);

    Set<string> expected2 = {"one", "two", "three"};
    EXPECT_EQUAL(gatherTokens("one __two !! three+"), expected2);

    Set<string> expected3 = {"cs106b", "is", "awesome"};
    EXPECT_EQUAL(gatherTokens("CS106**b IS (awesome)"), expected3);
}

STUDENT_TEST("buildIndex from website.txt") {
    Vector<string> lines;
    Map<string, Set<string>> index;
    readDatabaseFile("res/website.txt", lines);
    int nPages = buildIndex(lines, index);
    EXPECT_EQUAL(nPages, 36);

    EXPECT(index.containsKey("file"));
    EXPECT(index.containsKey("week"));
    EXPECT(!index.containsKey("helloo"));
    TIME_OPERATION(lines.size(), buildIndex(lines, index)); // 0.110 sec
}

STUDENT_TEST("findQueryMatches extra testing") {
    Vector<string> lines;
    Map<string, Set<string>> index;
    readDatabaseFile("res/tiny.txt", lines);
    buildIndex(lines, index);
    Set<string> matchesFish = findQueryMatches(index, "fish");
    EXPECT_EQUAL(matchesFish.size(), 3);
    Set<string> matchesFishWithoutRed = findQueryMatches(index, "fish -red");
    EXPECT_EQUAL(matchesFishWithoutRed.size(), 2);
    Set<string> matchesOr = findQueryMatches(index, "fish -red green");
    EXPECT_EQUAL(matchesOr.size(), 3);
    Set<string> matchesOrAnd = findQueryMatches(index, "green +eat fish -red");
    EXPECT_EQUAL(matchesOrAnd.size(), 2);
}
