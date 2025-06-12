#include "ant.hpp"

Ant::Ant(int x, int y, Kierunek kierunek, int maxX, int maxY)
    : x(x), y(y), kierunek(kierunek), maxX(maxX), maxY(maxY) {}

void Ant::obrocPrawo() {
    kierunek = static_cast<Kierunek>((static_cast<int>(kierunek) + 1) % 4);
}

void Ant::obrocLewo() {
    kierunek = static_cast<Kierunek>((static_cast<int>(kierunek) + 3) % 4); // -1 mod 4
}

void Ant::idzDoPrzodu() {
    switch (kierunek) {
        case Kierunek::N: y = (y - 1 + maxY) % maxY; break;
        case Kierunek::E: x = (x + 1) % maxX; break;
        case Kierunek::S: y = (y + 1) % maxY; break;
        case Kierunek::W: x = (x - 1 + maxX) % maxX; break;
    }
}

void Ant::wykonajRuch(std::vector<std::vector<char>>& siatka) {
    if (!siatka[y][x]) {
        obrocPrawo();
        siatka[y][x] = true;
    } else {
        obrocLewo();
        siatka[y][x] = false;
    }
    idzDoPrzodu();
}

int Ant::pobierzX() const { return x; }
int Ant::pobierzY() const { return y; }
