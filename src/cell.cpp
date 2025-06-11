#include "cell.hpp"

Cell::Cell(float x, float y, float size) : alive(false), shape(sf::Vector2f(size, size)) {
    shape.setPosition(sf::Vector2f(x, y));
    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color(200, 200, 200));
    shape.setOutlineThickness(1);
}

void Cell::setAlive(bool alive) {
    this->alive = alive;
    shape.setFillColor(alive ? sf::Color::Black : sf::Color::White);
}

bool Cell::isAlive() const {
    return alive;
}

void Cell::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}
