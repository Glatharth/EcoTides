#pragma once
#include <array>
#include <string>
#include "ui/Screens/Screens.hpp"
#include "ui/Powers/Powers.hpp"
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

    ScreenState getCurrent() const;
    void retry();

private:
    Screens* screen;
    Powers* powers;

    Card* card;
    Animation* animation;

    std::array<int, 4> cardIds;
    int cardIndex;
    
    bool playerWon;
    bool playerLost;

    FileLoader loader;

    void cardSwap();
    void drawPowers();
};

extern World* globalWorldInstance;