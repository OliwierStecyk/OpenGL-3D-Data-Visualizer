# 3D Data Visualizer (OpenGL)

---
## 📋 Opis projektu
Aplikacja służąca do wizualizacji zbiorów danych w przestrzeni 3D. Projekt realizowany w ramach przedmiotu Grafika Komputerowa. Program pozwala na generowanie histogramów oraz wykresów słupkowych 3D na podstawie wczytanych plików danych.

---
## 🚀 Funkcje
- **Wizualizacja 3D:** Renderowanie danych w formie słupków 3D.
- **Wczytywanie danych:** Możliwość wyboru pliku z danymi przez użytkownika.
- **Interfejs:** Siatka (grid), znaczniki oraz opisy osi (X, Y, Z).
- **Analiza:** Automatyczne generowanie histogramów.


---
## 📸 Galeria

## 📸 Galeria

| Wizualizacja Słupkowa | Wykres Liniowy (Serie) | Chmura Punktów |
| :---: | :---: | :---: |
| ![Bars](images/bar.png) | ![Lines](images/line.png) | ![Scatter](images/scatter.png) |



---
## ⚙️ Technologie i Matematyka
Projekt wykorzystuje nowoczesny potok programowania karty graficznej (Programmable Pipeline):
*   **Shaders (GLSL):** Autorskie shadery wierzchołków i fragmentów obsługujące model oświetlenia Phonga (Ambient, Diffuse, Specular).
*   **GLM (OpenGL Mathematics):** Zaawansowane operacje na macierzach transformacji (Model-View-Projection).
*   **Buforowanie VBO/VAO:** Wysoka wydajność dzięki przetrzymywaniu geometrii bezpośrednio w pamięci karty graficznej.
*   **Dynamic File Parsing:** System inteligentnego wczytywania plików tekstowych obsługujący niestandardowe separatory i etykiety.


---
## 📂 Struktura plików
- `main.cpp` - Główna pętla programu i inicjalizacja.
- `shadersLoader.cpp/h` - Obsługa i kompilacja shaderów GLSL.
- `tekstura.cpp/h` - Zarządzanie teksturami i materiałami.
- `*.glsl` - Kody źródłowe shaderów wierzchołków i fragmentów.

---
## 🎮 Sterowanie

| Klawisz / Mysz | Akcja |
| :--- | :--- |
| **LPM + Ruch** | Obracanie sceny (Orbit) |
| **Scroll / PPM** | Przybliżanie i oddalanie (Zoom) |
| **Klawisz N** | Załadowanie następnego pliku z danymi |
| **Klawisz H** | Przełączanie trybu (Słupki / Linie / Punkty) |
| **ESC** | Wyjście z aplikacji |

---
## 🚀 Szybki Start

### Wymagania
*   Visual Studio 2019/2022 (Rekomendowane)
*   Biblioteki: GLEW, FreeGLUT / GLFW
*   Zestaw danych w formacie `.txt` (Przykłady w folderze `/data`)

---
### Instalacja
1. Sklonuj repozytorium: `git clone https://github.com/twoj-login/3d-data-visualizer.git`
2. Otwórz plik `.sln` w Visual Studio.
3. Upewnij się, że biblioteki są poprawnie podpięte w ustawieniach projektu (Linker/Include).
4. Skompiluj i uruchom (F5).

---

## 👥 Autorzy
- [Oliwier Stecyk](https://github.com/OliwierStecyk)
- [Jakub Smaga](https://github.com/Heimerin) 
