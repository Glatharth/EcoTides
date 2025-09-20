#include "Screens.hpp"
#include <cstdlib>
#include <raylib-cpp.hpp>

// ---------------- CONSTRUTOR ----------------
Screens::Screens() : current(ScreenState::MENU), showConfirmPopup(false) {
    int w = 300, h = 150;
    int x = GetScreenWidth()/2 - w/2;
    int y = GetScreenHeight()/2 - h/2;

    // definir botões relativos à caixa do popup
    btnContinue = { (float)(x + 30), (float)(y + 90), 100, 40 };
    btnMenu     = { (float)(x + w - 130), (float)(y + 90), 100, 40 };
}

void Screens::change(ScreenState next) {
    current = next;
}

ScreenState Screens::getCurrent() const {
    return current;
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

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "START GAME", LIGHTGRAY, SKYBLUE))
        change(ScreenState::GAME);

    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "OPTIONS", LIGHTGRAY, SKYBLUE))
        change(ScreenState::OPTIONS);

    if (drawButton(makeButton(centerX, startY, 2, 200, 50, 20), "EXIT", LIGHTGRAY, SKYBLUE))
        exit(0);
}

void Screens::drawGameScreen() {
    ClearBackground(RAYWHITE);
    // Nenhum botão aqui! Apenas o jogo e o popup se ativado
}

void Screens::drawOptionsScreen() {
    ClearBackground(LIGHTGRAY);
    drawCenteredText("OPTIONS MENU", 120, 40, DARKPURPLE);

    int centerX = GetScreenWidth() / 2;
    int startY = GetScreenHeight() / 2;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "BACK", RAYWHITE, SKYBLUE))
        change(ScreenState::MENU);
}

void Screens::drawVictoryScreen() {
    ClearBackground(GREEN);
    drawCenteredText("YOU WIN!", 200, 40, WHITE);

    int centerX = GetScreenWidth() / 2;
    int startY = 300;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "MENU", LIGHTGRAY, SKYBLUE))
        change(ScreenState::MENU);

    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "RETRY", LIGHTGRAY, SKYBLUE))
        change(ScreenState::GAME);
}

void Screens::drawDefeatScreen() {
    ClearBackground(RED);
    drawCenteredText("YOU LOSE!", 200, 40, WHITE);

    int centerX = GetScreenWidth() / 2;
    int startY = 300;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "MENU", LIGHTGRAY, SKYBLUE))
        change(ScreenState::MENU);

    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "RETRY", LIGHTGRAY, SKYBLUE))
        change(ScreenState::GAME);
}

// ---------------- UPDATE ----------------
void Screens::update(float delta) {
    if (showConfirmPopup) {
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, btnContinue)) showConfirmPopup = false;
            if (CheckCollisionPointRec(mouse, btnMenu)) {
                showConfirmPopup = false;
                change(ScreenState::MENU);
            }
        }
        return;
    }

    // tecla ESC ativa popup apenas na tela de jogo
    if (current == ScreenState::GAME && IsKeyPressed(KEY_ENTER))
        showConfirmPopup = true;
}

// ---------------- RENDER ----------------
void Screens::render() {
    switch (current) {
        case ScreenState::MENU: drawMenuScreen(); break;
        case ScreenState::GAME: drawGameScreen(); break;
        case ScreenState::VICTORY: drawVictoryScreen(); break;
        case ScreenState::DEFEAT: drawDefeatScreen(); break;
        case ScreenState::OPTIONS: drawOptionsScreen(); break;
    }

    // Desenha popup se ativo
    if (showConfirmPopup) {
        int w = 300, h = 150;
        int x = GetScreenWidth()/2 - w/2;
        int y = GetScreenHeight()/2 - h/2;

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        DrawRectangle(x, y, w, h, RAYWHITE);
        DrawRectangleLines(x, y, w, h, DARKGRAY);
        drawCenteredText("Deseja sair do jogo?", y + 30, 20, BLACK);

        Color c1 = CheckCollisionPointRec(GetMousePosition(), btnContinue) ? LIGHTGRAY : GRAY;
        Color c2 = CheckCollisionPointRec(GetMousePosition(), btnMenu) ? LIGHTGRAY : GRAY;

        DrawRectangleRec(btnContinue, c1);
        DrawRectangleLinesEx(btnContinue, 2, DARKGRAY);
        DrawText("Continuar", btnContinue.x + 10, btnContinue.y + 10, 18, BLACK);

        DrawRectangleRec(btnMenu, c2);
        DrawRectangleLinesEx(btnMenu, 2, DARKGRAY);
        DrawText("Menu", btnMenu.x + 25, btnMenu.y + 10, 18, BLACK);
    }
}
