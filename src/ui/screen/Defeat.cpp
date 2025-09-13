#include "Defeat.hpp"
#include <raylib-cpp.hpp>

void Defeat::draw(World &world) {
    // Centralizar texto de derrota
    const char* defeatText = "Você Perdeu!";
    int fontSize = 30;
    int screenWidth = GetScreenWidth();
    int textWidth = MeasureText(defeatText, fontSize);
    int textX = (screenWidth - textWidth) / 2;
    int textY = 280;

    DrawText(defeatText, textX, textY, fontSize, RED);

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