#ifndef SCREENS_HPP
#define SCREENS_HPP

#include <raylib.h>
#include "utils/enum.hpp"

typedef void (*StartGameCallback)();

class Screens {
public:
    Screens();

    void update(float delta);
    void render();
    void change(ScreenState next);
    ScreenState getCurrentScreen() const { return current; }
    void setStartGameCallback(StartGameCallback callback);

private:
    ScreenState current;

    StartGameCallback startGameCallback = nullptr;

    bool playerWon = false;
    bool playerLost = false;

    void drawCenteredText(const char* text, int y, int fontSize, Color color);
    bool drawButton(Rectangle rect, const char* label, Color normal, Color hover, int fontSize = 20);
    Rectangle makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing);

    void drawMenuScreen();
    void drawGameScreen();
    void drawOptionsScreen();
    void drawVictoryScreen();
    void drawDefeatScreen();
};

#endif