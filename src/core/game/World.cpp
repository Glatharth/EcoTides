#include "World.hpp"

#include "utils/utils.hpp"


World* globalWorldInstance = nullptr;

void startGameWrapper() {
    if (globalWorldInstance) {
        globalWorldInstance->startGame();
    }
}

World::World() : card(nullptr), animation(nullptr), cardIndex(0), playerWon(false), playerLost(false) {
    globalWorldInstance = this;

    screen = new Screens();
    screen->change(ScreenState::MENU);
    screen->setStartGameCallback(startGameWrapper);
}

World::~World() {
    if (animation) delete animation;
    if (card) delete card;
    delete screen;
}

void World::startGame() {
    std::string seed;
    generateSeed(&seed, GameDifficulty::HARD);
    printf("Testttttttttttttttttttttttttttttttt: %s\n", getSeedCardIdsAsString(seed).c_str());
    cardIds[0] = 1;
    cardIds[1] = 2;
    cardIds[2] = 3;
    cardIds[3] = 4;

    card = new Card(cardIds[0]);
    animation = new Animation(card);
    cardIndex = 1;

    playerWon = false;
    playerLost = false;

    screen->change(ScreenState::GAME);
}

void World::cardSwap() {
    if (card) delete card;
    card = new Card(cardIds[cardIndex]);
    cardIndex = (cardIndex + 1) % 4;
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