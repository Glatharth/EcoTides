#pragma once
#include <raylib.h>

class Card;

class Animation {
    enum AnimationState { Idle, Dragging, Discarding, Entering, Returning };

public:
    explicit Animation(Card* card);
    ~Animation();

    void setCard(Card* newCard);
    bool needsCardSwap() const;
    void resetSwap();
    void update(float delta);
    void draw();

private:
    Card* card;
    float dragOffsetX = 0.0f;
    float maxRotation = 5.0f;
    bool dragging = false;
    float lastMouseX = 0.0f;
    AnimationState state = Idle;
    float discardSpeed = 1500.0f;
    float enterSpeed = 1500.0f;
    bool cardSwapFlag = false;
};