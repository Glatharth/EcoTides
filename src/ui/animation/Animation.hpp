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
};
