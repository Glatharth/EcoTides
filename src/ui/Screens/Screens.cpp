#include "Screens.hpp"
#include <cstdlib>
#include "core/game/World.hpp"

/// Global pointer to the currently active world instance.
extern World* globalWorldInstance;

/**
 * @class Screens
 * @brief Manages different screen states (Menu, Game, Victory, Defeat) and related UI rendering & interactions.
 */
Screens::Screens() : current(ScreenState::MENU), showConfirmPopup(false), mouseDebounce(false) {
    int w = 300, h = 150;
    int x = GetScreenWidth()/2 - w/2;
    int y = GetScreenHeight()/2 - h/2;

    // Popup buttons
    btnPopupContinue = { (float)(x + 30), (float)(y + 90), 100, 40 };
    btnPopupMenu     = { (float)(x + w - 130), (float)(y + 90), 100, 40 };

    // Victory screen buttons
    btnVictoryMenu  = { (float)(x + 30), (float)(y + 200), 100, 40 };
    btnVictoryRetry = { (float)(x + w - 130), (float)(y + 200), 100, 40 };

    // Defeat screen buttons
    btnDefeatMenu  = { (float)(x + 30), (float)(y + 200), 100, 40 };
    btnDefeatRetry = { (float)(x + w - 130), (float)(y + 200), 100, 40 };
}

/**
 * @brief Changes the current screen state.
 * @param next The screen state to switch to.
 */
void Screens::change(ScreenState next) { current = next; }
/**
 * @brief Returns the currently active screen state.
 * @return The current ScreenState.
 */
ScreenState Screens::getCurrent() const { return current; }

/**
 * @brief Draws centered text horizontally on the screen.
 * @param text The string to render.
 * @param y The Y coordinate where text will be drawn.
 * @param fontSize Font size.
 * @param color Color of the text.
 */
void Screens::drawCenteredText(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int x = (GetScreenWidth() - textWidth) / 2;
    DrawText(text, x, y, fontSize, color);
}

/**
 * @brief Draws a clickable button and handles hover/click state.
 * @param rect The button rectangle.
 * @param label Text label for the button.
 * @param normal Color when not hovered.
 * @param hover Color when hovered.
 * @param fontSize Font size of the label.
 * @return true if the button is clicked, false otherwise.
 */
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

/**
 * @brief Utility function to create a button rectangle based on layout parameters.
 * @param centerX X center of the button.
 * @param startY Y starting offset for button list.
 * @param index Vertical index of the button (used for spacing).
 * @param btnWidth Width of the button.
 * @param btnHeight Height of the button.
 * @param spacing Spacing between vertically stacked buttons.
 * @return Rectangle representing the button.
 */
Rectangle Screens::makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing) {
    return { (float)(centerX - btnWidth / 2),
             (float)(startY + index * (btnHeight + spacing)),
             (float)btnWidth,
             (float)btnHeight };
}

/**
 * @brief Draws the main menu screen with title and buttons.
 */
void Screens::drawMenuScreen() {
    ClearBackground(BLUE);
    drawCenteredText("EcoTides", 80, 50, DARKBLUE);

    int centerX = GetScreenWidth() / 2;
    int startY = (GetScreenHeight() - (2 * 50 + 1 * 20)) / 2;

    if (drawButton(makeButton(centerX, startY, 0, 200, 50, 20), "START GAME", LIGHTGRAY, SKYBLUE))
        change(ScreenState::GAME);

    if (drawButton(makeButton(centerX, startY, 1, 200, 50, 20), "EXIT", LIGHTGRAY, SKYBLUE))
        exit(0);
}

/**
 * @brief Draws the game screen (currently empty background).
 */
void Screens::drawGameScreen() {
    ClearBackground(BLUE);
}

/**
 * @brief Draws the victory screen with options to return to menu or retry.
 */
void Screens::drawVictoryScreen() {
    ClearBackground(GREEN);
    drawCenteredText("YOU WIN!", 200, 40, WHITE);

    if (drawButton(btnVictoryMenu, "MENU", LIGHTGRAY, SKYBLUE))
        change(ScreenState::MENU);

    if (drawButton(btnVictoryRetry, "RETRY", LIGHTGRAY, SKYBLUE))
        if (globalWorldInstance) globalWorldInstance->retry();
}

/**
 * @brief Draws the defeat screen with options to return to menu or retry.
 */
void Screens::drawDefeatScreen() {
    ClearBackground(RED);
    drawCenteredText("YOU LOSE!", 200, 40, WHITE);

    if (drawButton(btnDefeatMenu, "MENU", LIGHTGRAY, SKYBLUE))
        change(ScreenState::MENU);

    if (drawButton(btnDefeatRetry, "RETRY", LIGHTGRAY, SKYBLUE))
        if (globalWorldInstance) globalWorldInstance->retry();
}

/**
 * @brief Updates the screen logic, handles input and popups.
 * @param delta Time delta in seconds since last frame.
 */
void Screens::update(float delta) {

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

/**
 * @brief Renders the currently active screen and the confirmation popup if shown.
 */
void Screens::render() {
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