#include "Screens.hpp"
#include <cstdlib>
#include <raylib-cpp.hpp>
#include "core/game/World.hpp"

extern World* globalWorldInstance;

Screens::Screens() : current(ScreenState::MENU), showConfirmPopup(false), mouseDebounce(false) {
    int w = 300, h = 150;
    int x = GetScreenWidth()/2 - w/2;
    int y = GetScreenHeight()/2 - h/2;

    btnPopupContinue = { (float)(x + 30), (float)(y + 90), 100, 40 };
    btnPopupMenu     = { (float)(x + w - 130), (float)(y + 90), 100, 40 };

    btnVictoryMenu  = { (float)(x + 30), (float)(y + 200), 100, 40 };
    btnVictoryRetry = { (float)(x + w - 130), (float)(y + 200), 100, 40 };

    btnDefeatMenu  = { (float)(x + 30), (float)(y + 200), 100, 40 };
    btnDefeatRetry = { (float)(x + w - 130), (float)(y + 200), 100, 40 };

    backgroundImage = new raylib::Image("assets/background.png");
    backgroundTexture = backgroundImage->LoadTexture();
}

Screens::~Screens() {
    backgroundTexture.Unload();
    if (backgroundImage) {
        delete backgroundImage;
        backgroundImage = nullptr;
    }
}

void Screens::change(ScreenState next) { current = next; }
ScreenState Screens::getCurrent() const { return current; }

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
    int textX = rect.x + (rect.width - textWidth)/2;
    int textY = rect.y + (rect.height - fontSize)/2;
    DrawText(label, textX, textY, fontSize, BLACK);

    return isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

Rectangle Screens::makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing) {
    return { (float)(centerX - btnWidth / 2),
             (float)(startY + index * (btnHeight + spacing)),
             (float)btnWidth,
             (float)btnHeight };
}

void Screens::drawMenuScreen() {
    // ClearBackground(BLUE);
    drawCenteredText("EcoTides", 80, 50, DARKBLUE);

    int centerX = GetScreenWidth() / 2;
    int startY = (GetScreenHeight() - (2 * 50 + 1 * 20)) / 2;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "START GAME", LIGHTGRAY, SKYBLUE))
        change(ScreenState::GAME);

    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "EXIT", LIGHTGRAY, SKYBLUE))
        exit(0);
}


void Screens::drawGameScreen() {
}

void Screens::drawVictoryScreen() {
    ClearBackground(GREEN);
    drawCenteredText("YOU WIN!", 200, 40, WHITE);

    if (drawButton(btnVictoryMenu, "MENU", LIGHTGRAY, SKYBLUE))
        change(ScreenState::MENU);

    if (drawButton(btnVictoryRetry, "RETRY", LIGHTGRAY, SKYBLUE))
        if (globalWorldInstance) globalWorldInstance->retry();
}

void Screens::drawDefeatScreen() {
    ClearBackground(RED);
    drawCenteredText("YOU LOSE!", 200, 40, WHITE);

    if (drawButton(btnDefeatMenu, "MENU", LIGHTGRAY, SKYBLUE))
        change(ScreenState::MENU);

    if (drawButton(btnDefeatRetry, "RETRY", LIGHTGRAY, SKYBLUE))
        if (globalWorldInstance) globalWorldInstance->retry();
}

void Screens::update(float delta) {
    backgroundAnimTime += delta;
    if (backgroundAnimTime >= backgroundFrameDuration) {
        backgroundAnimTime = 0.0f;
        backgroundFrame = (backgroundFrame + 1) % backgroundFrameCount;
    }
    if (mouseDebounce) {
        if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            mouseDebounce = false;
        } else {
            return;
        }
    }

    if (showConfirmPopup) {
        Vector2 mouse = GetMousePosition();
if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

    if (CheckCollisionPointRec(mouse, btnPopupContinue)) {
        showConfirmPopup = false;
        if (globalWorldInstance)
            globalWorldInstance->retry();

        mouseDebounce = true;
    }



            if (CheckCollisionPointRec(mouse, btnPopupMenu)) {
                showConfirmPopup = false;
                change(ScreenState::MENU);

                mouseDebounce = true;
            }
        }
        return;
    }


    if (current == ScreenState::GAME && IsKeyPressed(KEY_ENTER))
        showConfirmPopup = true;
}


void Screens::render() {
    const raylib::Rectangle src = {
        static_cast<float>(backgroundFrame) * 75.0f,
        0,
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
    };
    backgroundTexture.Draw(src, Vector2{0,0}, WHITE);
    switch (current) {
        case ScreenState::MENU: drawMenuScreen(); break;
        case ScreenState::GAME: drawGameScreen(); break;
        case ScreenState::VICTORY: drawVictoryScreen(); break;
        case ScreenState::DEFEAT: drawDefeatScreen(); break;
    }

    if (showConfirmPopup) {
        int w = 300, h = 150;
        int x = GetScreenWidth()/2 - w/2;
        int y = GetScreenHeight()/2 - h/2;

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        DrawRectangle(x, y, w, h, RAYWHITE);
        DrawRectangleLines(x, y, w, h, DARKGRAY);

        drawCenteredText("Deseja sair do jogo?", y + 30, 20, BLACK);

        Color c1 = CheckCollisionPointRec(GetMousePosition(), btnPopupContinue) ? LIGHTGRAY : GRAY;
        Color c2 = CheckCollisionPointRec(GetMousePosition(), btnPopupMenu) ? LIGHTGRAY : GRAY;

        DrawRectangleRec(btnPopupContinue, c1);
        DrawRectangleLinesEx(btnPopupContinue, 2, DARKGRAY);
        DrawText("Continuar", btnPopupContinue.x + 10, btnPopupContinue.y + 10, 18, BLACK);

        DrawRectangleRec(btnPopupMenu, c2);
        DrawRectangleLinesEx(btnPopupMenu, 2, DARKGRAY);
        DrawText("Menu", btnPopupMenu.x + 25, btnPopupMenu.y + 10, 18, BLACK);
    }
}