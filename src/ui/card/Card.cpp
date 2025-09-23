#include "Card.hpp"
#include <iostream>

Card::Card(const std::string& imagePath) : path(imagePath) {
    screenSize = raylib::Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };
    UpdatePosition();

    loaded = LoadImage();
    if (!loaded) {
        std::cerr << "Failed to load card image: " << path << std::endl;
    }
}

Card::~Card() {
    if (texture.id > 0) texture.Unload();
    if (image) {
        image->Unload();
        delete image;
        image = nullptr;
    }
}

bool Card::LoadImage() {
    try {
        image = new raylib::Image(path);
        image->ResizeNN(squareSize, squareSize);
        texture = image->LoadTexture();
        return true;
    } catch (...) {
        return false;
    }
}

void Card::Draw(const raylib::Color& color) const {
    if (loaded) {
        texture.Draw(position, color);
    }
}