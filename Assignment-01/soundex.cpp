/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <cctype>
#include <fstream>
#include <string>
#include <map>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "graph.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* This function is intended to return a string which
 * includes only the letter characters from the original
 * (all non-letter characters are excluded)
 *
 * WARNING: The provided code is buggy!
 *
 * Use test cases to identify which inputs to this function
 * are incorrectly handled.
 * TODO: Remove this comment and replace it with a 
 * description of the bug you fixed.
 */
string lettersOnly(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/*
 * Encode each letter to digit using defined table.
 */

int encodeLetter(char input) {
    map<int, vector<char>> dict;
    dict[0] = {'A', 'E', 'I', 'O', 'U', 'H', 'W', 'Y'};
    dict[1] = {'B', 'F', 'P', 'V'};
    dict[2] = {'C', 'G', 'J', 'K', 'Q', 'S', 'X', 'Z'};
    dict[3] = {'D', 'T'};
    dict[4] = {'L'};
    dict[5] = {'M', 'N'};
    dict[6] = {'R'};

    for (auto& i : dict) {
        for (int j = 0; j < dict[i.first].size(); j++) {
            if (i.second[j] == toupper(input)) {
                return i.first;
            }
        }
    }

    return -1;
}

/* Given a string, remmove duplicate letters. Keep a left and right pointer
 * and if the letters are different, append to string. If they are repeating,
 * simply increment both pointers without adding to string.
 */
string removeDuplicates(string s) {
    if (s.size() == 0 or s.size() == 1)
        return s;

    int left(0), right(1);
    string res;
    res += s[left];

    while (right != s.size()) {
        if (s[left] != s[right]) {
            res += s[right];
        }
        left += 1;
        right += 1;
    }

    return res;
}

/*
 * Replace first digit of string with first letter of original name
 */

string updateFirst(string orig, string curr) {
    curr[0] = toupper(orig[0]);
    return curr;
}

/*
 * Discard zeros from code
 */
void discardZeros(string& s) {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '0')
            s.erase(i, 1);
    }
    if (s[s.size() - 1] == '0')
        s.pop_back();
//    return s;
}

/*
 * Make code length exactly 4. Add remaining zeros if size is less than 4.
 * Remove following characters greater than 4.
 */
void lengthFour(string& s) {
    if (s.size() > 4) {
       s.erase(4, s.size() - 1);
    }
    else { // s.size() < 4)
        while (s.size() < 4) {
            s += '0';
        }
    }
}

/* Input a surname and convert it to its Soundex code, a four-character
 * word with an initial followed by three digits. The first letter of the code
 * is the first letter of the input and the following characters are drawm from
 * a table.
 */
string soundex(string s) {
    string res;
    res = lettersOnly(s);
    for (int i = 0; i < res.length(); i++) {
        res.replace(i,1,to_string(encodeLetter(res[i])));
    }
    res = removeDuplicates(res);
    res = updateFirst(s, res);
    discardZeros(res);
    lengthFour(res);

    return res;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;
    Vector<string> soundexNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, allNames);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    string line = getLine("Enter a surname (RETURN to quit): ");
    string code = soundex(line);

    code = soundex(line);
    cout << "Soundex code is " << code;
    for (string s : allNames) {
        string sound = soundex(s);
        if (sound == code)
            soundexNames.add(s);
    }
    soundexNames.sort();
    cout << "\nMatches from database: " << soundexNames;


//    cout << "All done!";
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}


STUDENT_TEST("Letters only") {
    EXPECT_EQUAL(lettersOnly("AB8D"), "ABD");
    EXPECT_EQUAL(lettersOnly(""), "");
    EXPECT_EQUAL(lettersOnly("1839"), "");
}

STUDENT_TEST("Encoding char to int") {
    EXPECT_EQUAL(encodeLetter('A'), 0);
    EXPECT_EQUAL(encodeLetter('R'), 6);
    EXPECT_EQUAL(encodeLetter('t'), 3); // lowercase letter
    EXPECT_EQUAL(encodeLetter('1'), -1); // invalid char
}

STUDENT_TEST("Remove duplicate letters") {
    EXPECT_EQUAL(removeDuplicates("abcccddef"), "abcdef");
    EXPECT_EQUAL(removeDuplicates("hello worrrrld"), "helo world");
    EXPECT_EQUAL(removeDuplicates("h"), "h");
    EXPECT_EQUAL(removeDuplicates(""), "");
}

STUDENT_TEST("Update first letter") {
    EXPECT_EQUAL(updateFirst("Marie", "9601"), "M601");
}

STUDENT_TEST("Discard zeros") {
    string s = "0B70";
    discardZeros(s);
    EXPECT_EQUAL(s, "B7");

    string t = "A900";
    discardZeros(t);
    EXPECT_EQUAL(t, "A9");

    string u = "Y308";
    discardZeros(u);
    EXPECT_EQUAL(u, "Y38");
}

STUDENT_TEST("Length of four") {
    string s = "T5";
    lengthFour(s);
    EXPECT_EQUAL(s, "T500");

    string t = "B1122";
    lengthFour(t);
    EXPECT_EQUAL(t, "B112");

    string u = "";
    lengthFour(u);
    EXPECT_EQUAL(u,"0000");
}

STUDENT_TEST("Soundex search") {
    soundexSearch("/Users/rainawan/Downloads/CS 106B/starter-assign1/res/surnames.txt");
}
