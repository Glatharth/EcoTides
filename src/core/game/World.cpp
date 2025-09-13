#include "World.hpp"
#include "ui/screen/Game.hpp"
#include "ui/screen/Options.hpp"
#include "ui/screen/Defeat.hpp"
#include "ui/screen/Victory.hpp"
#include <raylib-cpp.hpp>
#include <utility>

World::World() :
    currentScreen(GameScreen::MENU),
    dummy(0),
    game(std::make_unique<GameScreenClass>()),
    options(std::make_unique<Options>()),
    victory(std::make_unique<Victory>()),
    defeat(std::make_unique<Defeat>())
{}

World::~World() = default;

/**
 * @brief Reads user input and updates the state of the game.
 */
void World::update(float delta) {
    // Aqui você pode colocar a lógica de atualização do jogo, se necessário.
}


/**
 * @brief Draws the state of the game.
 */
void World::draw() {

    BeginDrawing();
    ClearBackground(WHITE);

    switch (currentScreen) {
        case GameScreen::MENU:
            drawMenu();
            break;
        case GameScreen::GAME:
            if (game) game->draw(*this);
            break;
        case GameScreen::OPTIONS:
            if (options) options->draw(*this);
            break;
        case GameScreen::VICTORY:
            if (victory) victory->draw(*this);
            break;
        case GameScreen::DEFEAT:
            if (defeat) defeat->draw(*this);
            break;
    }

    DrawFPS(20, 20);
    EndDrawing();
}

void World::drawMenu() {

    const char *text = "EcoTides";
    Vector2 m = MeasureTextEx(GetFontDefault(), text, 40, 4);
    int x = GetScreenWidth() / 2 - (int)(m.x / 2) - 15;
    int y = GetScreenHeight() / 2 - (int)(m.y / 2) - 300;

    // Título
    DrawRectangle(x, y, (int)m.x, (int)m.y, WHITE);
    DrawText(text, x, y, 40, BLUE);

    // Botões
    Rectangle btnStart = { (float)(GetScreenWidth()/2 - 100), (float)(y + 100), 200.0f, 50.0f };
    Rectangle btnOpcoes = { (float)(GetScreenWidth()/2 - 100), (float)(y + 170), 200.0f, 50.0f };

    Vector2 mouse = GetMousePosition();

    // Start
    bool hoverStart = CheckCollisionPointRec(mouse, btnStart);
    DrawRectangleRec(btnStart, hoverStart ? LIGHTGRAY : GRAY);
    int twStart = MeasureText("Start", 30);
    DrawText("Start",
             (int)(btnStart.x + btnStart.width/2 - twStart/2),
             (int)(btnStart.y + btnStart.height/2 - 15),
             30, BLACK);

    if (hoverStart && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentScreen = GameScreen::GAME;
    }

    // Opções
    bool hoverOpcoes = CheckCollisionPointRec(mouse, btnOpcoes);
    DrawRectangleRec(btnOpcoes, hoverOpcoes ? LIGHTGRAY : GRAY);
    int twOpcoes = MeasureText("Opcoes", 30);
    DrawText("Opcoes",
             (int)(btnOpcoes.x + btnOpcoes.width/2 - twOpcoes/2),
             (int)(btnOpcoes.y + btnOpcoes.height/2 - 15),
             30, BLACK);
             
    if (hoverOpcoes && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentScreen = GameScreen::OPTIONS;
    }
}
