// World.hpp
#ifndef ECOTIDES_WORLD_HPP
#define ECOTIDES_WORLD_HPP
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

private:
    Screens* screen;
    Card* card;
    Animation* animation;

    int cardIds[4];
    int cardIndex;

    bool playerWon;
    bool playerLost;
    void cardSwap();
};

#endif