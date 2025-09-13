#include "Game.hpp"
#include "../../core/game/World.hpp"
#include <raylib-cpp.hpp>

void GameScreenClass::draw(World &world) {
    // Centralizar texto do jogo
    const char* gameText = "Tela do Jogo!";
    int fontSize = 30;
    int screenWidth = GetScreenWidth();
    int textWidth = MeasureText(gameText, fontSize);
    int textX = (screenWidth - textWidth) / 2;
    int textY = 280;
    DrawText(gameText, textX, textY, fontSize, GREEN);

    // Centralizar instrução
    const char* instrText = "Pressione ENTER para voltar";
    int instrFontSize = 20;
    int instrWidth = MeasureText(instrText, instrFontSize);
    int instrX = (screenWidth - instrWidth) / 2;
    int instrY = textY + fontSize + 20;
    DrawText(instrText, instrX, instrY, instrFontSize, DARKGRAY);

    // Centralizar botões Victory e Defeat
    int btnWidth = 150;
    int btnHeight = 40;
    int spacing = 40; // Espaço entre os botões

    int totalBtnWidth = btnWidth * 2 + spacing;
    int btnStartX = (screenWidth - totalBtnWidth) / 2;
    int btnY = instrY + instrFontSize + 40;

    // Botão Victory
    Rectangle btnVictory = { (float)btnStartX, (float)btnY, (float)btnWidth, (float)btnHeight };
    bool hoverVictory = CheckCollisionPointRec(GetMousePosition(), btnVictory);
    DrawRectangleRec(btnVictory, hoverVictory ? LIGHTGRAY : GRAY);

    const char* victoryText = "Vitória";
    int victoryTextWidth = MeasureText(victoryText, 20);
    int victoryTextX = btnStartX + (btnWidth - victoryTextWidth) / 2;
    int victoryTextY = btnY + (btnHeight - 20) / 2;
    DrawText(victoryText, victoryTextX, victoryTextY, 20, BLACK);

    if (hoverVictory && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        world.currentScreen = GameScreen::VICTORY;
    }

    // Botão Derrota
    int btnDefeatX = btnStartX + btnWidth + spacing;
    Rectangle btnDefeat = { (float)btnDefeatX, (float)btnY, (float)btnWidth, (float)btnHeight };
    bool hoverDefeat = CheckCollisionPointRec(GetMousePosition(), btnDefeat);
    DrawRectangleRec(btnDefeat, hoverDefeat ? LIGHTGRAY : GRAY);

    const char* defeatText = "Derrota";
    int defeatTextWidth = MeasureText(defeatText, 20);
    int defeatTextX = btnDefeatX + (btnWidth - defeatTextWidth) / 2;
    int defeatTextY = btnY + (btnHeight - 20) / 2;
    DrawText(defeatText, defeatTextX, defeatTextY, 20, BLACK);

    if (hoverDefeat && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        world.currentScreen = GameScreen::DEFEAT;
    }

    if (IsKeyPressed(KEY_ENTER))
        world.currentScreen = GameScreen::MENU;
}