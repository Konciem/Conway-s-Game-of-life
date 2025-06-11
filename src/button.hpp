// Button.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    sf::RectangleShape rect;
    sf::Text text;

    Button(const std::string& label, sf::Vector2f position, sf::Vector2f size, sf::Font& font);

    bool isClicked(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
};
