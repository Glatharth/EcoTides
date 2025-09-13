// World.hpp
#ifndef ECOTIDES_WORLD_HPP
#define ECOTIDES_WORLD_HPP

#pragma once

#include <raylib-cpp.hpp>
#include "utils/enum.hpp"
#include <memory>
class GameScreenClass;
class Options;
class Victory;
class Defeat;

class World {
public:
    World();
    ~World();

    void update(float delta);
    void draw();

private:
    void drawMenu(); 
    
public:
    GameScreen currentScreen;

private:
    int dummy;
    std::unique_ptr<GameScreenClass> game;
    std::unique_ptr<Options> options;
    std::unique_ptr<Victory> victory;
    std::unique_ptr<Defeat> defeat;

};

#endif // ECOTIDES_WORLD_HPP
