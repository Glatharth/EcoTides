// World.hpp
#ifndef ECOTIDES_WORLD_HPP
#define ECOTIDES_WORLD_HPP
#include "ui/card/CardManager.hpp"
#include "ui/animation/Animation.hpp"

class World {
public:
    World();
    ~World();
    void cardSwap();
    void update(float delta);
    void draw();

private:
    CardManager* card;
    Animation* animation;
    int dummy;
    int cardIndex = 0;
    int cardIds[4];
};

#endif // ECOTIDES_WORLD_HPP
