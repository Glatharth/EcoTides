#include "Screens.hpp"
#include <cstdlib>
#include "core/game/World.hpp"

extern World* globalWorldInstance;

Screens::Screens()
    : current(ScreenState::MENU),
      showConfirmPopup(false),
      mouseDebounce(false),
      backgroundImage(nullptr),
      backgroundAnimTime(0.0f),
      backgroundFrame(0),
      tutorialPage(0)
{
    int w = 300, h = 150;
    int x = GetScreenWidth() / 2 - w / 2;
    int y = GetScreenHeight() / 2 - h / 2;
    btnPopupContinue = { (float)(x + 30), (float)(y + 90), 100, 40 };
    btnPopupMenu     = { (float)(x + w - 130), (float)(y + 90), 100, 40 };

    backgroundImage = new raylib::Image("assets/background.png");
    backgroundTexture = backgroundImage->LoadTexture();

    texPlay = LoadTexture("assets/PLAYBUTTON.png");
    texPlayPressed = LoadTexture("assets/PLAYPRESSED.png");
    texExit = LoadTexture("assets/EXITBUTTON.png");
    texExitPressed = LoadTexture("assets/EXITPRESSED.png");
    texMenu = LoadTexture("assets/MENUBUTTON.png");
    texMenuPressed = LoadTexture("assets/MENUPRESSED.png");
    texContinue = LoadTexture("assets/CONTINUEBUTTON.png");
    texContinuePressed = LoadTexture("assets/CONTINUEPRESSED.png");
    texWin = LoadTexture("assets/WINSCREEN.png");
    texLose = LoadTexture("assets/LOSESCREEN.png");

    texTutorialButton = LoadTexture("assets/TUTORIALBUTTON.png");
    texTutorialButtonPressed = LoadTexture("assets/TUTORIALPRESSED.png");
    texArrowRight = LoadTexture("assets/seta direita.png");
    texArrowLeft = LoadTexture("assets/seta esquerda.png");
    texTutorialCard = LoadTexture("assets/tutorial carta.png");
}

Screens::~Screens() {
    backgroundTexture.Unload();
    if (backgroundImage) {
        delete backgroundImage;
        backgroundImage = nullptr;
    }

    UnloadTexture(texPlay);
    UnloadTexture(texPlayPressed);
    UnloadTexture(texExit);
    UnloadTexture(texExitPressed);
    UnloadTexture(texMenu);
    UnloadTexture(texMenuPressed);
    UnloadTexture(texContinue);
    UnloadTexture(texContinuePressed);
    UnloadTexture(texWin);
    UnloadTexture(texLose);

    UnloadTexture(texTutorialButton);
    UnloadTexture(texTutorialButtonPressed);
    UnloadTexture(texArrowRight);
    UnloadTexture(texArrowLeft);
    UnloadTexture(texTutorialCard);
}

void Screens::drawCenteredText(const char* text, int y, int fontSize, Color color) const {
    int textWidth = MeasureText(text, fontSize);
    int x = (GetScreenWidth() - textWidth) / 2;
    DrawText(text, x, y, fontSize, color);
}

bool Screens::drawButton(const Rectangle& rect, const char* label, Color normal, Color hover, int fontSize) const {
    Vector2 mouse = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mouse, rect);
    DrawRectangleRec(rect, isHover ? hover : normal);

    int textWidth = MeasureText(label, fontSize);
    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;
    DrawText(label, textX, textY, fontSize, BLACK);

    return isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

Rectangle Screens::makeButton(int centerX, int startY, int index, int btnWidth, int btnHeight, int spacing) const {
    return { (float)(centerX - btnWidth / 2), (float)(startY + index * (btnHeight + spacing)), (float)btnWidth, (float)btnHeight };
}

