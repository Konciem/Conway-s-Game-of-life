#pragma once
#include <SFML/Graphics.hpp>

class Cell {
public:
    Cell(float x, float y, float size);

    void setAlive(bool alive);
    bool isAlive() const;

    void draw(sf::RenderWindow& window) const;

private:
    bool alive;
    sf::RectangleShape shape;
};
