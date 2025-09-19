// World.hpp
#ifndef ECOTIDES_WORLD_HPP
#define ECOTIDES_WORLD_HPP

#pragma once

#include <raylib-cpp.hpp>
#include "utils/enum.hpp"
#include "ui/screen/Screen.hpp"



class World {
public:
    World();
    ~World();

    void update(float delta);
    void draw();

private:
    int dummy;
    Screens* screen;
};

#endif // ECOTIDES_WORLD_HPP
