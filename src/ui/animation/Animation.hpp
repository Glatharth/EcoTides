#pragma once

class Card;
class Animation {
public:
    explicit Animation(Card* card);
    ~Animation();

    void update(float delta);
    void draw();

private:
    Card* card;
    float dragOffsetX = 0.0f;
    float maxRotation = 20.0f;
    bool dragging = false;

    float lastMouseX = 0.0f;
};