// ------------------------- DRAW MENU -------------------------
void Screens::drawMenuScreen() {
    drawCenteredText("EcoTides", 80, 50, WHITE);

    int centerX = GetScreenWidth() / 2;
    int startY = GetScreenHeight() / 2;
    float scale = 3.0f;

    // Botão Play
    Rectangle btnPlay = { (float)(centerX - texPlay.width * scale / 2),
                          (float)(startY),
                          texPlay.width * scale,
                          texPlay.height * scale };

    // Botão Tutorial
    float scaleTutorial = 2.5f;
    Rectangle btnTutorial = { (float)(centerX - texTutorialButton.width * scaleTutorial / 2),
                              (float)(startY + texPlay.height * scale + 20),
                              texTutorialButton.width * scaleTutorial,
                              texTutorialButton.height * scaleTutorial };

    // Botão Exit
    Rectangle btnExit = { (float)(centerX - texExit.width * scale / 2),
                          (float)(startY + texPlay.height * scale + texTutorialButton.height * scaleTutorial + 40),
                          texExit.width * scale,
                          texExit.height * scale };

    Vector2 mouse = GetMousePosition();

    // Draw Play
    if (CheckCollisionPointRec(mouse, btnPlay) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        DrawTextureEx(texPlayPressed, { btnPlay.x, btnPlay.y }, 0.0f, scale, WHITE);
    else
        DrawTextureEx(texPlay, { btnPlay.x, btnPlay.y }, 0.0f, scale, WHITE);

    if (CheckCollisionPointRec(mouse, btnPlay) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        change(ScreenState::GAME);

    // Draw Tutorial
    if (CheckCollisionPointRec(mouse, btnTutorial) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        DrawTextureEx(texTutorialButtonPressed, { btnTutorial.x, btnTutorial.y }, 0.0f, scaleTutorial, WHITE);
    else
        DrawTextureEx(texTutorialButton, { btnTutorial.x, btnTutorial.y }, 0.0f, scaleTutorial, WHITE);

    if (CheckCollisionPointRec(mouse, btnTutorial) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        change(ScreenState::TUTORIAL);

    // Draw Exit
    if (CheckCollisionPointRec(mouse, btnExit) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        DrawTextureEx(texExitPressed, { btnExit.x, btnExit.y }, 0.0f, scale, WHITE);
    else
        DrawTextureEx(texExit, { btnExit.x, btnExit.y }, 0.0f, scale, WHITE);

    if (CheckCollisionPointRec(mouse, btnExit) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        exit(0);
}

// ------------------------- DRAW GAME -------------------------
void Screens::drawGameScreen() {
    // Aqui entra todo o código original da tela de jogo
}

// ------------------------- DRAW VICTORY -------------------------
void Screens::drawVictoryScreen() {
    ClearBackground({200, 230, 255, 255});
    float scaleX = (float)GetScreenWidth() / texWin.width;
    float scaleY = (float)GetScreenHeight() / texWin.height;
    float scale = fmin(scaleX, scaleY);
    DrawTextureEx(texWin, {0,0}, 0.0f, scale, WHITE);

    float btnWidth = GetScreenWidth() * 0.25f;
    float btnHeightMenu = btnWidth * (texMenu.height / (float)texMenu.width);
    float btnHeightRetry = btnWidth * (texContinue.height / (float)texContinue.width);

    Rectangle btnMenu = { GetScreenWidth() * 0.25f - btnWidth/2, GetScreenHeight() * 0.75f, btnWidth, btnHeightMenu };
    Rectangle btnRetry = { GetScreenWidth() * 0.75f - btnWidth/2, GetScreenHeight() * 0.75f, btnWidth, btnHeightRetry };

    Vector2 mouse = GetMousePosition();
    bool hoverMenu = CheckCollisionPointRec(mouse, btnMenu);
    bool hoverRetry = CheckCollisionPointRec(mouse, btnRetry);

    DrawTexturePro(hoverMenu ? texMenuPressed : texMenu, {0,0,(float)texMenu.width,(float)texMenu.height}, btnMenu, {0,0}, 0.0f, WHITE);
    DrawTexturePro(hoverRetry ? texContinuePressed : texContinue, {0,0,(float)texContinue.width,(float)texContinue.height}, btnRetry, {0,0}, 0.0f, WHITE);

    if (hoverMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        change(ScreenState::MENU);
    if (hoverRetry && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        if (globalWorldInstance) globalWorldInstance->retry();
}

// ------------------------- DRAW DEFEAT -------------------------
void Screens::drawDefeatScreen() {
    ClearBackground({255, 200, 200, 255});
    float scaleX = (float)GetScreenWidth() / texLose.width;
    float scaleY = (float)GetScreenHeight() / texLose.height;
    float scale = fmin(scaleX, scaleY);
    DrawTextureEx(texLose, {0,0}, 0.0f, scale, WHITE);

    float btnWidth = GetScreenWidth() * 0.25f;
    float btnHeightMenu = btnWidth * (texMenu.height / (float)texMenu.width);
    float btnHeightRetry = btnWidth * (texContinue.height / (float)texContinue.width);

    Rectangle btnMenu = { GetScreenWidth() * 0.25f - btnWidth/2, GetScreenHeight() * 0.75f, btnWidth, btnHeightMenu };
    Rectangle btnRetry = { GetScreenWidth() * 0.75f - btnWidth/2, GetScreenHeight() * 0.75f, btnWidth, btnHeightRetry };

    Vector2 mouse = GetMousePosition();
    bool hoverMenu = CheckCollisionPointRec(mouse, btnMenu);
    bool hoverRetry = CheckCollisionPointRec(mouse, btnRetry);

    DrawTexturePro(hoverMenu ? texMenuPressed : texMenu, {0,0,(float)texMenu.width,(float)texMenu.height}, btnMenu, {0,0}, 0.0f, WHITE);
    DrawTexturePro(hoverRetry ? texContinuePressed : texContinue, {0,0,(float)texContinue.width,(float)texContinue.height}, btnRetry, {0,0}, 0.0f, WHITE);

    if (hoverMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        change(ScreenState::MENU);
    if (hoverRetry && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        if (globalWorldInstance) globalWorldInstance->retry();
}

// ------------------------- DRAW TUTORIAL -------------------------
void Screens::drawTutorialScreen() {
    ClearBackground({30, 30, 60, 255});

    int centerX = GetScreenWidth() / 2;
    float centerY = GetScreenHeight() / 2.0f;

    // Desenhar página atual
    switch (tutorialPage) {
        case 0:
            drawCenteredText("Contexto do jogo", 50, 35, WHITE); // título um pouco menor
            drawCenteredText("Você é um agente da Agencia Nacional das aguas", 150, 20, WHITE);
            drawCenteredText("e seu objetivo é mediar decisões que afetam os mares", 180, 20, WHITE);
            break;
        case 1:
            drawCenteredText("Mecânica das cartas", 50, 35, WHITE); // título menor
            drawCenteredText("Para jogar,segure a carta", 150, 20, WHITE);
            drawCenteredText("E mova para decidir", 180, 20, WHITE);
            DrawTextureEx(texTutorialCard, { centerX - texTutorialCard.width / 2.0f, centerY - texTutorialCard.height / 2.0f }, 0.0f, 1.0f, WHITE);
            break;
        case 2:
            drawCenteredText("Funcionamento dos poderes", 50, 35, WHITE); // título menor
            drawCenteredText("Você terá de equilibrar a balança", 150, 20, WHITE);
            drawCenteredText("são quatro poderes", 180, 20, WHITE);
            drawCenteredText("O único malefício é ter Acúmulo alto;", 210, 20, WHITE);
            break;
    }

    // Setas de navegação
    float arrowY = static_cast<float>(GetScreenHeight()) - texArrowRight.height - 20;
    Rectangle btnLeft  = { 50.0f, arrowY, static_cast<float>(texArrowLeft.width), static_cast<float>(texArrowLeft.height) };
    Rectangle btnRight = { static_cast<float>(GetScreenWidth() - texArrowRight.width - 50), arrowY, static_cast<float>(texArrowRight.width), static_cast<float>(texArrowRight.height) };

    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, btnLeft) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        DrawTextureEx(texArrowLeft, {btnLeft.x, btnLeft.y}, 0.0f, 1.0f, WHITE);
    else
        DrawTextureEx(texArrowLeft, {btnLeft.x, btnLeft.y}, 0.0f, 1.0f, WHITE);

    if (CheckCollisionPointRec(mouse, btnRight) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        DrawTextureEx(texArrowRight, {btnRight.x, btnRight.y}, 0.0f, 1.0f, WHITE);
    else
        DrawTextureEx(texArrowRight, {btnRight.x, btnRight.y}, 0.0f, 1.0f, WHITE);

    if (!mouseDebounce) {
        if (CheckCollisionPointRec(mouse, btnLeft) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            tutorialPage = (tutorialPage - 1 + 3) % 3;
            mouseDebounce = true;
        }
        if (CheckCollisionPointRec(mouse, btnRight) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            tutorialPage = (tutorialPage + 1) % 3;
            mouseDebounce = true;
        }
    }
}

// ------------------------- UPDATE -------------------------
void Screens::update(float delta) {
    backgroundAnimTime += delta;
    if (backgroundAnimTime >= backgroundFrameDuration) {
        backgroundAnimTime = 0.0f;
        backgroundFrame = (backgroundFrame + 1) % backgroundFrameCount;
    }

    if (mouseDebounce) {
        if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            mouseDebounce = false;
        else
            return;
    }

    if (showConfirmPopup) {
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, btnPopupContinue)) {
                showConfirmPopup = false;
                if (globalWorldInstance) globalWorldInstance->retry();
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

    if ((current == ScreenState::GAME || current == ScreenState::TUTORIAL) && IsKeyPressed(KEY_ENTER))
        showConfirmPopup = true;

    if (IsKeyPressed(KEY_V)) change(ScreenState::VICTORY);
    if (IsKeyPressed(KEY_D)) change(ScreenState::DEFEAT);
}

// ------------------------- RENDER -------------------------
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
        case ScreenState::TUTORIAL: drawTutorialScreen(); break;
    }

    if (showConfirmPopup) {
        float popupW = GetScreenWidth() * 0.45f;
        float popupH = GetScreenHeight() * 0.3f;
        float popupX = GetScreenWidth()/2 - popupW/2;
        float popupY = GetScreenHeight()/2 - popupH/2;

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

        Color popupColor = {240, 240, 240, 255};
        DrawRectangleRounded({popupX, popupY, popupW, popupH}, 0.1f, 10, popupColor);
        DrawRectangleRoundedLines({popupX, popupY, popupW, popupH}, 0.1f, 10, 3, DARKGRAY);

        int fontSize = static_cast<int>(popupH * 0.15f);
        drawCenteredText("Deseja sair ?", popupY + popupH * 0.25f, fontSize, BLACK);

        float btnWidth = popupW * 0.4f;
        float btnHeight = popupH * 0.3f;
        float btnY = popupY + popupH * 0.6f;

        Rectangle btnContinue = {popupX + popupW * 0.05f, btnY, btnWidth, btnHeight};
        Rectangle btnMenu     = {popupX + popupW - btnWidth - popupW * 0.05f, btnY, btnWidth, btnHeight};

        Vector2 mouse = GetMousePosition();
        bool hoverContinue = CheckCollisionPointRec(mouse, btnContinue);
        bool hoverMenu     = CheckCollisionPointRec(mouse, btnMenu);

        DrawTexturePro(
            hoverContinue ? texContinuePressed : texContinue,
            {0,0,(float)texContinue.width,(float)texContinue.height},
            btnContinue,
            {0,0},0.0f,WHITE
        );

        DrawTexturePro(
            hoverMenu ? texMenuPressed : texMenu,
            {0,0,(float)texMenu.width,(float)texMenu.height},
            btnMenu,
            {0,0},0.0f,WHITE
        );

        if (!mouseDebounce) {
            if (hoverContinue && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showConfirmPopup = false;
                mouseDebounce = true;
                if (globalWorldInstance) globalWorldInstance->retry();
            }
            if (hoverMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showConfirmPopup = false;
                mouseDebounce = true;
                change(ScreenState::MENU);
            }
        }
    }
}

// ------------------------- CHANGE & GET -------------------------
void Screens::change(ScreenState next) { current = next; }
ScreenState Screens::getCurrent() const { return current; }