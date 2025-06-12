#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include "infoPanel.hpp"
#include "ant.hpp"

using Grid = std::vector<std::vector<char>>;

sf::RectangleShape makeCell1(int x, int y, sf::Color color, float cellSize) {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setFillColor(color);
    cell.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
    return cell;
}

void runLangtonsAnt(sf::RenderWindow& window, int speed, float cellSize) {
    const int oknoSzerokosc = window.getSize().x;
    const int oknoWysokosc = window.getSize().y;

    const int kolumny = oknoSzerokosc / cellSize;
    const int wiersze = oknoWysokosc / cellSize;

    Grid grid(wiersze, std::vector<char>(kolumny, false));
    Ant ant(kolumny / 2, wiersze / 2, Kierunek::N, kolumny, wiersze);

    bool paused = false;
    int krok = 0;

    sf::Clock clock;
    sf::Font font;
    if (!font.openFromFile("/Users/konradciemala/Library/Fonts/Oswald-VariableFont_wght.ttf")) return;

    InfoPanel infoPanel(window.getSize().x - 260.0f, 5.0f, font);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) paused = !paused;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    for (auto& row : grid) std::fill(row.begin(), row.end(), false);
                    ant = Ant(kolumny / 2, wiersze / 2, Kierunek::N, kolumny, wiersze);
                    krok = 0;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) speed = std::max(1, speed - 1);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) speed = std::min(1000, speed + 1);
            }
        }

        if (!paused) {
            ant.wykonajRuch(grid);
            krok++;
        }

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

        for (int y = 0; y < wiersze; ++y) {
            for (int x = 0; x < kolumny; ++x) {
                if (grid[y][x]) {
                    window.draw(makeCell1(x, y, sf::Color::Black, cellSize));
                }
            }
        }

        window.draw(makeCell1(ant.pobierzX(), ant.pobierzY(), sf::Color::Red, cellSize));
        infoPanel.update(0, krok);
        infoPanel.draw(window);
        window.display();
        sf::sleep(sf::milliseconds(speed * 10));
    }
}
