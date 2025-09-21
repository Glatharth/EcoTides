#ifndef WORLD_HPP
#define WORLD_HPP

#include <raylib-cpp.hpp>
#include "ui/Screens/Screens.hpp"
#include "ui/Powers/Powers.hpp"
#include "ui/card/Card.hpp"
#include "ui/animation/Animation.hpp"
#include "utils/enum.hpp"

#pragma once

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
    int dummy;

    void drawPowers(); // desenha as barras de poderes




    Card* card;
    Animation* animation;

    int cardIds[4];
    int cardIndex;

    bool playerWon;
    bool playerLost;
    void cardSwap();
};
#endif // WORLD_HPP