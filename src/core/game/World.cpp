#include "World.hpp"
#include <raylib-cpp.hpp>

World::World() : dummy(0) {
    int initialCardId = 4;
    card = new Card(initialCardId);
    animation = new Animation(card);
}

World::~World() {
    delete animation;
    delete card;
}

/**
 * @brief Reads user input and updates the state of the game.
 */

void World::cardSwap() {
    static int cardIndex = 0;
    delete card;

    const int cardIds[] = {1, 2, 3, 4};

    card = new Card(cardIds[cardIndex]);
    cardIndex = (cardIndex + 1) % 4;

    animation->setCard(card);
}



void World::update(float delta) {
    animation->update(delta);
    if (animation->needsCardSwap()) {
        cardSwap();
        animation->resetSwap();
    }
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
