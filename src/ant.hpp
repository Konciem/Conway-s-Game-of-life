#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

enum class Kierunek { N, E, S, W };

class Ant {
public:
    Ant(int x, int y, Kierunek kierunek, int maxX, int maxY);

    void wykonajRuch(std::vector<std::vector<char>>& siatka);
    int pobierzX() const;
    int pobierzY() const;

private:
    int x, y;
    int maxX, maxY;
    Kierunek kierunek;

    void obrocPrawo();
    void obrocLewo();
    void idzDoPrzodu();
};
