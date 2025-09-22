#pragma once
#include <cstdint>
#include <raylib-cpp.hpp>
#include "ui/card/Card.hpp"
#include "ui/animation/Animation.hpp"
#include "utils/enum.hpp"
#include "ui/screen/Screen.hpp"

#pragma once

class World {
public:
    World();
    ~World();

    void startGame(); 
    void update(float delta);
    void draw();
    static int nextCard() {
        if (seed.empty()) return 1;

        if (currentIndex == seed.size()) {
            return 0;
        }

        const uint8_t currentId = seed[currentIndex];

        currentIndex = currentIndex + 1;

        return currentId;
    }

    static int getNextCard() {
        if (seed.empty()) return 1;
        return seed[(currentIndex) % seed.size()]; // Próximo card
    }

private:
    Screens* screen;
    Card* card;
    Animation* animation;

    bool playerWon;
    bool playerLost;
    void cardSwap();

    static std::vector<uint8_t> seed;
    static size_t currentIndex;
};