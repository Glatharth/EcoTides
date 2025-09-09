#include "Card.hpp"

void Card::Draw(const raylib::Color& color) const {
   texture.Draw(position, color);
}
