#include "Log.hpp"
#include <fstream>

SimulationLogger::SimulationLogger(const std::string& name)
    : simulationName(name), startTime(std::chrono::steady_clock::now()) {}

void SimulationLogger::start() {
    startTime = std::chrono::steady_clock::now();
}

void SimulationLogger::logStep(const Grid& grid) {
    ++steps;
    for (const auto& row : grid)
        for (char cell : row)
            if (cell)
                ++totalAliveCells;
}

void SimulationLogger::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    file << "Nazwa symulacji: " << simulationName << '\n';
    file << "Czas trwania: " << duration << "s" << '\n';
    file << "Liczba kroków: " << steps << '\n';
    file << "Sumaryczna liczba żywych komórek: " << totalAliveCells << '\n';
}
