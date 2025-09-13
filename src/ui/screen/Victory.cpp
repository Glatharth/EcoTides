#include "Victory.hpp"
#include <raylib-cpp.hpp>

void Victory::draw(World &world) {
    // Centralizar texto de vitória
    const char* victoryText = "Você Venceu!";
    int fontSize = 30;
    int screenWidth = GetScreenWidth();
    int textWidth = MeasureText(victoryText, fontSize);
    int textX = (screenWidth - textWidth) / 2;
    int textY = 280;

    DrawText(victoryText, textX, textY, fontSize, GREEN);

    // Centralizar botão
    int btnWidth = 200;
    int btnHeight = 50;
    int x = (screenWidth - btnWidth) / 2;
    int y = 350;

    Rectangle btnMenu = { (float)x, (float)y, (float)btnWidth, (float)btnHeight };
    bool hoverMenu = CheckCollisionPointRec(GetMousePosition(), btnMenu);
    DrawRectangleRec(btnMenu, hoverMenu ? LIGHTGRAY : GRAY);

    const char* btnText = "Voltar ao Menu";
    int btnTextWidth = MeasureText(btnText, 20);
    int btnTextX = x + (btnWidth - btnTextWidth) / 2;
    int btnTextY = y + (btnHeight - 20) / 2;

    DrawText(btnText, btnTextX, btnTextY, 20, BLACK);

    if (hoverMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        world.currentScreen = GameScreen::MENU;
    }
}