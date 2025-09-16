#include "Screen.hpp"

// ---------------- CONSTRUTOR ----------------
Screens::Screens() : current(ScreenState::MENU) {}

void Screens::change(ScreenState next) {
    current = next;
}

// ---------------- HELPERS ----------------
void Screens::drawCenteredText(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int x = (GetScreenWidth() - textWidth) / 2;
    DrawText(text, x, y, fontSize, color);
}

bool Screens::drawButton(Rectangle rect, const char* label, Color normal, Color hover, int fontSize) {
    Vector2 mouse = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mouse, rect);

    DrawRectangleRec(rect, isHover ? hover : normal);

    int textWidth = MeasureText(label, fontSize);
    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;

    DrawText(label, textX, textY, fontSize, BLACK);

    return isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// Cria um botão alinhado ao centro com base em índice (0,1,2,...)
Rectangle Screens::makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing) {
    return { (float)(centerX - btnWidth / 2), 
             (float)(startY + index * (btnHeight + spacing)), 
             (float)btnWidth, 
             (float)btnHeight };
}

// ---------------- TELAS ----------------
void Screens::drawMenuScreen() {
    ClearBackground(RAYWHITE);

    drawCenteredText("EcoTides", 80, 50, DARKBLUE);

    int centerX = GetScreenWidth() / 2;
    int startY = (GetScreenHeight() - (3 * 50 + 2 * 20)) / 2;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "START GAME", LIGHTGRAY, SKYBLUE)) change(ScreenState::GAME);
    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "OPTIONS", LIGHTGRAY, SKYBLUE)) change(ScreenState::OPTIONS);
    if (drawButton(makeButton(centerX, startY, 2, 200, 50, 20), "EXIT", LIGHTGRAY, SKYBLUE)) CloseWindow();
}

void Screens::drawGameScreen() {
    ClearBackground(RAYWHITE);

    int centerX = GetScreenWidth() / 2;
    int startY = (GetScreenHeight() - (3 * 50 + 2 * 20)) / 2;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "VICTORY", LIGHTGRAY, GREEN)) change(ScreenState::VICTORY);
    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "DEFEAT", LIGHTGRAY, RED)) change(ScreenState::DEFEAT);
    if (drawButton(makeButton(centerX, startY, 2, 200, 50, 20), "BACK", LIGHTGRAY, SKYBLUE)) change(ScreenState::MENU);
}

void Screens::drawOptionsScreen() {
    ClearBackground(LIGHTGRAY);

    drawCenteredText("OPTIONS MENU", 120, 40, DARKPURPLE);

    int centerX = GetScreenWidth() / 2;
    int startY = GetScreenHeight() / 2;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "BACK", RAYWHITE, SKYBLUE)) change(ScreenState::MENU);
}

void Screens::drawVictoryScreen() {
    ClearBackground(GREEN);
    drawCenteredText("YOU WIN!", 200, 40, WHITE);

    int centerX = GetScreenWidth() / 2;
    int startY = 300;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "MENU", LIGHTGRAY, SKYBLUE)) change(ScreenState::MENU);
    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "RETRY", LIGHTGRAY, SKYBLUE)) change(ScreenState::GAME);
}

void Screens::drawDefeatScreen() {
    ClearBackground(RED);
    drawCenteredText("YOU LOSE!", 200, 40, WHITE);

    int centerX = GetScreenWidth() / 2;
    int startY = 300;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "MENU", LIGHTGRAY, SKYBLUE)) change(ScreenState::MENU);
    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "RETRY", LIGHTGRAY, SKYBLUE)) change(ScreenState::GAME);
}

// ---------------- UPDATE ----------------
void Screens::update(float delta) {
    // Por enquanto a lógica de input está embutida nos botões da renderização.
    // Se quiser lógica separada, moveria parte disso pra cá.
    (void)delta; // evitar warning
}

// ---------------- RENDER ----------------
void Screens::render() {
    switch (current) {
        case ScreenState::MENU:    drawMenuScreen();    break;
        case ScreenState::GAME:    drawGameScreen();    break;
        case ScreenState::OPTIONS: drawOptionsScreen(); break;
        case ScreenState::VICTORY: drawVictoryScreen(); break;
        case ScreenState::DEFEAT:  drawDefeatScreen();  break;
    }
}

