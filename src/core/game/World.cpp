#include "World.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <raylib-cpp.hpp>

/**
 * @brief Global pointer to the current World instance.
 */
World* globalWorldInstance = nullptr;
/**
 * @brief Static seed vector used for card generation.
 */
std::vector<uint8_t> World::seed{};
/**
 * @brief Static index tracking the current position in the seed vector.
 */
size_t World::currentIndex = 0;

/**
 * @brief Constructs a new World object and initializes its systems.
 * Sets up the screen manager, powers manager, loads card data from XML,
 * and generates a random seed for gameplay.
 */
World::World()
    : screen(new Screens()), powers(new Powers()),
      card(nullptr), animation(nullptr),
      cardIndex(0), playerWon(false), playerLost(false),
      loader("src/xml/cards.xml") 
{
    globalWorldInstance = this;
    generateSeed(&seed, GameDifficulty::HARD);
}

/**
 * @brief Destructor for the World class.
 */
World::~World() {
    delete screen;
    delete powers;
    if (card) delete card;
    if (animation) delete animation;
}

/**
 * @brief Gets the current screen state.
 */
ScreenState World::getCurrent() const {
    return screen->getCurrent();
}

/**
 * @brief Starts the game logic by initializing the first card and animation.
 * Resets win/loss states and transitions the screen to the GAME state.
 */
void World::startGame() {
    cardIds = {1,2,3,4};
    card = new Card(nextCard(), loader);
    animation = new Animation(card);

    playerWon = false;
    playerLost = false;

    screen->change(ScreenState::GAME);
}

/**
 * @brief Swaps the current card with the next one.
 */
void World::cardSwap() {
    if (card) delete card;
    card = new Card(nextCard(), loader);
    animation->setCard(card);
}

/**
 * @brief Restarts the game by resetting state.
 */
void World::retry() {
    if (card) { delete card; card = nullptr; }
    if (animation) { delete animation; animation = nullptr; }
    startGame(); 
}

/**
 * @brief Updates the game logic for the current frame.
 * Handles animation, screen transitions, power values, win/loss conditions,
 * and card transitions.
 * @param delta Time elapsed since the last frame (in seconds).
 */
void World::update(float delta) {
    ScreenState prev = screen->getCurrent();

    if (screen->getCurrent() == ScreenState::GAME && card == nullptr) {
        startGame(); 
    }

    // Update animation and check for card swap
    if (animation) {
        animation->update(delta);
        if (animation->needsCardSwap()) {
            cardSwap();
            animation->resetSwap();
        }
    }

    screen->update(delta);

    // Reset state if entering GAME screen from another state
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

    // Check for loss conditions
    if (powers->getValue(ResourceType::ECONOMY) <= 0 ||
    powers->getValue(ResourceType::POPULATION_AWARENESS) <= 0 ||
    powers->getValue(ResourceType::WASTE_COLLECTION) <= 0 ||
    powers->getValue(ResourceType::WASTE_ACCUMULATION) >= 100)
{
    playerLost = true;
} 
    // Check for win conditions
else if (powers->getValue(ResourceType::ECONOMY) >= 70 &&
         powers->getValue(ResourceType::POPULATION_AWARENESS) >= 70 &&
         powers->getValue(ResourceType::WASTE_COLLECTION) >= 70 &&
         powers->getValue(ResourceType::WASTE_ACCUMULATION) <= 30)
{
    playerWon = true;
}
    // Screen transitions based on game state
    if (playerWon && screen->getCurrent() != ScreenState::VICTORY)
        screen->change(ScreenState::VICTORY);
    else if (playerLost && screen->getCurrent() != ScreenState::DEFEAT)
        screen->change(ScreenState::DEFEAT);
}

/**
 * @brief Draws power icons to the screen if in GAME state.
 *
 * Displays visual indicators of resource levels at the bottom of the screen.
 */
void World::drawPowers() {
    if (!powers) return;
    if (screen->getCurrent() != ScreenState::GAME) return;

    int startX = 50;
    int startY = GetScreenHeight() - 150;
    int size   = 64;
    int spacing = 80;

    powers->drawIcons(startX, startY, size, spacing);
}

/**
 * @brief Renders the current frame to the screen.
 *
 * Clears the background, renders the screen contents, overlays the power icons
 * and animations if applicable, and draws the FPS counter.
 */
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