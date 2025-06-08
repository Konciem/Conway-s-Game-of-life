#pragma once

#include <SFML/Graphics.hpp>

extern const int cellSize;
extern const int gridSize;

void runConway(sf::RenderWindow& window, int speed);

sf::RectangleShape makeCell(int x, int y, sf::Color color, float cellSize);

int countAliveNeighbors(const std::unordered_map<int, std::unordered_map<int, bool>>& grid, int x, int y);
