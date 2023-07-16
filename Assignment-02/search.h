#pragma once

#include "map.h"
#include "set.h"
#include "vector.h"
#include <string>

// Prototypes to be shared with other modules

void readDatabaseFile(std::string filename, Vector<std::string>& lines);

std::string cleanToken(std::string token);

Set<std::string> gatherTokens(std::string bodyText);

int buildIndex(Vector<std::string>& lines, Map<std::string, Set<std::string>>& index);

Set<std::string> findQueryMatches(Map<std::string, Set<std::string>>& index, std::string query);

void searchEngine(Vector<std::string>& lines);
