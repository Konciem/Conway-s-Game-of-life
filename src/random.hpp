#pragma once
#include <vector>

class Randomizer {
public:
    static void fillRandom(std::vector<std::vector<char>>& grid);
    static void clear(std::vector<std::vector<char>>& grid);
};
