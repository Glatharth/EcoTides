#pragma once
#ifndef ECOTIDES_CARD_HPP
#define ECOTIDES_CARD_HPP

#include <map>
#include <string>
#include <raylib-cpp.hpp>
#include "utils/enum.hpp"

class Card {
public:
    explicit Card(int cardId);
    ~Card();

    void Draw(const raylib::Color& color = WHITE) const;
    void SetFilter(TextureFilter filter);
    const raylib::Texture& GetTexture() const { return texture; }
    static void UpdateScreenSize(const raylib::Vector2& newSize);
    static raylib::Vector2 GetScreenSize();
    static raylib::Vector2 GetPosition();
    static int GetSquareSize();
    int GetId() const { return id; }
    std::string GetPath() const { return path; }
    EventType GetEventType() const { return eventType; }
    const std::map<ResourceType, int>& GetResourcesYes() const { return resourcesYes; }
    const std::map<ResourceType, int>& GetResourcesNo() const { return resourcesNo; }
    // int GetResource(ResourceType type) const;
    bool IsLoaded() const { return loaded; }

    [[nodiscard]] const std::string& getText() const;
    void setText(const std::string& newText);

    friend class Animation;

private:
    static void InitializeStatic();
    static void UpdatePosition();
    bool LoadImage();

    inline static raylib::Vector2 screenSize = {0, 0};
    inline static raylib::Vector2 position = {0, 0};
    static constexpr int squareSize = 7 * 64;
    inline static bool initialized = false;

    int id = -1;
    std::string path;
    EventType eventType;
    std::map<ResourceType, int> resourcesYes;
    std::map<ResourceType, int> resourcesNo;
    std::string text;
    bool loaded = false;

    raylib::Image* image = nullptr;
    raylib::Texture texture{};
};

#endif // ECOTIDES_CARD_HPP
