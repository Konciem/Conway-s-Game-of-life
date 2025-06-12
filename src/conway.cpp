#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <ranges>
#include <numeric>
#include <concepts>

#include "infoPanel.hpp"
#include "gameState.hpp"
#include "pattern.hpp"
#include "random.hpp"


using Grid = std::vector<std::vector<char>>;

//Concepty
template<typename T>
concept SiatkaChar =
    std::ranges::range<T> &&
    std::ranges::range<std::ranges::range_value_t<T>> &&
    std::same_as<std::ranges::range_value_t<std::ranges::range_value_t<T>>, char>;

int policzZyweKomorki(SiatkaChar auto const& siatka) {
    return std::accumulate(siatka.begin(), siatka.end(), 0,
        [](int suma, const auto& wiersz) {
            return suma + std::ranges::count(wiersz, true);
        });
}

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

    Grid grid(liczbaWierszy, std::vector<char>(liczbaKolumn, false));

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    bool paused = false;
    bool showShadow = true;

    sf::Clock clock;
    sf::Font font;
    int liczbaKrokow = 0;

    if (!font.openFromFile("/Users/konradciemala/Library/Fonts/Oswald-VariableFont_wght.ttf")) {
        return;
    }

    InfoPanel infoPanel(window.getSize().x - 260.0f, 5.0f, font);
    const std::string nazwaPliku = "save.txt";

    PatternLoader loader("patterns");
    Pattern currentPattern = loader.getCurrent();

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int gridX = mousePos.x / cellSize;
        int gridY = mousePos.y / cellSize;

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) saveToFile(grid, liczbaKrokow, nazwaPliku);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) loadFromFile(grid, liczbaKrokow, nazwaPliku);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) Randomizer::fillRandom(grid);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
                    Randomizer::clear(grid);
                    liczbaKrokow = 0;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) paused = !paused;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) speed = std::min(1000, speed + 1);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) speed = std::max(1, speed - 1);
            }

            if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (wheel->wheel == sf::Mouse::Wheel::Vertical) {
                    if (wheel->delta > 0) {
                // scroll w gore
                        loader.next();
                    } else {
                // scroll w dol
                        loader.previous();
                    }
                    currentPattern = loader.getCurrent();
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    for (size_t y = 0; y < currentPattern.size(); ++y) {
                        for (size_t x = 0; x < currentPattern[y].size(); ++x) {
                            int gx = gridX + x;
                            int gy = gridY + y;
                            if (gx >= 0 && gx < liczbaKolumn && gy >= 0 && gy < liczbaWierszy) {
                                grid[gy][gx] = currentPattern[y][x];
                            }
                        }
                    }
                }
            }
        }

        if (!paused) {
            Grid nextGrid = grid;
            for (int y = 0; y < liczbaWierszy; ++y) {
                for (int x = 0; x < liczbaKolumn; ++x) {
                    int alive = countAliveNeighbors(grid, x, y, liczbaKolumn, liczbaWierszy);
                    nextGrid[y][x] = (grid[y][x] ? (alive == 2 || alive == 3) : (alive == 3));
                }
            }
            grid = std::move(nextGrid);
            liczbaKrokow++;
        }

        int aliveCount = policzZyweKomorki(grid);
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

        for (int y = 0; y < liczbaWierszy; ++y)
            for (int x = 0; x < liczbaKolumn; ++x)
                if (grid[y][x])
                    window.draw(makeCell(x, y, sf::Color::Black, cellSize));

        // Wyświetlanie cienia
        if (showShadow) {
            for (size_t y = 0; y < currentPattern.size(); ++y) {
                for (size_t x = 0; x < currentPattern[y].size(); ++x) {
                    int gx = gridX + x;
                    int gy = gridY + y;
                    if (gx >= 0 && gx < liczbaKolumn && gy >= 0 && gy < liczbaWierszy) {
                        if (currentPattern[y][x]) {
                            auto ghost = makeCell(gx, gy, sf::Color(100, 100, 100, 150), cellSize);
                            window.draw(ghost);
                        }
                    }
                }
            }
        }

        infoPanel.update(aliveCount, liczbaKrokow);
        infoPanel.draw(window);
        window.display();
        sf::sleep(sf::milliseconds(speed));
    }
}
