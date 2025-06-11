#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <regex>
#include <fstream>

#include "langton.hpp"
#include "conway.hpp"
#include "button.hpp"
#include "gameState.hpp"

enum class AppState {
    START,
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

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_SIZE.x, WINDOW_SIZE.y}), "Symulacje", sf::Style::Close);
    sf::Font font;

    sf::Texture tloTekstura;
    if (!tloTekstura.loadFromFile("/Users/konradciemala/Documents/Conway-s-Game-of-life/puls.png")) {
        std::cerr << "Błąd ładowania tła\n";
        return 1;
    }
    sf::Sprite tloSprite(tloTekstura);

    sf::Vector2f scale = tloSprite.getScale();
    tloSprite.setScale({
        static_cast<float>(window.getSize().x) / tloTekstura.getSize().x,
        static_cast<float>(window.getSize().y) / tloTekstura.getSize().y}
    );

    if (!font.openFromFile("/Users/konradciemala/Library/Fonts/Doto-VariableFont_ROND,wght.ttf")) {
        return 1;
    }
    font.setSmooth(false);

    float cellSize; 

    AppState appState = AppState::START;
    SimulationType simType = SimulationType::NONE;
    std::string speedInput = "";
    std::regex liczba_regex("^[0-9]+$"); //regex

    Button startButton("Start", { 350, 400 }, { 300, 80 }, font);
    Button loadButton("Wczytaj zapis", { 350, 500 }, { 300, 80 }, font);


    Button langtonButton("Langton's Ant", { 350, 300 }, { 300, 80 }, font);
    Button lifeButton("Game of Life", { 350, 400 }, { 300, 80 }, font);

    Button smallBtn("Mala", { 350, 250 }, { 300, 70 }, font);
    Button mediumBtn("Srednia", { 350, 350 }, { 300, 70 }, font);
    Button largeBtn("Duza", { 350, 450 }, { 300, 70 }, font);

    Button saveButton("Zapisz stan", { 20, 20 }, { 200, 50 }, font);

    sf::Text startTitle(font, "CONWAY'S GAME OF LIFE", 70);
    startTitle.setFillColor(sf::Color::White);
    sf::FloatRect startTitleBounds = startTitle.getLocalBounds();
    startTitle.setOrigin({startTitleBounds.size.x / 2.f, startTitleBounds.size.y / 2.f});
    startTitle.setPosition({window.getSize().x / 2.f, 300});

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
                if (appState == AppState::START) {
                    if (startButton.isClicked(mousePos)) {
                        appState = AppState::MENU;
                    }
                    else if (loadButton.isClicked(mousePos)) {
                        Grid grid;
                        int krok = 0;
                        if (loadFromFile(grid, krok, "save.txt")) {
                            appState = AppState::SIMULATION;
                        } else {
                            std::cout << "blad wczytywania";
                        }
                    }
                } else if (appState == AppState::MENU) {
                    if (langtonButton.isClicked(mousePos)) {
                        simType = SimulationType::LANGTON;
                        appState = AppState::SIZE_SELECTION;
                    } else if (lifeButton.isClicked(mousePos)) {
                        simType = SimulationType::GAME_OF_LIFE;
                        appState = AppState::SIZE_SELECTION;
                    }
                } else if (appState == AppState::SIZE_SELECTION) {
                    if (smallBtn.isClicked(mousePos)) {
                        cellSize = 25.f;
                        appState = AppState::SPEED_INPUT;
                    } else if (mediumBtn.isClicked(mousePos)) {
                        cellSize = 20.f; 
                        appState = AppState::SPEED_INPUT;
                    } else if (largeBtn.isClicked(mousePos)) {
                        cellSize = 10.f; 
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

                    if (std::regex_match(speedInput, liczba_regex)) { //regex
                        int speed = std::stoi(speedInput);
                        if (speed > 0) {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                                appState = AppState::SIMULATION;
                            }
                            
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
            case AppState::START:
                window.draw(tloSprite);
                window.draw(startTitle);
                startButton.draw(window);
                loadButton.draw(window);
                break;
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
                    runLangtonsAnt(window, speed, cellSize);
                    window.close();
                } else if (simType == SimulationType::GAME_OF_LIFE) {
                    int speed = std::stoi(speedInput);  
                    
                    runConway(window, speed, cellSize);
                    saveButton.draw(window);
                    window.close();
                }
                break;
        }
        window.display();
    }

    return 0;
}
