#include "random.hpp"
#include <cstdlib>
#include <random>
#include <ranges>
#include <algorithm>

void Randomizer::fillRandom(std::vector<std::vector<char>>& grid) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 1);

    for (auto& row : grid | std::views::all) {
        std::ranges::generate(row, [] { return static_cast<char>(dist(gen)); });
    }
}

void Randomizer::clear(std::vector<std::vector<char>>& grid) {
    for (auto& row : grid | std::views::all) {
        std::ranges::fill(row, false);
    }
}
