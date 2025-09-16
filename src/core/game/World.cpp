#include "World.hpp"
#include <raylib-cpp.hpp>


World::World() : dummy(0) {
    screen = new Screens();
    
}

World::~World() {
    delete screen;
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void World::update(float delta) {
    screen->update(delta);
}


/**
 * @brief Draws the state of the game.
 */
void World::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Desenha a tela atual
    screen->render();

    DrawFPS(20, 20); // FPS no canto
    EndDrawing();
}
