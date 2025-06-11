#include "infoPanel.hpp"
#include <sstream>

InfoPanel::InfoPanel(float x, float y, sf::Font& font)
    : text(font,"", 20) // Initialize sf::Text with empty string, font, and size
{
    background.setSize(sf::Vector2f(190.0f, 80.0f));
    background.setFillColor(sf::Color::White);
    background.setPosition(sf::Vector2f(x + 5, y + 5));

    border.setSize(sf::Vector2f(200.0f, 90.0f));
    border.setFillColor(sf::Color::Black);
    border.setPosition(sf::Vector2f(x, y));

    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Underlined);
    text.setPosition(sf::Vector2f(x + 10, y + 13));

}

void InfoPanel::update(int aliveCount, int steps) {
    std::ostringstream ss;
    ss << "Zywe komrki: " << aliveCount << "\n";
    ss << "Kroki: " << steps;
    text.setString(ss.str());
}

void InfoPanel::draw(sf::RenderWindow& window) {
    window.draw(border);
    window.draw(background);
    window.draw(text);
}
