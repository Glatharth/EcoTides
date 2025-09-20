#include "Card.hpp"
#include <iostream>
#include "io/FileLoader.hpp"

Card::Card(int cardId) : id(cardId) {
   InitializeStatic();

   FileLoader fileLoader;

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

   if (!FileLoader::PathExists(path)) {
      std::cerr << "Error: Path does not exist: " << path << std::endl;
      return;
   }

   loaded = LoadImage();

   if (loaded) {
      std::cout << "Card " << cardId << " loaded successfully: " << path << std::endl;
   }
}

Card::~Card() {
   if (texture.id > 0) {
      texture.Unload();
   }
   if (image) {
      image->Unload();
      delete image;
   }
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