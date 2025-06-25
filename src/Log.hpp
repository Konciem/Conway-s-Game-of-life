#pragma once
#include <string>
#include <chrono>
#include <vector>

using Grid = std::vector<std::vector<char>>;

class SimulationLogger {
private:
    std::string simulationName;
    int totalAliveCells = 0;
    int steps = 0;
    std::chrono::steady_clock::time_point startTime;

public:
    SimulationLogger(const std::string& name);

    void start();
    void logStep(const Grid& grid);
    void saveToFile(const std::string& filename) const;
};
