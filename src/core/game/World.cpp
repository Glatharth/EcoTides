#include "World.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <raylib-cpp.hpp>

World* globalWorldInstance = nullptr;
std::vector<uint8_t> World::seed;
size_t World::currentIndex = 0;

World::World()
    : screen(new Screens()), powers(new Powers()),
      card(nullptr), animation(nullptr),
      playerWon(false), playerLost(false),
      cardXML("src/xml/cards.xml")
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
    if (animation) animation->setCard(card);
    }

    powers->update(delta);

    if (powers->getValue(ResourceType::ECONOMY) <= 0 ||
    powers->getValue(ResourceType::POPULATION_AWARENESS) <= 0 ||
    powers->getValue(ResourceType::WASTE_COLLECTION) <= 0 ||
    powers->getValue(ResourceType::WASTE_ACCUMULATION) >= 100)
{
    playerLost = true;
} 
else if (powers->getValue(ResourceType::ECONOMY) >= 70 &&
         powers->getValue(ResourceType::POPULATION_AWARENESS) >= 70 &&
         powers->getValue(ResourceType::WASTE_COLLECTION) >= 70 &&
         powers->getValue(ResourceType::WASTE_ACCUMULATION) <= 30)
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