#include "Animation.hpp"
#include "ui/card/Card.hpp"
#include <raylib.h>

Animation::Animation(Card* card) : card(card) {}

Animation::~Animation() {}

void Animation::update(float delta) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float mouseX = GetMousePosition().x;
        if (!dragging) {
            lastMouseX = mouseX;
            dragging = true;
        }
        float deltaX = mouseX - lastMouseX;
        dragOffsetX += deltaX;

        float maxDrag = 150.0f;
        if (dragOffsetX > maxDrag) dragOffsetX = maxDrag;
        else if (dragOffsetX < -maxDrag) dragOffsetX = -maxDrag;

        lastMouseX = mouseX;
    } else {
        dragging = false;
        if (dragOffsetX > 0) {
            dragOffsetX -= 300.0f * delta;
            if (dragOffsetX < 0) dragOffsetX = 0;
        } else if (dragOffsetX < 0) {
            dragOffsetX += 300.0f * delta;
            if (dragOffsetX > 0) dragOffsetX = 0;
        }
    }
}
void Animation::draw() {
    float normDrag = dragOffsetX / 150.0f;
    float rotation = maxRotation * normDrag;

    raylib::Vector2 position = Card::GetPosition();
    position.x += dragOffsetX;

    Rectangle sourceRec = { 0, 0, (float)card->texture.width, (float)card->texture.height };
    Vector2 origin = { card->texture.width / 2.0f, (float)card->texture.height };

    float adjustX = 220.0f;
    float adjustY = 450.0f;

    Rectangle destRec = {
        position.x - origin.x + adjustX,
        position.y - origin.y + adjustY,
        (float)card->texture.width,
        (float)card->texture.height
    };

    DrawTexturePro(card->texture, sourceRec, destRec, origin, rotation, WHITE);
}

