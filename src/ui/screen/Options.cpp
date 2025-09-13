#include "Options.hpp"
#include "../../core/game/World.hpp"
#include <raylib-cpp.hpp>

void Options::draw(World &world) {
    // Centralizar texto de opções
    const char* optionsText = "Tela de Opções";
    int fontSize = 30;
    int screenWidth = GetScreenWidth();
    int textWidth = MeasureText(optionsText, fontSize);
    int textX = (screenWidth - textWidth) / 2;
    int textY = 280;
    DrawText(optionsText, textX, textY, fontSize, ORANGE);

    // Centralizar instrução
    const char* instrText = "Pressione ENTER para voltar";
    int instrFontSize = 20;
    int instrWidth = MeasureText(instrText, instrFontSize);
    int instrX = (screenWidth - instrWidth) / 2;
    int instrY = textY + fontSize + 20;
    DrawText(instrText, instrX, instrY, instrFontSize, DARKGRAY);

    if (IsKeyPressed(KEY_ENTER))
        world.currentScreen = GameScreen::MENU;
}