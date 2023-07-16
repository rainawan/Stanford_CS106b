/*
 *  Use stacks, queues, sets, BFS, and DFS to process and solve mazes.
 *  Create functions that generate valid neighbors, and a final path
 *  from the top left to bottom right corner, while avoiding walls.
 */

#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

// Takes in coordinates and determines if out of bounds or not.
// Returns false if out of bounds or if maze at location is false.
// Returnst true if location is true and within bounds.
GridLocation validHelper(Grid<bool>& maze, GridLocation cur) {
    const int ROWS = maze.numRows();
    const int COLS = maze.numCols();

    if (cur.row < 0 or cur.row >= ROWS or cur.col < 0 or cur.col >= COLS)
        return {-1,-1};  // out of bounds
    else if (maze[cur.row][cur.col] == true)
        return cur;
    else
        return {-1, -1};  // false
}

// Takes in coordinates and passes neighboring squares to helper function.
// If neighbor is valid path, adds location to set.
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    const int ROWS = maze.numRows();
    const int COLS = maze.numCols();
    GridLocation invalid = {-1,-1};

    if (!(validHelper(maze, {cur.row-1, cur.col}) == invalid))
        neighbors.add({cur.row-1, cur.col});
    if (!(validHelper(maze, {cur.row+1, cur.col}) == invalid))
        neighbors.add({cur.row+1, cur.col});
    if (!(validHelper(maze, {cur.row, cur.col+1}) == invalid))
        neighbors.add({cur.row, cur.col+1});
    if (!(validHelper(maze, {cur.row, cur.col-1}) == invalid))
        neighbors.add({cur.row, cur.col-1});

    return neighbors;
}

// Confirm maze path by validating path is not empty, starts at top left
// and ends at bottom right, paths are not repeated, and path does not
// contain a loop.
void validatePath(Grid<bool>& maze, Vector<GridLocation>& path) {
    if (path.isEmpty()) {
        error("Path is empty!");
    }

    GridLocation start = {0,0};
    GridLocation end = {maze.numRows() - 1, maze.numCols() - 1};
    Set<GridLocation> visited;

    if (!(path.get(0) == start)) {
        error("Doesn't start top left.");
    }
    if (!((path.get(path.size() - 1)) == end)) {
        error("Doesn't end in bottom left.");
    }

    visited.add(path.get(0));

    for (int i = 0; i < path.size() - 1; i++) {
        Set<GridLocation> valid = generateValidMoves(maze, path[i]);
        if (!(valid.contains(path[i+1])))
            error("Invalid path");
        if (visited.contains(path[i+1]))
            error("Already visited");
        visited.add(path[i+1]);
    }
}

// Finds a valid maze path by adding possible paths to a queue,
// then popping the front of the queue and adding more valid locations.
// Paths are added in order from the entry (top left) to the exit (bototm right).
Vector<GridLocation> solveMazeBFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Queue<Vector<GridLocation>> allPaths;

    drawMaze(maze);
    Vector<GridLocation> curr;
    GridLocation entry = {0, 0};
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    path.add(entry);

    Set<GridLocation> visited;
    visited.add(entry);

    allPaths.enqueue(path);

    while (!allPaths.isEmpty()) {
        curr = allPaths.dequeue();
        if (curr[curr.size() - 1] == exit) { // found exit. return path
            return curr;
        }
        Set<GridLocation> moves = generateValidMoves(maze, curr[curr.size() - 1]);
        for (GridLocation loc : moves) {
            if (!visited.contains(loc)) {
                curr.add(loc);
                allPaths.enqueue(curr);
                curr.remove(curr.size() - 1);
                visited.add(loc);
            }
        }
    }

    return path;
}

// Finds a valid maze path by exploring a single path to its fullest
// depth before moving onto other paths if unsuccessful. Uses a stack
// to push and remove potential paths.
Vector<GridLocation> solveMazeDFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Stack<Vector<GridLocation>> allPaths;

    drawMaze(maze);
    Vector<GridLocation> curr;
    GridLocation entry = {0, 0};
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    path.add(entry);

    Set<GridLocation> visited;
    visited.add(entry);

    allPaths.push(path);

    while (!allPaths.isEmpty()) {
        curr = allPaths.pop();
        if (curr[curr.size() - 1] == exit) { // found exit. return path
            return curr;
        }
        Set<GridLocation> moves = generateValidMoves(maze, curr[curr.size() - 1]);
        for (GridLocation loc : moves) {
            if (!visited.contains(loc)) {
                curr.add(loc);
                allPaths.push(curr);
                curr.remove(curr.size() - 1);
                visited.add(loc);
            }
        }
    }

    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Vector<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution, hand-constructed maze") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Vector<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution read from file, medium maze") {
    Grid<bool> maze;
    Vector<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true},
                       {true, true}};
    Vector<GridLocation> notBeginAtEntry = { {1, 1}, {2, 1} };
    Vector<GridLocation> notEndAtExit = { {0, 0}, {1, 0}, {2, 0} };
    Vector<GridLocation> moveThroughWall = { {0 ,0}, {0, 1}, {1, 1}, {2, 1} };

    EXPECT_ERROR(validatePath(maze, notBeginAtEntry));
    EXPECT_ERROR(validatePath(maze, notEndAtExit));
    EXPECT_ERROR(validatePath(maze, moveThroughWall));
}

