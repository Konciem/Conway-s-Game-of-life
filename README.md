# Conway-s-Game-of-life

:potted_plant: Conway's game of life w c++ z uzyciem SFML 3.0.0 :potted_plant:

## 📖 Opis

**Gra w życie** to automat komórkowy stworzony przez Johna Conwaya. Jest to gra zeroosobowa – po ustaleniu warunków początkowych (czyli logiki gry i konfiguracji komórek), symulacja przebiega automatycznie bez dalszej ingerencji użytkownika.

W symulacji każda komórka na planszy może być:
- **żywa** (kolor biały),
- **martwa** (kolor czarny).

Stan każdej komórki zależy od jej ośmiu sąsiadów (tzw. sąsiedztwo Moore’a – komórki otaczające daną w pionie, poziomie i na ukos).

## 🧠 Logika gry

- Żywa komórka umiera, jeśli ma **mniej niż 2 sąsiadów** (samotność).
- Żywa komórka umiera, jeśli ma **więcej niż 3 sąsiadów** (przeludnienie).
- Martwa komórka ożywa, jeśli ma **dokładnie 3 sąsiadów** (rozmnażanie).

![Image](https://github.com/user-attachments/assets/da2b2889-cc1c-4da6-987a-dd0b5ef2f67a)

---

## 🐜 Langton's Ant

Dodatkowo planowana jest implementacja drugiego automatu komórkowego – **Mrówki Langtona**. W tym automacie wyróżniona jest jedna komórka, nazwana „mrówką”, która oprócz koloru ma określony **kierunek poruszania się**. Mrówka zachowuje się według prostych zasad:

- ➡️ **Na białym polu**: mrówka obraca się w **prawo (90°)**, zmienia kolor pola na czarny, przechodzi na kolejną komórkę.
- ⬅️ **Na czarnym polu**: mrówka obraca się w **lewo (90°)**, zmienia kolor pola na biały, przechodzi na kolejną komórkę.

Mrówka porusza się po nieskończonej planszy podzielonej na pola w dwóch kolorach: białym i czarnym.

---

## 🖥️ Interfejs użytkownika

Po uruchomieniu programu użytkownik zobaczy okno startowe. Aby kontynuować, należy kliknąć dowolny przycisk.

W kolejnym etapie dostępne będą ustawienia:

### 🔧 Ustawienia startowe:
- **Tryb symulacji**:
  - Conway’s Game of Life
  - Langton’s Ant
- **Rozmiar komórek**:
  - Małe
  - Średnie
  - Duże  
  *(okno symulacji ma stałe wymiary 1000x1000 px)*
- **Szybkość odświeżania**: możliwość ręcznego ustawienia czasu w milisekundach.

### ⌨️ Sterowanie w trakcie symulacji:
- **Spacja** – pauza/wznowienie symulacji.
- **Prawy przycisk myszy** – kliknięcie zmienia stan danej komórki.
- Panel w prawym górnym rogu wyświetlający:
  - Liczbę żywych komórek („mieszkańców”)
  - Czas trwania symulacji (liczba kroków/odświeżeń)

---