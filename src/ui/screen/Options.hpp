#pragma once

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "../../core/game/World.hpp"
#include <raylib-cpp.hpp>

class World;

class Options {
public:
    void draw(World &world);
};

#endif