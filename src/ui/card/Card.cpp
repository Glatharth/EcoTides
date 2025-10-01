#include "Card.hpp"
#include <iostream>
#include "io/FileLoader.hpp"
/**
 * @class Card
 * @brief Represents a game card loaded from XML with associated image, event type, and resources.
 *
 * The Card class loads card data (like texture path, event type, and resources)
 * from a FileLoader based on the card ID, and manages the image and texture used for rendering.
 */

 /**
 * @brief Constructs a Card by loading its data from an XML file via FileLoader.
 * 
 * @param cardId The unique identifier of the card.
 * @param loader Reference to the FileLoader used to retrieve XML card data.
 */
Card::Card(int cardId, FileLoader& loader) : id(cardId) {
    InitializeStatic();

    if (!loader.IsLoaded()) {
        std::cerr << "Error: Could not load XML file" << std::endl;
        return;
    }

    if (!loader.CardExists(cardId)) {
        std::cerr << "Error: Card ID " << cardId << " not found in XML" << std::endl;
        return;
    }

    path = loader.GetCardPath(cardId);
    eventType = loader.GetCardEventType(cardId);
    resources = loader.GetCardResources(cardId);

    if (!FileLoader::PathExists(path)) {
        std::cerr << "Error: Path does not exist: " << path << std::endl;
        return;
    }

    loaded = LoadImage();

    if (loaded) {
        std::cout << "Card " << cardId << " loaded successfully: " << path << std::endl;
    }
}

/**
 * @brief Destructor.
 * 
 * Unloads the texture and image if they were loaded.
 */
Card::~Card() {
    if (texture.id > 0) texture.Unload();
    if (image) { image->Unload(); delete image; }
}

/**
 * @brief Loads the card image and creates the texture.
 * 
 * @return true if the image was loaded successfully, false otherwise.
 */
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

/**
 * @brief Draws the card texture at its current position.
 * 
 * @param color The color tint to apply while drawing.
 */
void Card::Draw(const raylib::Color& color) const {
    if (loaded) texture.Draw(position, color);
}