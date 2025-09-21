#include "Animation.hpp"
#include "ui/card/Card.hpp"
#include <raylib.h>

Animation::Animation(Card* card) : card(card) {}

Animation::~Animation() {}

void Animation::update(float delta) {
    switch (state) {
        case Idle:
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                float mouseX = GetMousePosition().x;
                lastMouseX = mouseX;
                dragging = true;
                state = Dragging;
            }
            break;

        case Dragging:
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                float mouseX = GetMousePosition().x;
                float deltaX = mouseX - lastMouseX;
                dragOffsetX += deltaX;
                float maxDrag = 250.0f;
                if (dragOffsetX > maxDrag) dragOffsetX = maxDrag;
                else if (dragOffsetX < -maxDrag) dragOffsetX = -maxDrag;
                lastMouseX = mouseX;
            } else {
                dragging = false;
                constexpr float discardThreshold = 200.0f;
                if (fabs(dragOffsetX) > discardThreshold) {
                    state = Discarding;
                } else if (fabs(dragOffsetX) > 1.0f) {
                    state = Returning;
                } else {
                    dragOffsetX = 0.0f;
                    state = Idle;
                }
            }
            break;

        case Discarding:
            dragOffsetX += (dragOffsetX > 0 ? 1 : -1) * discardSpeed * delta;
            if (fabs(dragOffsetX) > Card::GetScreenSize().x + Card::GetSquareSize()) {
                cardSwapFlag = true;
                state = Entering;
                dragOffsetX = -(dragOffsetX > 0 ? 1 : -1) * (Card::GetScreenSize().x + Card::GetSquareSize());
            }
            break;

        case Entering:{
            float dir = (dragOffsetX > 0) ? -1 : 1;
            dragOffsetX += enterSpeed * delta * dir;
            if ((dir == -1 && dragOffsetX >= 0) || (dir == 1 && dragOffsetX <= 0)) {
                dragOffsetX = 0.0f;
                state = Idle;
            }
            break;
        }
        case Returning: {
        float returnSpeed = 600.0f;
        
        float direction = (dragOffsetX > 0) ? -1.0f : 1.0f;

        dragOffsetX += direction * returnSpeed * delta;

        if ((direction < 0 && dragOffsetX < 0) || (direction > 0 && dragOffsetX > 0) || fabs(dragOffsetX) < 1.0f) {
            dragOffsetX = 0.0f;
            state = Idle;
        }
        break;
        }

    }
}

void Animation::draw() {
    float normDrag = dragOffsetX / 250.0f;
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

    if (normDrag > 0.5f) {
        float alphaFactor = (normDrag - 0.5f) * 1.4f;
        if (alphaFactor > 1.0f) alphaFactor = 1.0f;

        Rectangle overlayRect = { destRec.x - 45, destRec.y - 420, 100, 50 };
        Color blackTransparent = { 0, 0, 0, (unsigned char)(alphaFactor * 255) };
        DrawRectangleRec(overlayRect, blackTransparent);

        DrawText("SIM", (int)(overlayRect.x + 10), (int)(overlayRect.y + 10), 40, ColorAlpha(GREEN, alphaFactor));
    }
    else if (normDrag < -0.5f) {
        float alphaFactor = (-normDrag - 0.5f) * 1.4f;
        if (alphaFactor > 1.0f) alphaFactor = 1.0f;

        Rectangle overlayRect = { destRec.x - 45, destRec.y - 420, 100, 50 };
        Color blackTransparent = { 0, 0, 0, (unsigned char)(alphaFactor * 255) };
        DrawRectangleRec(overlayRect, blackTransparent);

        DrawText("NÃO", (int)(overlayRect.x + 10), (int)(overlayRect.y + 10), 40, ColorAlpha(RED, alphaFactor));
    }

}

void Animation::setCard(Card* newCard) {
    card = newCard;
}

bool Animation::needsCardSwap() const {
    return cardSwapFlag;
}

void Animation::resetSwap() {
    cardSwapFlag = false;
}
