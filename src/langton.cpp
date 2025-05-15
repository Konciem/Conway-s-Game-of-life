#include "langton.hpp"
#include <unordered_map>

constexpr int windowSize = 1000;
constexpr int cellSize = 10;
constexpr int gridSize = windowSize / cellSize;

enum Direction { Up, Right, Down, Left };

sf::RectangleShape makeCell(int x, int y, sf::Color color) {
    sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
    rect.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
    rect.setFillColor(color);
    return rect;
}

void runLangtonsAnt(sf::RenderWindow& window, int speed) {
    std::unordered_map<int, std::unordered_map<int, bool>> grid;

    int antX = gridSize / 2;
    int antY = gridSize / 2;
    Direction antDir = Up;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        bool& isBlack = grid[antY][antX];

        if (isBlack)
            antDir = static_cast<Direction>((antDir + 3) % 4); // left
        else
            antDir = static_cast<Direction>((antDir + 1) % 4); // right

        isBlack = !isBlack;

        switch (antDir) {
            case Up:    --antY; break;
            case Right: ++antX; break;
            case Down:  ++antY; break;
            case Left:  --antX; break;
        }

        window.clear(sf::Color::White);

        for (const auto& [y, row] : grid) {
            for (const auto& [x, black] : row) {
                if (black) {
                    auto rect = makeCell(x, y, sf::Color::Black);
                    window.draw(rect);
                }
            }
        }

        sf::RectangleShape antShape(sf::Vector2f(cellSize, cellSize));
        antShape.setPosition(sf::Vector2f(antX * cellSize, antY * cellSize));
        antShape.setFillColor(sf::Color::Red);
        window.draw(antShape);

        window.display();
        sf::sleep(sf::milliseconds(speed));
    }
}
