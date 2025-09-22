#include "World.hpp"
#include "utils/utils.hpp"


World* globalWorldInstance = nullptr;
std::vector<uint8_t> World::seed;
size_t World::currentIndex = 0;

void startGameWrapper() {
    if (globalWorldInstance) {
        globalWorldInstance->startGame();
    }
}

World::World() : card(nullptr), animation(nullptr), playerWon(false), playerLost(false) {
    globalWorldInstance = this;

    screen = new Screens();
    screen->change(ScreenState::MENU);
    screen->setStartGameCallback(startGameWrapper);
    generateSeed(&seed, GameDifficulty::HARD);
}

World::~World() {
    if (animation) delete animation;
    if (card) delete card;
    delete screen;
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

void World::update(float delta) {
    if (animation) {
        animation->update(delta);

        if (animation->needsCardSwap()) {
            cardSwap();
            animation->resetSwap();
        }
        /* IMPLEMENTAR CONDICOES DE VITÓRIA/DERROTA
        if ((condicao de vitoria)false) {
            playerWon = true;
            screen->change(ScreenState::VICTORY);
        }
        else if ((condição de derrota)false) {
            playerLost = true;
            screen->change(ScreenState::DEFEAT);
        }
        */
        //condicoes de vitória/derrota temporárias para teste
        if (playerWon) {
            // Garante que só mude uma vez para a tela de vitória
            if (screen->getCurrentScreen() != ScreenState::VICTORY) {
                screen->change(ScreenState::VICTORY);
            }
        }
        else if (playerLost) {
            if (screen->getCurrentScreen() != ScreenState::DEFEAT) {
                screen->change(ScreenState::DEFEAT);
            }
        }
        else {
            if (screen->getCurrentScreen() != ScreenState::GAME) {
                screen->change(ScreenState::GAME);
            }
        }
    }

    screen->update(delta);
}

void World::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    screen->render();

    if (animation) {
        animation->draw();
    }

    DrawFPS(20, 20);
    EndDrawing();
}