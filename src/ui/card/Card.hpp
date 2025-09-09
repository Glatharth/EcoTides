#pragma once
#ifndef ECOTIDES_CARD_HPP
#define ECOTIDES_CARD_HPP

#include <raylib-cpp.hpp>

class Card {
public:
    explicit Card(const std::string& filePath) {
        InitializeStatic();
        image = new raylib::Image(filePath);
        image->ResizeNN(squareSize, squareSize);
        texture = image->LoadTexture();
        // texture.SetFilter(TEXTURE_FILTER_BILINEAR); // Não lembro se precisa incluir ou se era teste :)
    }

    ~Card() {
        texture.Unload();
        image->Unload();
        delete image;
    }

    void Draw(const raylib::Color& color = WHITE) const;

    static void UpdateScreenSize(const raylib::Vector2& newSize) {
        screenSize = newSize;
        UpdatePosition();
    }

    void SetFilter(TextureFilter filter) {
        texture.SetFilter(filter);
    }

    static raylib::Vector2 GetScreenSize() { return screenSize; }
    static raylib::Vector2 GetPosition() { return position; }
    static int GetSquareSize() { return squareSize; }

private:
    static void InitializeStatic() {
        if (!initialized) {
            screenSize.x = static_cast<float>(GetScreenWidth());
            screenSize.y = static_cast<float>(GetScreenHeight());
            UpdatePosition();
            initialized = true;
        }
    }

    static void UpdatePosition() {
        position = raylib::Vector2(
            (screenSize.x - squareSize) / 2.0f,
            (screenSize.y - squareSize) / 2.0f
        );
    }

    inline static raylib::Vector2 screenSize = {0, 0};
    inline static raylib::Vector2 position = {0, 0};
    static constexpr int squareSize = 7 * 64;
    inline static bool initialized = false;

    raylib::Image* image;
    raylib::Texture texture{};
};

#endif // ECOTIDES_CARD_HPP