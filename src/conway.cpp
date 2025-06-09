#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdlib>

using Grid = std::vector<std::vector<bool>>;

sf::RectangleShape makeCell(int x, int y, sf::Color color, float cellSize) {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setFillColor(color);
    cell.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
    return cell;
}

int countAliveNeighbors(const Grid& grid, int x, int y, int szerokosc, int wysokosc) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < szerokosc && ny >= 0 && ny < wysokosc) {
                if (grid[ny][nx]) ++count;
            }
        }
    }
    return count;
}

void runConway(sf::RenderWindow& window, int speed, float cellSize) {
    const int oknoSzerokosc = window.getSize().x;
    const int oknoWysokosc = window.getSize().y;

    const int liczbaKolumn = oknoSzerokosc / cellSize;
    const int liczbaWierszy = oknoWysokosc / cellSize;

    Grid grid(liczbaWierszy, std::vector<bool>(liczbaKolumn, false));

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    bool paused = false;

    sf::Clock clock;
    sf::Font font;
    sf::Text infoText(font, "", 16);
    infoText.setStyle(sf::Text::Underlined);
    int liczbaKrokow = 0;

    if (!font.openFromFile("/Users/konradciemala/Library/Fonts/Oswald-VariableFont_wght.ttf")) {
        return;
    }

    infoText.setFont(font);
    infoText.setCharacterSize(16);
    infoText.setFillColor(sf::Color::Red);
    infoText.setPosition({window.getSize().x - 250.0f, 12.0f});

    sf::RectangleShape rectB(sf::Vector2f(150.0f, 60.0f));
    rectB.setFillColor(sf::Color::Black);
    rectB.setPosition({window.getSize().x - 260.0f, 5.0f});

    sf::RectangleShape rectW(sf::Vector2f(140.0f, 50.0f));
    rectW.setFillColor(sf::Color::White);
    rectW.setPosition({window.getSize().x - 255.0f, 10.0f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                for (int y = 0; y < liczbaWierszy; ++y)
                    for (int x = 0; x < liczbaKolumn; ++x)
                        grid[y][x] = std::rand() % 2;
                clock.restart();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
                for (auto& row : grid) std::fill(row.begin(), row.end(), false);
                clock.restart();
                liczbaKrokow = 0;
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) paused = !paused;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) speed = std::min(100, speed + 1);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) speed = std::max(1, speed - 1);
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int gridX = mousePos.x / cellSize;
                    int gridY = mousePos.y / cellSize;
                    if (gridX >= 0 && gridX < liczbaKolumn && gridY >= 0 && gridY < liczbaWierszy) {
                        grid[gridY][gridX] = !grid[gridY][gridX];
                    }
                }
            }
        }

        if (!paused) {
            Grid nextGrid = grid;

            for (int y = 0; y < liczbaWierszy; ++y) {
                for (int x = 0; x < liczbaKolumn; ++x) {
                    int alive = countAliveNeighbors(grid, x, y, liczbaKolumn, liczbaWierszy);
                    if (grid[y][x]) {
                        nextGrid[y][x] = (alive == 2 || alive == 3);
                    } else {
                        nextGrid[y][x] = (alive == 3);
                    }
                }
            }

            grid = std::move(nextGrid);
            liczbaKrokow++;
        }

        int aliveCount = 0;
        for (const auto& row : grid)
            for (bool cell : row)
                if (cell) ++aliveCount;

        std::ostringstream ss;
        ss << "Zywe komorki: " << aliveCount << "\n";
        ss << "Czas: " << liczbaKrokow;
        infoText.setString(ss.str());

        window.clear(sf::Color::White);

        for (int x = 0; x <= oknoSzerokosc; x += cellSize) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, 0), sf::Color(200, 200, 200)),
                sf::Vertex(sf::Vector2f(x, oknoWysokosc), sf::Color(200, 200, 200))
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        for (int y = 0; y <= oknoWysokosc; y += cellSize) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(0, y), sf::Color(200, 200, 200)),
                sf::Vertex(sf::Vector2f(oknoSzerokosc, y), sf::Color(200, 200, 200))
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        for (int y = 0; y < liczbaWierszy; ++y) {
            for (int x = 0; x < liczbaKolumn; ++x) {
                if (grid[y][x]) {
                    auto rect = makeCell(x, y, sf::Color::Black, cellSize);
                    window.draw(rect);
                }
            }
        }

        window.draw(rectB);
        window.draw(rectW);
        window.draw(infoText);
        window.display();
        sf::sleep(sf::milliseconds(speed * 10));
    }
}
