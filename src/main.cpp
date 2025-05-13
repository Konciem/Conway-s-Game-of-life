#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include "langton.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1000u, 1000u}), "GameofLife");

    sf::Font font;
    if (!font.openFromFile("/Users/konradciemala/Library/Fonts/Monocraft.ttc")) {
        return 1;
    }

    sf::Text text(font,"Conway's Game of life", 40);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y/ 2.f});
    text.setPosition({window.getSize().x / 2.0f, window.getSize().y / 2.0f -50.f});
    text.setFillColor(sf::Color::White);

    sf::RectangleShape button({150.f,50.f});
    sf::FloatRect bd = button.getLocalBounds();
    button.setOrigin({bd.size.x / 2.f, bd.size.y/ 2.f});
    button.setPosition({window.getSize().x / 2.0f, window.getSize().y / 2.0f +50.f});
    
    sf::Text text1(font,"START", 40);
    sf::FloatRect bounds1 = text1.getLocalBounds();
    text1.setOrigin({bounds1.size.x / 2.f, bounds1.size.y/ 2.f});
    text1.setPosition({window.getSize().x / 2.0f, (window.getSize().y / 2.0f) + bd.size.y/2.f});
    text1.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    //Tu beddzie odpalany langton
                    runLangtonsAnt(window);

                    
                    }
                }

        window.clear();
        window.draw(text);
        window.draw(button);
        window.draw(text1);
        window.display();
    }

    return 0;
}


