#include "World.hpp"

World* globalWorldInstance = nullptr;

void startGameWrapper() {
    if (globalWorldInstance) {
        globalWorldInstance->startGame();
    }
}

World::World() : card(nullptr), animation(nullptr), cardIndex(0), playerWon(false), playerLost(false) {
    globalWorldInstance = this;

    screen = new Screens();
    powers = new Powers();
}

World::~World() {
    if (animation) delete animation;
    if (card) delete card;
    delete screen;
    delete powers;
}

ScreenState World::getCurrent() const {
    return screen->getCurrent();
}

// ---------------- INICIA O JOGO ----------------
void World::startGame() {
    cardPaths[0] = "assets/card1.png";
    cardPaths[1] = "assets/card2.png";
    cardPaths[2] = "assets/card3.png";
    cardPaths[3] = "assets/card4.png";

    card = new Card(cardPaths[0]);
    animation = new Animation(card);
    cardIndex = 1;

    playerWon = false;
    playerLost = false;

    screen->change(ScreenState::GAME);
}

// ---------------- TROCA DE CARTA ----------------
void World::cardSwap() {
    if (card) delete card;
    card = new Card(cardPaths[cardIndex]);
    cardIndex = (cardIndex + 1) % 4;
    animation->setCard(card);
}

// ---------------- ATUALIZAÇÃO ----------------
void World::update(float delta) {
    ScreenState prev = screen->getCurrent();

    // Atualiza animação e troca de cartas
    if (animation) {
        animation->update(delta);

        if (animation->needsCardSwap()) {
            cardSwap();
            animation->resetSwap();
        }
    }

    // Atualiza tela e poderes
    screen->update(delta);

    // Reset dos poderes ao entrar em GAME (START ou RETRY)
    if (screen->getCurrent() == ScreenState::GAME && prev != ScreenState::GAME) {
        powers->reset();
        playerWon = false;
        playerLost = false;
        return; // não processa derrota neste frame
    }

    powers->update(delta);

    // Controles de teste (Q/W/E/R/A/S/D/F)
    if(IsKeyPressed(KEY_Q)) powers->change(PowerType::ECONOMY, +5);
    if(IsKeyPressed(KEY_A)) powers->change(PowerType::ECONOMY, -5);
    if(IsKeyPressed(KEY_W)) powers->change(PowerType::AWARENESS, +5);
    if(IsKeyPressed(KEY_S)) powers->change(PowerType::AWARENESS, -5);
    if(IsKeyPressed(KEY_E)) powers->change(PowerType::TRASH_COLLECTION, +5);
    if(IsKeyPressed(KEY_D)) powers->change(PowerType::TRASH_COLLECTION, -5);
    if(IsKeyPressed(KEY_R)) powers->change(PowerType::TRASH_ACCUMULATION, +5);
    if(IsKeyPressed(KEY_F)) powers->change(PowerType::TRASH_ACCUMULATION, -5);

    // ---------------- Condições de Vitória/Derrota ----------------
    if (screen->getCurrent() == ScreenState::GAME) {
        // Derrota se algum poder crítico atingir limite
        if (powers->get(PowerType::ECONOMY) <= 0 ||
            powers->get(PowerType::AWARENESS) <= 0 ||
            powers->get(PowerType::TRASH_COLLECTION) <= 0 ||
            powers->get(PowerType::TRASH_ACCUMULATION) >= 100) {

            playerLost = true;
        }

        // Vitória condicional (exemplo: todos acima de 70%, lixo <= 30%)
        if (powers->get(PowerType::ECONOMY) >= 70 &&
            powers->get(PowerType::AWARENESS) >= 70 &&
            powers->get(PowerType::TRASH_COLLECTION) >= 70 &&
            powers->get(PowerType::TRASH_ACCUMULATION) <= 30) {

            playerWon = true;
        }
    }

    // Aplica mudança de tela
    if (playerWon && screen->getCurrent() != ScreenState::VICTORY) {
        screen->change(ScreenState::VICTORY);
    } else if (playerLost && screen->getCurrent() != ScreenState::DEFEAT) {
        screen->change(ScreenState::DEFEAT);
    } else if (!playerWon && !playerLost && screen->getCurrent() != ScreenState::GAME) {
        screen->change(ScreenState::GAME);
    }
}

// ---------------- DESENHA AS BARRAS DE PODER ----------------
void World::drawPowers() {
    if (!powers) return;
    if (screen->getCurrent() != ScreenState::GAME) return;

    int startX = GetScreenWidth() - 100;
    int startY = 100;
    int barWidth = 50;
    int barHeight = 250;
    int spacing = 70;
    int x = startX;

    auto drawBar = [&](const char* label, float animatedValue, Color color) {
        DrawRectangle(x, startY, barWidth, barHeight, Fade(LIGHTGRAY, 0.3f));
        int valHeight = (barHeight * animatedValue) / 100;
        DrawRectangle(x, startY + barHeight - valHeight, barWidth, valHeight, color);
        DrawText(label, x - 5, startY + barHeight + 10, 20, color);
        x -= spacing;
    };

    drawBar("E", powers->getAnimated(PowerType::ECONOMY), BLUE);
    drawBar("C", powers->getAnimated(PowerType::AWARENESS), GREEN);
    drawBar("Co", powers->getAnimated(PowerType::TRASH_COLLECTION), ORANGE);
    drawBar("L", powers->getAnimated(PowerType::TRASH_ACCUMULATION), RED);
}

// ---------------- DESENHO PRINCIPAL ----------------
void World::draw() {
    BeginDrawing();
    ClearBackground(WHITE);

    screen->render();

    if(!screen->isPopupActive()){
        drawPowers();
        if (animation) {
            animation->draw();
        }
    }

    DrawFPS(20, 20);
    EndDrawing();
}