PROVIDED_TEST("solveMazeBFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}






STUDENT_TEST("generate valid moves") {
    Grid<bool> maze = {{true, false, true},
                       {false, true, false},
                       {true, false, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {};
    EXPECT_EQUAL(generateValidMoves(maze, center), expected);

    Grid<bool> maze2 = {{false, true, true},
                        {true, true, true},
                        {true, true, true}};
    GridLocation center2 = {1,0};
    Set<GridLocation> expected2 = {{1,1},{2,0}};
    EXPECT_EQUAL(generateValidMoves(maze2, center2), expected2);

    Grid<bool> maze3 ={{true, false, true},
                       {true, false, true},
                       {true, true, true}};
    GridLocation center3 = {0,0};
    Set<GridLocation> expected3 = {{1,0}};
    EXPECT_EQUAL(generateValidMoves(maze3, center3), expected3);
}

STUDENT_TEST("validatePath on valid path") {
    Grid<bool> maze = {{true, false, true},
                       {true, false, true},
                       {true, true, true}};
    Vector<GridLocation> soln = { {0 ,0}, {1, 0}, {2, 0}, {2, 1}, {2, 2} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on invalid paths") {
    Grid<bool> maze = {{true, false, true},
                       {true, true, true},
                       {true, true, false}};
    Vector<GridLocation> notBeginAtEntry = { {1, 0}, {2, 1} };
    Vector<GridLocation> notEndAtExit = { {0, 0}, {1, 0}, {1, 1}, {1, 2} };
    Vector<GridLocation> moveThroughWall = { {0 ,0}, {0, 1}, {0, 2} };

    EXPECT_ERROR(validatePath(maze, notBeginAtEntry));
    EXPECT_ERROR(validatePath(maze, notEndAtExit));
    EXPECT_ERROR(validatePath(maze, moveThroughWall));
}

STUDENT_TEST("BFS solve maze") {
    Grid<bool> maze = {{true, false, true},
                       {true, true, true}};
    Vector<GridLocation> sol = { {0,0}, {1,0}, {1,1}, {1,2} };
    solveMazeBFS(maze);
    EXPECT_EQUAL(solveMazeBFS(maze), sol);

    Grid<bool> maze2 = {{true, true, false},
                        {false, true, false},
                        {true, true, true}};
    Vector<GridLocation> sol2 = { {0,0}, {0,1}, {1,1}, {2,1}, {2,2} };
    solveMazeBFS(maze2);
    EXPECT_EQUAL(solveMazeBFS(maze2), sol2);

    Grid<bool> maze3;
    readMazeFile("res/21x25.maze", maze3);
    Vector<GridLocation> sol3 = solveMazeBFS(maze3);

    EXPECT_NO_ERROR(validatePath(maze3, sol3));
}

STUDENT_TEST("DFS solve maze") {
    Grid<bool> maze = {{true, true, true},
                       {false, false, true}};
    Vector<GridLocation> sol = { {0,0}, {0,1}, {0,2}, {1,2} };
    solveMazeBFS(maze);
    EXPECT_EQUAL(solveMazeBFS(maze), sol);

    Grid<bool> maze2 = {{true, true, false},
                        {false, true, false},
                        {true, true, true}};
    Vector<GridLocation> sol2 = { {0,0}, {0,1}, {1,1}, {2,1}, {2,2} };
    solveMazeBFS(maze2);
    EXPECT_EQUAL(solveMazeBFS(maze2), sol2);

    Grid<bool> maze3;
    readMazeFile("res/19x35.maze", maze3);
    Vector<GridLocation> sol3 = solveMazeBFS(maze3);
    EXPECT_NO_ERROR(validatePath(maze3, sol3));
}
