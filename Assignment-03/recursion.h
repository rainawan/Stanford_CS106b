#pragma once

/* Needed for sierpinski.cpp */
#include "gtypes.h"
#include "gwindow.h"
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three);
int drawSierpinskiTriangle(GWindow& window,
                            GPoint one, GPoint two, GPoint three,
                            int order);
void runInteractiveDemo();

/* Needed for merge.cpp */
#include "queue.h"
#include "vector.h"

Queue<int> binaryMerge(Queue<int> a, Queue<int> b);
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all);
Queue<int> recMultiMerge(Vector<Queue<int>>& all);

