#pragma once
#ifndef ECOTIDES_CARD_HPP
#define ECOTIDES_CARD_HPP

#include <map>
#include <raylib-cpp.hpp>
#include <string>
#include "utils/enum.hpp"

class FileLoader;

class Card {
public:
    explicit Card(int cardId, FileLoader& loader);
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

    int GetId() const { return id; }
    std::string GetPath() const { return path; }
    EventType GetEventType() const { return eventType; }
    const std::map<ResourceType, int>& GetResources() const { return resources; }
    int GetResource(const ResourceType type) const {
        const auto it = resources.find(type);
        return (it != resources.end()) ? it->second : 0;
    }
    bool IsLoaded() const { return loaded; }

    friend class Animation;

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
            screenSize.x / 2.0f,
            (screenSize.y + squareSize) / 2.0f
        );
    }

    bool LoadImage();

    inline static raylib::Vector2 screenSize = {0, 0};
    inline static raylib::Vector2 position = {0, 0};
    static constexpr int squareSize = 7 * 64;
    inline static bool initialized = false;

    int id = -1;
    std::string path;
    EventType eventType;
    std::map<ResourceType, int> resources;
    bool loaded = false;

    raylib::Image* image = nullptr;
    raylib::Texture texture{};
};

#endif // ECOTIDES_CARD_HPP
