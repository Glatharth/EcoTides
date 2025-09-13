// World.hpp
#ifndef ECOTIDES_WORLD_HPP
#define ECOTIDES_WORLD_HPP
#include "../../ui/card/Card.hpp"
#include "../../ui/animation/Animation.hpp"

class World {
public:
    World();
    ~World();

    void update(float delta);
    void draw();

private:
    Card* card;
    Animation* animation;
    int dummy;
};

#endif // ECOTIDES_WORLD_HPP
