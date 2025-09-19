#ifndef SCREENS_HPP
#define SCREENS_HPP

#include <raylib.h>
#include "utils/enum.hpp"

// Classe responsável por gerenciar todas as telas do jogo.
class Screens {
public:
    Screens();
    ~Screens() = default;

    // Atualiza a lógica da tela atual (inputs, cliques, etc.)
    void update(float delta);

    // Renderiza a tela atual (textos, botões, cores de fundo).
    void render();

    // Troca de estado/tela
    void change(ScreenState next);

private:
    ScreenState current;  // Tela atual

    // Helpers de desenho
    void drawCenteredText(const char* text, int y, int fontSize, Color color);
    bool drawButton(Rectangle rect, const char* label, Color normal, Color hover, int fontSize = 20);

    // Helpers de layout
    Rectangle makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing);
    void drawMenuScreen();
    void drawGameScreen();
    void drawOptionsScreen();
    void drawVictoryScreen();
    void drawDefeatScreen();
};

#endif // SCREENS_HPP
