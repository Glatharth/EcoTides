#pragma once
#include "utils/enum.hpp"
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <cstdlib>

class World;

class Screens {
private:
    ScreenState current;
    bool showConfirmPopup;
    bool mouseDebounce;
   
    Rectangle btnPopupContinue;
    Rectangle btnPopupMenu;

    Rectangle btnVictoryMenu;
    Rectangle btnVictoryRetry;

    Rectangle btnDefeatMenu;
    Rectangle btnDefeatRetry;

    raylib::Image* backgroundImage;
    raylib::Texture backgroundTexture;
    float backgroundAnimTime = 0.0f;
    int backgroundFrame = 0;
    const int backgroundFrameCount = 10;
    const float backgroundFrameDuration = 0.1f; // 10 fps
    
public:
    Screens();
    ~Screens();

    Texture2D texPlay;
    Texture2D texExit;
    Texture2D texMenu;
    Texture2D texContinue;
    Texture2D texWin;
    Texture2D texLose;
    Texture2D texPlayPressed;
    Texture2D texExitPressed;
    Texture2D texMenuPressed;
    Texture2D texContinuePressed;

    void change(ScreenState next);
    ScreenState getCurrent() const;

    void update(float delta);
    void render();

    bool isPopupActive() const { return showConfirmPopup; }

    void drawCenteredText(const char* text, int y, int fontSize, Color color) const;
    bool drawButton(const Rectangle& rect, const char* label, Color normal, Color hover, int fontSize = 20) const;
    Rectangle makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing) const;

    void drawMenuScreen();
    void drawGameScreen();
    void drawVictoryScreen();
    void drawDefeatScreen();
};
