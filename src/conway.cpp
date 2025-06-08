#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <sstream> // <-- dodaj to na górze pliku

#include "conway.hpp"

using Grid = std::unordered_map<int, std::unordered_map<int, bool>>;


sf::RectangleShape makeCell(int x, int y, sf::Color color, float cellSize) {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setFillColor(color); // OK – cell to sf::RectangleShape
    cell.setPosition(sf::Vector2f(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)));
    return cell;
}

int countAliveNeighbors(const Grid& grid, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            auto rowIt = grid.find(ny);
            if (rowIt != grid.end()) {
                auto colIt = rowIt->second.find(nx);
                if (colIt != rowIt->second.end() && colIt->second) {
                    ++count;
                }
            }
        }
    }
    return count;
}


void runConway(sf::RenderWindow& window, int speed) {
    Grid grid;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    bool paused = false;
    const float cellSize = 20.0f;

    // Zegar i tekst
    sf::Clock clock;
    sf::Font font;
    sf::Text infoText(font, "", 16);
    int liczbaKrokow = 0;

    // Załaduj czcionkę (upewnij się, że plik istnieje w folderze projektu!)
    if (!font.openFromFile("/Users/konradciemala/Library/Fonts/Oswald-VariableFont_wght.ttf")) {
        return;
    }

    infoText.setFont(font);
    infoText.setCharacterSize(16);
    infoText.setFillColor(sf::Color::Red);
    infoText.setPosition({window.getSize().x - 250.0f, 12.0f}); // prawy górny róg

    sf::RectangleShape rectB;
    rectB.setSize(sf::Vector2f(150.0f, 60.0f));
    rectB.setFillColor(sf::Color::Black);
    rectB.setPosition({window.getSize().x - 260.0f, 5.0f});
    
    sf::RectangleShape rectW;
    rectW.setSize(sf::Vector2f(140.0f, 50.0f));
    rectW.setFillColor(sf::Color::White);
    rectW.setPosition({window.getSize().x - 255.0f, 10.0f});
    

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                for (int i = 0; i < 100; i++) {
                    for (int j = 0; j < 100; j++) {
                        grid[i][j] = std::rand() % 2;
                    }
                }
                clock.restart(); // zresetuj czas po resecie
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
                grid.clear();
                clock.restart(); // zresetuj czas po czyszczeniu
                liczbaKrokow = 0; // zresetuj liczbę kroków
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    paused = !paused;
                }
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                    speed = std::min(100, speed + 1); // Zwiększ prędkość
                }
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                    speed = std::max(1, speed - 1); // Zmniejsz prędkość
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int gridX = mousePos.x / cellSize;
                    int gridY = mousePos.y / cellSize;
                    grid[gridY][gridX] = !grid[gridY][gridX];
                }
            }
        }

        if (!paused) {
            Grid nextGrid;
            std::unordered_map<int, std::unordered_map<int, bool>> cellsToCheck = grid;

            for (const auto& [y, row] : grid) {
                for (const auto& [x, alive] : row) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            cellsToCheck[y + dy][x + dx] = false;
                        }
                    }
                }
            }

            for (const auto& [y, row] : cellsToCheck) {
                for (const auto& [x, _] : row) {
                    int aliveNeighbors = countAliveNeighbors(grid, x, y);
                    bool currentlyAlive = grid[y][x];

                    if (currentlyAlive && (aliveNeighbors == 2 || aliveNeighbors == 3)) {
                        nextGrid[y][x] = true;
                    } else if (!currentlyAlive && aliveNeighbors == 3) {
                        nextGrid[y][x] = true;
                    }
                }
            }

            grid = std::move(nextGrid);
            liczbaKrokow++;
        }

        // Oblicz liczbę żywych komórek
        int aliveCount = 0;
        for (const auto& [y, row] : grid) {
            for (const auto& [x, alive] : row) {
                if (alive) aliveCount++;
            }
        }

        // Czas w sekundach
        float elapsedTime = clock.getElapsedTime().asSeconds();

        // Tekst informacyjny
        std::ostringstream ss;
        ss << "Zywe komorki: " << aliveCount << "\n";
        ss << "Czas: " << liczbaKrokow;
        infoText.setString(ss.str());

       

        // Rysowanie
        window.clear(sf::Color::White);

        for (int x = 0; x <= window.getSize().x; x += cellSize) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, 0), sf::Color(200, 200, 200)),
                sf::Vertex(sf::Vector2f(x, window.getSize().y), sf::Color(200, 200, 200))
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        for (int y = 0; y <= window.getSize().y; y += cellSize) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(0, y), sf::Color(200, 200, 200)),
                sf::Vertex(sf::Vector2f(window.getSize().x, y), sf::Color(200, 200, 200))
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        for (const auto& [y, row] : grid) {
            for (const auto& [x, alive] : row) {
                if (alive) {
                    auto rect = makeCell(x, y, sf::Color::Black, cellSize);
                    window.draw(rect);
                }
            }
        }

        window.draw(rectB); // Rysuj prostokąt tła
        window.draw(rectW);
        window.draw(infoText); // <-- wyświetl tekst informacyjny

        window.display();
        sf::sleep(sf::milliseconds(speed * 10));
    }
}
