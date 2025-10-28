#include "World.hpp"
#include "utils/utils.hpp"

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
    if (animation->option) {
        for (auto it = card->GetResourcesYes().begin(); it != card->GetResourcesYes().end(); ++it) {
            ResourceType type = it->first;
            int value = it->second;
            powers->applyChange(type, value);
        }
    } else {
        for (auto it = card->GetResourcesNo().begin(); it != card->GetResourcesNo().end(); ++it) {
            ResourceType type = it->first;
            int value = it->second;
            powers->applyChange(type, value);
        }
    }
    animation->setCard(card);
}

int World::nextCard() {
    if (seed.empty()) return 1;
    if (currentIndex == seed.size()) {
        if (powers->getValue(ResourceType::WASTE_COLLECTION) > 80 && powers->getValue(ResourceType::WASTE_ACCUMULATION) <= 80) {
            screen->change(ScreenState::VICTORY);
        } else {
            screen->change(ScreenState::DEFEAT);
        }
        return 0;
    }

    const uint8_t currentId = seed[currentIndex];

    currentIndex = currentIndex + 1;

    return currentId;
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
    if (animation) animation->setCard(card);
    }

    powers->update(delta);

    if (powers->getValue(ResourceType::ECONOMY) <= 0 ||
    powers->getValue(ResourceType::POPULATION_AWARENESS) <= 0 ||
    powers->getValue(ResourceType::WASTE_COLLECTION) <= 0 ||
    powers->getValue(ResourceType::WASTE_ACCUMULATION) >= 100) {
        playerLost = true;
    } else if (powers->getValue(ResourceType::ECONOMY) >= 70 &&
    powers->getValue(ResourceType::POPULATION_AWARENESS) >= 70 &&
    powers->getValue(ResourceType::WASTE_COLLECTION) >= 70 &&
    powers->getValue(ResourceType::WASTE_ACCUMULATION) <= 30) {
        playerWon = true;
    }

    if (playerWon && screen->getCurrent() != ScreenState::VICTORY) {
        screen->change(ScreenState::VICTORY);
    } else if (playerLost && screen->getCurrent() != ScreenState::DEFEAT) {
        screen->change(ScreenState::DEFEAT);
    }
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

    screen->render();

    if (!screen->isPopupActive() && screen->getCurrent() == ScreenState::GAME) {
        drawPowers();            
        if (animation) animation->draw();
    }
    EndDrawing();
}