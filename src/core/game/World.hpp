// World.hpp
#ifndef ECOTIDES_WORLD_HPP
#define ECOTIDES_WORLD_HPP

class World {
public:
    World();
    ~World();

    void update(float delta);
    void draw();

private:
    int dummy;
};

#endif // ECOTIDES_WORLD_HPP
