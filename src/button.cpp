// Button.cpp
#include "button.hpp"

Button::Button(const std::string& label, sf::Vector2f position, sf::Vector2f size, sf::Font& font)
: text(font, label, 30)
{
    rect.setSize(size);
    rect.setFillColor(sf::Color::White);
    rect.setPosition(position);

    text.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2.f, (textBounds.size.y / 2.f) + 6.f});
    text.setPosition(position + size / 2.f);
}

bool Button::isClicked(sf::Vector2f mousePos) {
    return rect.getGlobalBounds().contains(mousePos);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(rect);
    window.draw(text);
}
