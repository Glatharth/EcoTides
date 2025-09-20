#pragma once

#include "utils/enum.hpp"   // onde estão os enums ScreenState e PowerType
#include <array>

// Estrutura que guarda o valor real e o valor animado de cada poder
struct PowerData {
    float value;       // valor real (0-100)
    float animated;    // valor interpolado (suave)
};

class Powers {
public:
    Powers();

    void update(float delta);                       // atualiza animação dos valores
    void change(PowerType type, float amount);      // altera valor (+ ou -)
    float get(PowerType type) const;                // valor real
    float getAnimated(PowerType type) const;        // valor animado (suave)

    void reset();

private:
    std::array<PowerData, (int)PowerType::COUNT> powers;
};
