#include "World.hpp"
#include <raylib-cpp.hpp>

World::World() : dummy(0) {
    cardIds[0] = 1;
    cardIds[1] = 2;
    cardIds[2] = 3;
    cardIds[3] = 4;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < 4; ++i) {
        int randIndex = rand() % 4;
        int temp = cardIds[i];
        cardIds[i] = cardIds[randIndex];
        cardIds[randIndex] = temp;
    }

    card = new Card(cardIds[0]);
    animation = new Animation(card);
    cardIndex = 1;
}

World::~World() {
    delete animation;
    delete card;
}

/**
 * @brief Reads user input and updates the state of the game.
 */

void World::cardSwap() {
    delete card;

    card = new Card(cardIds[cardIndex]);  // cardIds e cardIndex são membros da classe
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
