#ifndef WORLD_HPP
#define WORLD_HPP

#include <raylib-cpp.hpp>
#include "ui/Screens/Screens.hpp"
#include "ui/Powers/Powers.hpp"

class World {
public:
    World();
    ~World();

    void update(float delta);
    void draw();

    ScreenState getCurrent() const;

    void retry();

private:
    Screens* screen;
    Powers* powers;
    int dummy;

    void drawPowers(); // desenha as barras de poderes
};

#endif // WORLD_HPP

