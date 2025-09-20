#pragma once
#include "utils/enum.hpp"
#include <raylib-cpp.hpp>

class Screens {
private:
    ScreenState current;
    bool showConfirmPopup; // controla popup do botão de sair

    // Botões do popup
    Rectangle btnContinue;
    Rectangle btnMenu;

public:
    Screens();

    void change(ScreenState next);
    ScreenState getCurrent() const;

    void update(float delta);
    void render();

    bool isPopupActive() const { return showConfirmPopup; }

    // helpers
    void drawCenteredText(const char* text, int y, int fontSize, Color color);
    bool drawButton(Rectangle rect, const char* label, Color normal, Color hover, int fontSize = 20);
    Rectangle makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing);

    // telas específicas
    void drawMenuScreen();
    void drawGameScreen();
    void drawOptionsScreen();
    void drawVictoryScreen();
    void drawDefeatScreen();
};
