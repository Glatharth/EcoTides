
#include "World.hpp"
#include <iostream>
#include <raylib-cpp.hpp>
#include "ui/Powers/Powers.hpp"

World* globalWorldInstance = nullptr;

World::World()
    : screen(new Screens()), powers(new Powers()),
      card(nullptr), animation(nullptr),
      cardIndex(0), playerWon(false), playerLost(false),
      loader("src/xml/cards.xml") 
{
    globalWorldInstance = this;
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

// ------------------ Start Game ------------------
void World::startGame() {
    if (!loader.IsLoaded()) {
        std::cerr << "Erro: XML de cartas não carregado!" << std::endl;
        return;
    }

    // Carrega os paths das cartas
    for (int i = 0; i < 4; ++i)
        cardPaths[i] = loader.GetCardPath(i + 1);

    // Cria a primeira carta
    if (card) delete card;
    card = new Card(cardPaths[0]);

    // ✅ Atualiza o tamanho da tela na classe Card antes da animação
    Card::UpdateScreenSize(raylib::Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() });

    // Cria animação
    if (animation) delete animation;
    animation = new Animation(card);

    // Próxima carta
    cardIndex = 1;

    // Reseta poderes e flags
    powers->reset();
    playerWon = false;
    playerLost = false;

    // Muda a tela para GAME
    screen->change(ScreenState::GAME);
}

// ------------------ Swap Card ------------------
void World::cardSwap() {
    if (!animation || cardPaths.empty()) return;
    if (card) delete card;
    card = new Card(cardPaths[cardIndex]);
    animation->setCard(card);
    cardIndex = (cardIndex + 1) % 4;
}

// ------------------ Retry ------------------
void World::retry() {
    if (card) { delete card; card = nullptr; }
    if (animation) { delete animation; animation = nullptr; }
    startGame(); // reinicia o jogo
}

// ------------------ Update ------------------
void World::update(float delta) {
    ScreenState prev = screen->getCurrent();

    // Se entramos no GAME, mas não há carta, inicia o jogo
    if (screen->getCurrent() == ScreenState::GAME && card == nullptr) {
        startGame(); // cria a primeira carta e animação
    }

    // Atualiza animação de cartas
    if (animation) {
        animation->update(delta);
        if (animation->needsCardSwap()) {
            cardSwap();
            animation->resetSwap();
        }
    }

    // Atualiza a tela
    screen->update(delta);

    // Reset dos poderes ao entrar na tela de GAME
    if (screen->getCurrent() == ScreenState::GAME && prev != ScreenState::GAME) {
        powers->reset();
        playerWon = false;
        playerLost = false;
        // Reset de cartas
        if (card) delete card;
        card = new Card(cardPaths[0]);
        if (animation) animation->setCard(card);
        cardIndex = 1;
    }

    // Atualiza poderes (suavização/animados)
    powers->update(delta);

    // ---------------- Controles de teste (teclado) ----------------
    if (IsKeyPressed(KEY_Q)) powers->applyChange(PowerType::ECONOMY, +5);
    if (IsKeyPressed(KEY_A)) powers->applyChange(PowerType::ECONOMY, -5);
    if (IsKeyPressed(KEY_W)) powers->applyChange(PowerType::AWARENESS, +5);
    if (IsKeyPressed(KEY_S)) powers->applyChange(PowerType::AWARENESS, -5);
    if (IsKeyPressed(KEY_E)) powers->applyChange(PowerType::TRASH_COLLECTION, +5);
    if (IsKeyPressed(KEY_D)) powers->applyChange(PowerType::TRASH_COLLECTION, -5);
    if (IsKeyPressed(KEY_R)) powers->applyChange(PowerType::TRASH_ACCUMULATION, +5);
    if (IsKeyPressed(KEY_F)) powers->applyChange(PowerType::TRASH_ACCUMULATION, -5);

    // ---------------- Condições de vitória/derrota ----------------
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

    // Aplica mudança de tela
    if (playerWon && screen->getCurrent() != ScreenState::VICTORY)
        screen->change(ScreenState::VICTORY);
    else if (playerLost && screen->getCurrent() != ScreenState::DEFEAT)
        screen->change(ScreenState::DEFEAT);
}


// ------------------ Draw Powers ------------------
void World::drawPowers() {
    if (!powers) return;
    if (screen->getCurrent() != ScreenState::GAME) return;

    int startX = 50;
    int startY = GetScreenHeight() - 150;
    int size   = 64;
    int spacing = 80;

    powers->drawIcons(startX, startY, size, spacing);
}

// ------------------ Draw ------------------
void World::draw() {
    BeginDrawing();
    ClearBackground(BLUE);

    // Renderiza a tela atual (menu, vitória, derrota, opções, etc.)
    screen->render();

    // Desenha barras de poderes e carta apenas na tela de jogo
    if (!screen->isPopupActive() && screen->getCurrent() == ScreenState::GAME) {
        drawPowers();            // Barras de poderes
        if (animation) animation->draw();  // Carta animada
    }

    DrawFPS(20, 20);
    EndDrawing();
}