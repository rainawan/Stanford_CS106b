#pragma once

/* Needed for balanced.cpp */
#include <string>
#include "gtypes.h"
#include "gwindow.h"
bool isBalanced(std::string str);
std::string operatorsFrom(std::string str);
bool operatorsAreMatched(std::string ops);
