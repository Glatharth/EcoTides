#include "World.hpp"
#include <raylib-cpp.hpp>

World::World() : dummy(0) {
    card = new Card(1);
    animation = new Animation(card);
}

World::~World() {
    delete animation;
    delete card;
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void World::update(float delta) {
    animation->update(delta);
}


/**
 * @brief Draws the state of the game.
 */
void World::draw() {
    BeginDrawing();
    ClearBackground(WHITE);
    animation->draw();
    DrawFPS(20, 20);
    EndDrawing();
}
