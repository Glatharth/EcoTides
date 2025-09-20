#ifndef SCREENS_HPP
#define SCREENS_HPP

#include <raylib-cpp.hpp>
#include "utils/enum.hpp"

// Classe responsável por gerenciar todas as telas do jogo.
class Screens {
public:
    Screens();
    ~Screens() = default;

    void update(float delta);
    void render();
    void change(ScreenState next);

        // Função nova para obter a tela atual
    ScreenState getCurrent() const { return current; }

private:
    ScreenState current;  // Tela atual

    // Helpers de desenho
    void drawCenteredText(const char* text, int y, int fontSize, Color color);
    bool drawButton(Rectangle rect, const char* label, Color normal, Color hover, int fontSize = 20);

    // Helpers de layout
    Rectangle makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing);

    // Telas
    void drawMenuScreen();
    void drawGameScreen();
    void drawOptionsScreen();
    void drawVictoryScreen();
    void drawDefeatScreen();
};

#endif // SCREENS_HPP

