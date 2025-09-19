// World.hpp
#ifndef ECOTIDES_WORLD_HPP
#define ECOTIDES_WORLD_HPP
#include "ui/card/Card.hpp"
#include "ui/animation/Animation.hpp"

#pragma once

#include <raylib-cpp.hpp>
#include "utils/enum.hpp"
#include "ui/screen/Screen.hpp"



class World {
public:
    World();
    ~World();
    void cardSwap();
    void update(float delta);
    void draw();

private:
    Card* card;
    Animation* animation;
    int dummy;
    int cardIndex = 0;
    int cardIds[4];
    Screens* screen;
};

#endif // ECOTIDES_WORLD_HPP
