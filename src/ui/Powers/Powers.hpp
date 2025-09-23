#pragma once
#include <string>
#include <raylib-cpp.hpp>
#include "utils/enum.hpp" // Aqui deve ter o enum PowerType

class Powers {
public:
    Powers();
    void reset();
    void applyChange(PowerType type, int delta);
    int getValue(PowerType type) const;
    void update(float delta);
    void drawIcons(int startX, int startY, int size, int spacing);

private:
    std::string iconPaths[4];    // Caminhos das imagens dos ícones
    raylib::Texture icons[4];    // Texturas dos ícones (corrigido)
    float value[4];              // Valor atual do poder
    float targetValue[4];        // Valor alvo para animação
};