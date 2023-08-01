#pragma once

/* Needed for predict.cpp */
#include "set.h"
#include "lexicon.h"
#include <string>
void predict(std::string digits, Set<std::string>& suggestions, Lexicon& lex);
