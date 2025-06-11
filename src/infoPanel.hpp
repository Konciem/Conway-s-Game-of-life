#pragma once
#include <SFML/Graphics.hpp>

class InfoPanel {
public:
    InfoPanel(float x, float y, sf::Font& font);

    void update(int aliveCount, int steps);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape background;
    sf::RectangleShape border;
    sf::Text text;
};
