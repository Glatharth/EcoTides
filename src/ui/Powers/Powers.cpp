#include "Powers.hpp"

Powers::Powers() {
    for (auto& p : powers) {
        p.value = 50;    // começa no meio
        p.animated = 50;
    }
}

void Powers::update(float delta) {
    for (auto& p : powers) {
        // interpolação suave para o animado
        p.animated += (p.value - p.animated) * 5.0f * delta;
    }
}

void Powers::change(PowerType type, float amount) {
    int i = (int)type;
    powers[i].value += amount;
    if (powers[i].value > 100) powers[i].value = 100;
    if (powers[i].value < 0)   powers[i].value = 0;
}



float Powers::get(PowerType type) const {
    return powers[(int)type].value;
}

float Powers::getAnimated(PowerType type) const {
    return powers[(int)type].animated;
}

// ---------------- reset ----------------
void Powers::reset() {
    for (auto& p : powers) {
        p.value = 50;      // valor inicial
        p.animated = 50;   // valor animado também
    }
}