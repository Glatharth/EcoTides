#include "World.hpp"
#include <raylib-cpp.hpp>

World::World() : dummy(0) {}

World::~World() {}

/**
 * @brief Reads user input and updates the state of the game.
 */
void World::update(float delta) {
}


/**
 * @brief Draws the state of the game.
 */
void World::draw() {
    BeginDrawing();
    ClearBackground(WHITE);

    const char *text = "Frase de efeito foda";
    Vector2 m = MeasureTextEx(GetFontDefault(), text, 40, 4);
    int x = GetScreenWidth() / 2 - m.x / 2;
    int y = GetScreenHeight() / 2 - m.y / 2;
    DrawRectangle(x, y, m.x, m.y, BLACK);
    DrawText(text, x, y, 40, WHITE);
    DrawFPS(20, 20);
    EndDrawing();
}
