#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include "infoPanel.hpp" 

using Grid = std::vector<std::vector<bool>>;  // false = białe, true = czarne

enum Kierunek {N, E, S, W};

sf::RectangleShape makeCell1(int x, int y, sf::Color color, float cellSize) {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setFillColor(color);
    cell.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
    return cell;
}

void runLangtonsAnt(sf::RenderWindow& window, int speed, float cellSize) {
    const int oknoSzerokosc = window.getSize().x;
    const int oknoWysokosc = window.getSize().y;

    const int liczbaKolumn = oknoSzerokosc / cellSize;
    const int liczbaWierszy = oknoWysokosc / cellSize;

    Grid grid(liczbaWierszy, std::vector<bool>(liczbaKolumn, false));  // początkowo białe pola

    // Pozycja mrówki na środku planszy
    int mrówkaX = liczbaKolumn / 2;
    int mrówkaY = liczbaWierszy / 2;
    Kierunek kierunek = N;

    bool paused = false;
    int krok = 0;

    sf::Clock clock;
    sf::Font font;

    if (!font.openFromFile("/Users/konradciemala/Library/Fonts/Oswald-VariableFont_wght.ttf")) {
        return;
    }
    InfoPanel infoPanel(window.getSize().x - 260.0f, 5.0f, font);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) paused = !paused;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    // Reset planszy i mrówki
                    for (auto& wiersz : grid)
                        std::fill(wiersz.begin(), wiersz.end(), false);
                    mrówkaX = liczbaKolumn / 2;
                    mrówkaY = liczbaWierszy / 2;
                    kierunek = N;
                    krok = 0;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) speed = std::max(1, speed - 1);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) speed = std::min(1000, speed + 1);
            }
        }

        if (!paused) {
            // Stan pola pod mrówką
            bool obecnePole = grid[mrówkaY][mrówkaX];

            // Obrót i zmiana pola
            if (!obecnePole) { 
                // Białe pole => obróć w prawo
                kierunek = static_cast<Kierunek>((kierunek + 1) % 4);
                grid[mrówkaY][mrówkaX] = true;  // zmień na czarne
            } else {
                // Czarne pole => obróć w lewo
                kierunek = static_cast<Kierunek>((kierunek + 3) % 4);  // +3 mod 4 == -1 mod 4
                grid[mrówkaY][mrówkaX] = false;  // zmień na białe
            }

            // Ruch mrówki do przodu
            switch (kierunek) {
                case N: mrówkaY = (mrówkaY - 1 + liczbaWierszy) % liczbaWierszy; break;
                case E: mrówkaX = (mrówkaX + 1) % liczbaKolumn; break;
                case S: mrówkaY = (mrówkaY + 1) % liczbaWierszy; break;
                case W: mrówkaX = (mrówkaX - 1 + liczbaKolumn) % liczbaKolumn; break;
            }

            krok++;
        }

        window.clear(sf::Color::White);

        // Rysuj siatkę
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

        // Rysuj pola
        for (int y = 0; y < liczbaWierszy; ++y) {
            for (int x = 0; x < liczbaKolumn; ++x) {
                if (grid[y][x]) {
                    auto rect = makeCell1(x, y, sf::Color::Black, cellSize);
                    window.draw(rect);
                }
            }
        }

        // Rysuj mrówkę jako czerwone pole
        sf::RectangleShape antCell = makeCell1(mrówkaX, mrówkaY, sf::Color::Red, cellSize);
        window.draw(antCell);


        infoPanel.update(0, krok);  // nie liczymy żywych, pokazujemy tylko krok
        infoPanel.draw(window);

        window.display();
        sf::sleep(sf::milliseconds(speed * 10));
    }
}
