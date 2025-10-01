#pragma once
#include "utils/enum.hpp"
#include <raylib-cpp.hpp>

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
    void change(ScreenState next);
    ScreenState getCurrent() const;
    void update(float delta);
    void render();
    bool isPopupActive() const { return showConfirmPopup; }
    void drawCenteredText(const char* text,int y,int fontSize,Color color);
    bool drawButton(Rectangle rect,const char* label,Color normal,Color hover,int fontSize=20);
    Rectangle makeButton(int centerX,int startY,int index,int btnWidth,int btnHeight,int spacing);
    void drawMenuScreen();
    void drawGameScreen();
    void drawVictoryScreen();
    void drawDefeatScreen();
};

