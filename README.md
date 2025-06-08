# Conway-s-Game-of-life

:potted_plant: Conway's game of life w c++ z uzyciem SFML 3.0.0 :potted_plant:

## 📖 Opis

**Gra w życie** to automat komórkowy stworzony przez Johna Conwaya. Jest to gra zeroosobowa – po ustaleniu warunków początkowych (czyli logiki gry i konfiguracji komórek), symulacja przebiega automatycznie bez dalszej ingerencji użytkownika.

W symulacji każda komórka na planszy może być:
- **żywa** (kolor biały),
- **martwa** (kolor czarny).

Stan każdej komórki zależy od jej ośmiu sąsiadów (tzw. sąsiedztwo Moore’a – komórki otaczające daną w pionie, poziomie i na ukos).

## 🧠 Logika gry

- 🔹 Żywa komórka umiera, jeśli ma **mniej niż 2 sąsiadów** (samotność).
- 🔹 Żywa komórka umiera, jeśli ma **więcej niż 3 sąsiadów** (przeludnienie).
- 🔹 Martwa komórka ożywa, jeśli ma **dokładnie 3 sąsiadów** (rozmnażanie).

---