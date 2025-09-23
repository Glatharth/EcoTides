
#pragma once
#include "utils/enum.hpp"
#include <raylib.h>

class World;

class Screens {
private:
    ScreenState current;
    bool showConfirmPopup;
    bool mouseDebounce;
   
    // Botões do popup
    Rectangle btnPopupContinue;
    Rectangle btnPopupMenu;

    // Botões da tela de vitória
    Rectangle btnVictoryMenu;
    Rectangle btnVictoryRetry;

    // Botões da tela de derrota
    Rectangle btnDefeatMenu;
    Rectangle btnDefeatRetry;
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
    void drawOptionsScreen();
    void drawVictoryScreen();
    void drawDefeatScreen();
};

