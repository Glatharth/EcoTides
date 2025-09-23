#pragma once
#include <raylib-cpp.hpp>
#include <string>

class Card {
public:
    explicit Card(const std::string& imagePath);
    ~Card();

    void Draw(const raylib::Color& color = WHITE) const;
    void SetFilter(TextureFilter filter) { texture.SetFilter(filter); }

    const raylib::Texture& GetTexture() const { return texture; }

    static void UpdateScreenSize(const raylib::Vector2& newSize) {
        screenSize = newSize;
        UpdatePosition();
    }

    static raylib::Vector2 GetScreenSize() { return screenSize; }
    static raylib::Vector2 GetPosition() { return position; }
    static int GetSquareSize() { return squareSize; }

    std::string GetPath() const { return path; }
    bool IsLoaded() const { return loaded; }

    friend class Animation;

private:
    static void UpdatePosition() {
        position = raylib::Vector2{
            screenSize.x / 2.0f,
            (screenSize.y + squareSize) / 2.0f
        };
    }

    bool LoadImage();

    inline static raylib::Vector2 screenSize = {0.0f, 0.0f};
    inline static raylib::Vector2 position = {0.0f, 0.0f};
    static constexpr int squareSize = 7 * 64;

    std::string path;
    bool loaded = false;

    raylib::Image* image = nullptr;
    raylib::Texture texture{};
};