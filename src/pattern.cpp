#include "pattern.hpp"
#include <fstream>
#include <filesystem>

PatternLoader::PatternLoader(const std::string& folder) : folderPath(folder) {
    loadPatterns();
}

void PatternLoader::loadPatterns() {
    using namespace std::filesystem;
    patterns.clear();
    currentIndex = 0;

    for (const auto& entry : directory_iterator(folderPath)) {
        if (entry.path().extension() == ".txt") {
            patterns.push_back(loadFromFile(entry.path()));
        }
    }
}

Pattern PatternLoader::loadFromFile(const std::filesystem::path& path) {
    Pattern pattern;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c == '1');
        }
        pattern.push_back(row);
    }
    return pattern;
}

const Pattern& PatternLoader::getCurrent() const {
    return patterns.at(currentIndex);
}

void PatternLoader::next() {
    if (!patterns.empty())
        currentIndex = (currentIndex + 1) % patterns.size();
}

void PatternLoader::previous() {
    if (!patterns.empty())
        currentIndex = (currentIndex - 1 + patterns.size()) % patterns.size();
}
