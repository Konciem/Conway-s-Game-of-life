#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <regex>
#include "langton.hpp"
#include "conway.hpp"




enum class AppState {
    MENU,
    SIZE_SELECTION,
    SPEED_INPUT,
    SIMULATION
};

enum class SimulationType {
    NONE,
    LANGTON,
    GAME_OF_LIFE
};

const sf::Vector2u WINDOW_SIZE(1000, 1000);

class Button {
public:
    sf::RectangleShape rect;
    sf::Text text;

    Button(const std::string& label, sf::Vector2f position, sf::Vector2f size, sf::Font& font)
    : text(font,label, 24)
    {
    rect.setSize(size);
    rect.setFillColor(sf::Color::White);
    rect.setPosition(position);

    text.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
    text.setPosition(position + size / 2.f);
    }


    bool isClicked(sf::Vector2f mousePos) {
        return rect.getGlobalBounds().contains(mousePos);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(rect);
        window.draw(text);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_SIZE.x, WINDOW_SIZE.y}), "Symulacje", sf::Style::Close);
    sf::Font font;
    if (!font.openFromFile("/Users/konradciemala/Library/Fonts/Oswald-VariableFont_wght.ttf")) {
        return 1;
    }

    AppState appState = AppState::MENU;
    SimulationType simType = SimulationType::NONE;
    std::string speedInput = "";
    std::regex liczba_regex("^[0-9]+$"); // Regex to match numeric input

    Button langtonButton("Langton's Ant", { 350, 300 }, { 300, 80 }, font);
    Button lifeButton("Game of Life", { 350, 400 }, { 300, 80 }, font);

    Button smallBtn("Mala", { 350, 250 }, { 300, 70 }, font);
    Button mediumBtn("Srednia", { 350, 350 }, { 300, 70 }, font);
    Button largeBtn("Duza", { 350, 450 }, { 300, 70 }, font);

    sf::Text titleText(font, "Wybierz symulacje:", 30);
    sf::FloatRect titleTextBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleTextBounds.size.x / 2.f, titleTextBounds.size.y/ 2.f});
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({window.getSize().x / 2.0f, 200});

    sf::Text sizeText(font, "Wybierz rozmiar planszy:", 30);
    sizeText.setFillColor(sf::Color::White);
    sf::FloatRect sizeTextBounds = sizeText.getLocalBounds();
    sizeText.setOrigin({sizeTextBounds.size.x / 2.f, sizeTextBounds.size.y / 2.f});
    sizeText.setPosition({window.getSize().x / 2.0f, 200});   

    sf::Text speedText(font, "Podaj predkosc (ms):", 30);
    speedText.setFillColor(sf::Color::White);
    sf::FloatRect speedTextBounds = speedText.getLocalBounds();
    speedText.setOrigin({speedTextBounds.size.x / 2.f, speedTextBounds.size.y / 2.f});
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition({window.getSize().x / 2.0f, 300});

    sf::Text inputText(font, "", 24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition({350, 350});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (appState == AppState::MENU) {
                    if (langtonButton.isClicked(mousePos)) {
                        simType = SimulationType::LANGTON;
                        appState = AppState::SIZE_SELECTION;
                    } else if (lifeButton.isClicked(mousePos)) {
                        simType = SimulationType::GAME_OF_LIFE;
                        appState = AppState::SIZE_SELECTION;
                    }
                } else if (appState == AppState::SIZE_SELECTION) {
                    if (smallBtn.isClicked(mousePos) || mediumBtn.isClicked(mousePos) || largeBtn.isClicked(mousePos)) {
                        appState = AppState::SPEED_INPUT;
                    }
                }
            }

            if (appState == AppState::SPEED_INPUT) {
                if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                    if (textEntered->unicode < 128)
                        std::cout << "ASCII character typed: " << static_cast<char>(textEntered->unicode) << std::endl;
                    if (textEntered->unicode == '\b' && !speedInput.empty()) {
                        speedInput.pop_back();
                    } else if (std::isdigit(textEntered->unicode)) {
                        speedInput += static_cast<char>(textEntered->unicode);
                    }

                    if (std::regex_match(speedInput, liczba_regex)) {
                        int speed = std::stoi(speedInput);
                        if (speed > 0) {
                            appState = AppState::SIMULATION;
                            std::cout << "Uruchamianie symulacji z predkoscia: " << speed << " ms\n";
                        } else {
                            std::cout << "Podaj liczbę większą od zera!\n";
                            speedInput.clear();
                            inputText.setString("Błąd: > 0");
                        }
                    }

                    inputText.setString(speedInput);
                    
                }
              
                    
            }
        }

        
        window.clear(sf::Color::Black);


        switch (appState) {
            case AppState::MENU:
                window.draw(titleText);
                langtonButton.draw(window);
                lifeButton.draw(window);
                break;
            case AppState::SIZE_SELECTION:
                window.draw(sizeText);
                smallBtn.draw(window);
                mediumBtn.draw(window);
                largeBtn.draw(window);
                break;
            case AppState::SPEED_INPUT:
                window.draw(speedText);
                window.draw(inputText);
                break;
            case AppState::SIMULATION:
                if (simType == SimulationType::LANGTON) {
                    int speed = std::stoi(speedInput);  
                    runLangtonsAnt(window, speed);
                    window.close();
                } else if (simType == SimulationType::GAME_OF_LIFE) {
                    int speed = std::stoi(speedInput);  
                    runConway(window, speed);
                    window.close();
                }
                break;
        }

         // Call the simulation function here
        window.display();
        
    }

    return 0;
}
