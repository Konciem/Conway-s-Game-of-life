#include "gameState.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>


bool saveToFile(const Grid& grid, int krok, const std::string& filename) {
    std::filesystem::path filePath = filename;

    // Tworzymy katalog, jeśli nie istnieje
    if (!filePath.parent_path().empty() && !std::filesystem::exists(filePath.parent_path())) {
        std::filesystem::create_directories(filePath.parent_path());
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) return false;

    outFile << krok << '\n';
    outFile << grid.size() << ' ' << grid[0].size() << '\n';

    for (const auto& row : grid) {
        for (char cell : row)
            outFile << static_cast<int>(cell);
        outFile << '\n';
    }

    return true;
}

bool loadFromFile(Grid& grid, int& krok, const std::string& filename) {
    std::filesystem::path filePath = filename;

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "Plik nie istnieje: " << filePath << '\n';
        return false;
    }

    std::ifstream inFile(filePath);
    if (!inFile.is_open()) return false;

    int rows, cols;
    inFile >> krok >> rows >> cols;
    inFile.ignore(); // pomijamy znak nowej linii

    grid.assign(rows, std::vector<char>(cols, 0));

    for (int y = 0; y < rows; ++y) {
        std::string line;
        std::getline(inFile, line);
        for (int x = 0; x < cols && x < static_cast<int>(line.size()); ++x)
            grid[y][x] = line[x] - '0';
    }

    return true;
}
