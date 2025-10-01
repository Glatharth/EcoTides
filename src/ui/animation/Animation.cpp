#include "Animation.hpp"
#include "ui/card/Card.hpp"
#include <cmath>
#include <algorithm>

/**
 * @class Animation
 * @brief Handles drag-and-drop card animation, including card discard logic and visual effects.
 *
 * This class manages the animation state of a Card object based on mouse input.
 * It supports dragging, discarding, entering new cards, and returning to idle.
 */

/// @brief Constructor.
/// @param card Pointer to the Card object this animation will control.
Animation::Animation(Card* card) : card(card) {}

/// @brief Destructor.
Animation::~Animation() {}

/// @brief Updates the animation logic based on the current state and user input.
/// @param delta Time (in seconds) since the last frame.
void Animation::update(float delta) {
    switch (state) {
        case Idle:
            // Begin dragging when left mouse button is pressed.
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                lastMouseX = GetMousePosition().x;
                dragging = true;
                state = Dragging;
            }
            break;

        case Dragging:
            //If left mouse button is pressed moves the card based on mouse position.
            //Else determines if the card is set to the discarding or returning state.
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
            // Moves card offscreen quickly
            dragOffsetX += (dragOffsetX > 0 ? 1 : -1) * discardSpeed * delta;
            if (fabs(dragOffsetX) > Card::GetScreenSize().x + Card::GetSquareSize()) {
                cardSwapFlag = true;
                state = Entering;
                dragOffsetX = -(dragOffsetX > 0 ? 1 : -1) * (Card::GetScreenSize().x + Card::GetSquareSize());
            }
            break;

        case Entering: {
            // Slides new card into the screen    
            float dir = (dragOffsetX > 0) ? -1 : 1;
            dragOffsetX += enterSpeed * delta * dir;
            if ((dir == -1 && dragOffsetX >= 0) || (dir == 1 && dragOffsetX <= 0)) {
                dragOffsetX = 0.0f;
                state = Idle;
            }
            break;
        }

        case Returning: {
            // Returns card to center
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

/// @brief Draws the card with appropriate transformations and overlay text.
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
        position.x - origin.x + adjustX,
        position.y - origin.y + adjustY,
        (float)tex.width,
        (float)tex.height
    };

    DrawTexturePro(tex, sourceRec, destRec, origin, rotation, WHITE);

    // Draw "SIM" or "NÃO" overlays depending on drag direction
    auto drawOverlay = [&](float factor, const char* text, Color color){
        factor = std::min(factor, 1.0f);
        Rectangle overlayRect = { destRec.x - 45, destRec.y - 420, 100, 50 };
        Color blackTransparent = { 0, 0, 0, (unsigned char)(factor * 255) };
        DrawRectangleRec(overlayRect, blackTransparent);
        DrawText(text, (int)(overlayRect.x + 10), (int)(overlayRect.y + 10), 40, ColorAlpha(color, factor));
    };

    if (normDrag > 0.5f) drawOverlay((normDrag - 0.5f) * 1.4f, "SIM", GREEN);
    else if (normDrag < -0.5f) drawOverlay((-normDrag - 0.5f) * 1.4f, "NÃO", RED);
}

/// @brief Sets a new card to animate.
/// @param newCard Pointer to the new Card.
void Animation::setCard(Card* newCard) { card = newCard; }

/// @brief Checks whether the current animation has triggered a card swap.
/// @return true if a new card should be loaded; false otherwise.
bool Animation::needsCardSwap() const { return cardSwapFlag; }

/// @brief Resets the swap flag to false after card change is handled.
void Animation::resetSwap() { cardSwapFlag = false; }