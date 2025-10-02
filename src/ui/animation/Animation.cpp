#include "Animation.hpp"
#include "ui/card/Card.hpp"
#include "utils/utils.hpp"
#include <cmath>
#include <algorithm>

Animation::Animation(Card* card) : card(card) {}

Animation::~Animation() {}

void Animation::update(float delta) {
    switch (state) {
        case Idle:
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                lastMouseX = GetMousePosition().x;
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
                dragOffsetX = std::clamp(dragOffsetX, -maxDrag, maxDrag);
                lastMouseX = mouseX;
            } else {
                dragging = false;
                constexpr float discardThreshold = 200.0f;
                if (fabs(dragOffsetX) > discardThreshold) state = Discarding;
                else if (fabs(dragOffsetX) > 1.0f) state = Returning;
                else { dragOffsetX = 0.0f; state = Idle; }
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

        case Entering: {
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
            if ((direction < 0 && dragOffsetX <= 0) || (direction > 0 && dragOffsetX >= 0) || fabs(dragOffsetX) < 1.0f) {
                dragOffsetX = 0.0f;
                state = Idle;
            }
            break;
        }
    }
}

void Animation::draw() {
    if (!card || !card->IsLoaded()) return;

    float normDrag = dragOffsetX / 250.0f;
    float rotation = maxRotation * normDrag;

    raylib::Vector2 position = Card::GetPosition();
    position.x += dragOffsetX;

    const raylib::Texture& tex = card->GetTexture();
    Rectangle sourceRec = { 0, 0, (float)tex.width, (float)tex.height };
    Vector2 origin = { (float)tex.width / 2.0f, (float)tex.height };

    float adjustX = 220.0f, adjustY = 450.0f;

    Rectangle destRec = {
        position.x + adjustX,
        position.y + adjustY,
        (float)tex.width,
        (float)tex.height
    };

    DrawTexturePro(tex, sourceRec, destRec, origin, rotation, WHITE);

    auto drawOverlay = [&](float factor, const char* text, Color color){
        factor = std::min(factor, 1.0f);
        Rectangle overlayRect = { destRec.x - 45, destRec.y - 420, 100, 50 };
        Color blackTransparent = { 0, 0, 0, (unsigned char)(factor * 255) };
        DrawRectangleRec(overlayRect, blackTransparent);
        DrawText(text, (int)(overlayRect.x + 10), (int)(overlayRect.y + 10), 40, ColorAlpha(color, factor));
    };

    if (normDrag > 0.5f) drawOverlay((normDrag - 0.5f) * 1.4f, "SIM", GREEN);
    else if (normDrag < -0.5f) drawOverlay((-normDrag - 0.5f) * 1.4f, "NÃO", RED);

    const std::string& cardText = card->getText();
    if (!cardText.empty()) {
        Font font = GetFontDefault();
        float fontSize = 20.0f;
        float lineSpacing = 5.0f;
        Vector2 screenSize = Card::GetScreenSize();
        float textMaxWidth = screenSize.x * 0.8f;
        Vector2 textPosition = {
            (screenSize.x - textMaxWidth) / 2.0f,
            screenSize.y * 0.025f
        };
        Rectangle bgRect = { textPosition.x - 20, textPosition.y - 10, textMaxWidth + 40, 160 };
        DrawRectangleRec(bgRect, Fade(BLACK, 0.6f));
        DrawTextWrapped(font, cardText.c_str(), textPosition, textMaxWidth, fontSize, lineSpacing, WHITE);
    }
}

void Animation::setCard(Card* newCard) { card = newCard; }

bool Animation::needsCardSwap() const { return cardSwapFlag; }

void Animation::resetSwap() { cardSwapFlag = false; }
