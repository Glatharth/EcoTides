#pragma once
#include <array>
#include "ui/Screens/Screens.hpp"
#include "ui/Powers/Powers.hpp"
#include <cstdint>
#include "ui/card/Card.hpp"
#include "ui/animation/Animation.hpp"
#include "io/FileLoader.hpp"
#include "utils/enum.hpp"

class World {
public:
    World();
    ~World();

    void startGame(); 
    void update(float delta);
    void draw();
    int nextCard();

    static int getNextCard() {
        if (seed.empty()) return 1;
        return seed[(currentIndex) % seed.size()];
    }

    ScreenState getCurrent() const;
    void retry();
    FileLoader cardXML;

private:
    Screens* screen;
    Powers* powers;

    Card* card;
    Animation* animation;

    bool playerWon;
    bool playerLost;

    void cardSwap();
    void drawPowers();

    static std::vector<uint8_t> seed;
    static size_t currentIndex;
};

extern World* globalWorldInstance;