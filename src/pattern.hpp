#pragma once
#include <vector>
#include <string>
#include <filesystem>

using Pattern = std::vector<std::vector<char>>;

class PatternLoader {
public:
    PatternLoader(const std::string& folder);
    const Pattern& getCurrent() const;
    void next();
    void previous();
    void loadPatterns();

private:
    std::vector<Pattern> patterns;
    int currentIndex = 0;
    std::string folderPath;

    Pattern loadFromFile(const std::filesystem::path& path);
};
