#include <iostream>
#include "console.h"
#include "SimpleTest.h"
#include "maze.h"
#include "search.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our main() during grading

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    solveMazeBFS(maze);

    Vector<string> lines;
    readDatabaseFile("res/website.txt", lines);
    searchEngine(lines);

    cout << endl << "All done!" << endl;
    return 0;
}


// Do not remove or edit below this line. It is here to confirm that your code
// conforms to the expected function prototypes needed for grading
void confirmFunctionPrototypes() {
    Grid<bool> g;
    GridLocation loc;
    Set<GridLocation> set;
    Vector<GridLocation> path;
    string str;

    set = generateValidMoves(g, loc);
    validatePath(g, path);
    readMazeFile(str, g);
    readSolutionFile(str, path);
    path = solveMazeBFS(g);
    path = solveMazeDFS(g);

    Vector<string> lines;
    Set<string> strset;
    Map<std::string, Set<std::string>> map;
    readDatabaseFile(str, lines);
    str = cleanToken(str);
    strset = gatherTokens(str);
    buildIndex(lines, map);
    strset = findQueryMatches(map, str);
    searchEngine(lines);
}
