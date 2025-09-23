#pragma once
#include <string>
#include <raylib-cpp.hpp>
#include "utils/enum.hpp"

class Powers {
public:
    Powers();
    void reset();
    void applyChange(PowerType type, int delta);
    int getValue(PowerType type) const;
    void update(float delta);
    void drawIcons(int startX, int startY, int size, int spacing);

private:
    std::string iconPaths[4];
    raylib::Texture icons[4]; 
    float value[4];
    float targetValue[4];
};