#pragma once

#include <vector>
#include <string>

using Grid = std::vector<std::vector<char>>;

bool saveToFile(const Grid& grid, int krok, const std::string& filename);
bool loadFromFile(Grid& grid, int& krok, const std::string& filename);

