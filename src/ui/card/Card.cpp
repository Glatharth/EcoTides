#include "Card.hpp"
#include "core/game/World.hpp"
#include "io/FileLoader.hpp"
#include <iostream>

Card::Card(int cardId) : id(cardId) {
    InitializeStatic();
    const FileLoader& fileLoader = globalWorldInstance->cardXML;
    if (!fileLoader.IsLoaded()) {
        std::cerr << "Error: Could not load XML file" << std::endl;
        return;
    }
    if (!fileLoader.CardExists(cardId)) {
        std::cerr << "Error: Card ID " << cardId << " not found in XML" << std::endl;
        return;
    }

    path = fileLoader.GetCardPath(cardId);
    eventType = fileLoader.GetCardEventType(cardId);
    resourcesYes = fileLoader.GetCardResourcesYes(cardId);
    resourcesNo = fileLoader.GetCardResourcesYes(cardId);
    text = fileLoader.GetCardText(cardId);

    if (!FileLoader::PathExists(path)) {
        std::cerr << "Error: Path does not exist " << path << std::endl;
        return;
    }

    loaded = LoadImage();
}

Card::~Card() {
    if (texture.id > 0) texture.Unload();
    delete image;
}

bool Card::LoadImage() {
    try {
        image = new raylib::Image(path);
        image->ResizeNN(squareSize, squareSize);
        texture = image->LoadTexture();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading image: " << e.what() << std::endl;
        return false;
    }
}

void Card::Draw(const raylib::Color& color) const {
    if (loaded) {
        texture.Draw(position, color);
    }
}

void Card::InitializeStatic() {
    if (!initialized) {
        screenSize = raylib::Vector2(GetScreenWidth(), GetScreenHeight());
        UpdatePosition();
        initialized = true;
    }
}

void Card::UpdatePosition() {
    position = raylib::Vector2{
        (screenSize.x - squareSize) / 2,
        (screenSize.y - squareSize) / 2
    };
}

void Card::SetFilter(TextureFilter filter) {
    texture.SetFilter(filter);
}

void Card::UpdateScreenSize(const raylib::Vector2& newSize) {
    screenSize = newSize;
    UpdatePosition();
}

// int Card::GetResource(const ResourceType type) const {
//     const auto it = resources.find(type);
//     return (it != resources.end()) ? it->second : 0;
// }

const std::string& Card::getText() const {
    return this->text;
}

void Card::setText(const std::string& newText) {
    this->text = newText;
}

raylib::Vector2 Card::GetScreenSize() {
    return screenSize;
}

raylib::Vector2 Card::GetPosition() {
    return position;
}

int Card::GetSquareSize() {
    return squareSize;
}
