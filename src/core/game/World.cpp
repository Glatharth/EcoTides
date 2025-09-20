#include "World.hpp"

World::World() : dummy(0) {
    screen = new Screens();
    powers = new Powers();
}

World::~World() {
    delete screen;
    delete powers;
}

ScreenState World::getCurrent() const {
    return screen->getCurrent();
}

void World::update(float delta){
    ScreenState prev = screen->getCurrent();

    screen->update(delta);

    // ALTERADO: Reset dos poderes ao entrar em GAME (START ou RETRY)
    if(screen->getCurrent() == ScreenState::GAME && prev != ScreenState::GAME){
        powers->reset();
        return; // não processa derrota neste frame
    }

    powers->update(delta);

    // Controles de teste
    if(IsKeyPressed(KEY_Q)) powers->change(PowerType::ECONOMY,+5);
    if(IsKeyPressed(KEY_A)) powers->change(PowerType::ECONOMY,-5);
    if(IsKeyPressed(KEY_W)) powers->change(PowerType::AWARENESS,+5);
    if(IsKeyPressed(KEY_S)) powers->change(PowerType::AWARENESS,-5);
    if(IsKeyPressed(KEY_E)) powers->change(PowerType::TRASH_COLLECTION,+5);
    if(IsKeyPressed(KEY_D)) powers->change(PowerType::TRASH_COLLECTION,-5);
    if(IsKeyPressed(KEY_R)) powers->change(PowerType::TRASH_ACCUMULATION,+5);
    if(IsKeyPressed(KEY_F)) powers->change(PowerType::TRASH_ACCUMULATION,-5);

    // ALTERADO: Verifica derrota somente após reset
    if(screen->getCurrent()==ScreenState::GAME){
        if(powers->get(PowerType::TRASH_ACCUMULATION) >= 100 ||
           powers->get(PowerType::ECONOMY) <= 0 ||
           powers->get(PowerType::AWARENESS) <= 0 ||
           powers->get(PowerType::TRASH_COLLECTION) <= 0){
            screen->change(ScreenState::DEFEAT);
        }
    }
}

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
        // fundo
        DrawRectangle(x, startY, barWidth, barHeight, Fade(LIGHTGRAY, 0.3f));

        // valor animado
        int valHeight = (barHeight * animatedValue) / 100;
        DrawRectangle(x, startY + barHeight - valHeight, barWidth, valHeight, color);

        // rótulo
        DrawText(label, x - 5, startY + barHeight + 10, 20, color);

        x -= spacing;
    };

    drawBar("E", powers->getAnimated(PowerType::ECONOMY), BLUE);
    drawBar("C", powers->getAnimated(PowerType::AWARENESS), GREEN);
    drawBar("Co", powers->getAnimated(PowerType::TRASH_COLLECTION), ORANGE);
    drawBar("L", powers->getAnimated(PowerType::TRASH_ACCUMULATION), RED);
}

void World::draw() {
    BeginDrawing();
    ClearBackground(WHITE);

    screen->render();

    if(!screen->isPopupActive()){
        drawPowers();
    }

    DrawFPS(20, 20);
    EndDrawing();
}