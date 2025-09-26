#include "World.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <raylib-cpp.hpp>

World* globalWorldInstance = nullptr;

World::World()
    : screen(new Screens()), powers(new Powers()),
      card(nullptr), animation(nullptr),
      cardIndex(0), playerWon(false), playerLost(false),
      loader("src/xml/cards.xml") 
{
    globalWorldInstance = this;
    generateSeed(&seed, GameDifficulty::HARD);
}

World::~World() {
    delete screen;
    delete powers;
    if (card) delete card;
    if (animation) delete animation;
}

ScreenState World::getCurrent() const {
    return screen->getCurrent();
}

void World::startGame() {
    cardIds[0] = 1;
    cardIds[1] = 2;
    cardIds[2] = 3;
    cardIds[3] = 4;

    card = new Card(nextCard());
    animation = new Animation(card);

    playerWon = false;
    playerLost = false;

    screen->change(ScreenState::GAME);
}

void World::cardSwap() {
    if (card) delete card;
    card = new Card(nextCard());
    animation->setCard(card);
}

void World::retry() {
    if (card) { delete card; card = nullptr; }
    if (animation) { delete animation; animation = nullptr; }
    startGame(); 
}

void World::update(float delta) {
    ScreenState prev = screen->getCurrent();

    if (screen->getCurrent() == ScreenState::GAME && card == nullptr) {
        startGame(); 
    }

    if (animation) {
        animation->update(delta);
        if (animation->needsCardSwap()) {
            cardSwap();
            animation->resetSwap();
        }
    }

    screen->update(delta);

    if (screen->getCurrent() == ScreenState::GAME && prev != ScreenState::GAME) {
        powers->reset();
        playerWon = false;
        playerLost = false;
    if (card) delete card;
    card = new Card(cardIds[cardIndex], loader);
    if (animation) animation->setCard(card);
    cardIndex = 1;
    }

    powers->update(delta);

    if (IsKeyPressed(KEY_Q)) powers->applyChange(PowerType::ECONOMY, +5);
    if (IsKeyPressed(KEY_A)) powers->applyChange(PowerType::ECONOMY, -5);
    if (IsKeyPressed(KEY_W)) powers->applyChange(PowerType::AWARENESS, +5);
    if (IsKeyPressed(KEY_S)) powers->applyChange(PowerType::AWARENESS, -5);
    if (IsKeyPressed(KEY_E)) powers->applyChange(PowerType::TRASH_COLLECTION, +5);
    if (IsKeyPressed(KEY_D)) powers->applyChange(PowerType::TRASH_COLLECTION, -5);
    if (IsKeyPressed(KEY_R)) powers->applyChange(PowerType::TRASH_ACCUMULATION, +5);
    if (IsKeyPressed(KEY_F)) powers->applyChange(PowerType::TRASH_ACCUMULATION, -5);

    if (powers->getValue(PowerType::ECONOMY) <= 0 ||
    powers->getValue(PowerType::AWARENESS) <= 0 ||
    powers->getValue(PowerType::TRASH_COLLECTION) <= 0 ||
    powers->getValue(PowerType::TRASH_ACCUMULATION) >= 100)
{
    playerLost = true;
} 
else if (powers->getValue(PowerType::ECONOMY) >= 70 &&
         powers->getValue(PowerType::AWARENESS) >= 70 &&
         powers->getValue(PowerType::TRASH_COLLECTION) >= 70 &&
         powers->getValue(PowerType::TRASH_ACCUMULATION) <= 30)
{
    playerWon = true;
}

    if (playerWon && screen->getCurrent() != ScreenState::VICTORY)
        screen->change(ScreenState::VICTORY);
    else if (playerLost && screen->getCurrent() != ScreenState::DEFEAT)
        screen->change(ScreenState::DEFEAT);
}


void World::drawPowers() {
    if (!powers) return;
    if (screen->getCurrent() != ScreenState::GAME) return;

    int startX = 50;
    int startY = GetScreenHeight() - 150;
    int size   = 64;
    int spacing = 80;

    powers->drawIcons(startX, startY, size, spacing);
}

void World::draw() {
    BeginDrawing();
    ClearBackground(BLUE);

    screen->render();

    if (!screen->isPopupActive() && screen->getCurrent() == ScreenState::GAME) {
        drawPowers();            
        if (animation) animation->draw();
    }

    DrawFPS(20, 20);
    EndDrawing();
}