#include "Powers.hpp"
#include <iostream>
#include <raylib-cpp.hpp>

Powers::Powers() {

    iconPaths[0] = "assets/cifrao.png";
    iconPaths[1] = "assets/conscientizacao.png";
    iconPaths[2] = "assets/coleta.png";
    iconPaths[3] = "assets/lixeira.png";

    for (int i = 0; i < 4; ++i) {
        value[i] = 50;
        targetValue[i] = 50;
    }


    for (int i = 0; i < 4; ++i) {
        try {
            icons[i].Load(iconPaths[i]);
        } catch (const raylib::RaylibException &e) {
            std::cerr << "Erro ao carregar ícone: " << iconPaths[i]
                      << " | " << e.what() << std::endl;
        }
    }
}

void Powers::reset() {
    for (int i = 0; i < 4; ++i) {
        value[i] = 50;
        targetValue[i] = 50;
    }
}

void Powers::applyChange(PowerType type, int delta) {
    int idx = static_cast<int>(type);
    targetValue[idx] += delta;
    if (targetValue[idx] < 0) targetValue[idx] = 0;
    if (targetValue[idx] > 100) targetValue[idx] = 100;
}

int Powers::getValue(PowerType type) const {
    return value[static_cast<int>(type)];
}

void Powers::update(float delta) {
    const float speed = 50.0f * delta;
    for (int i = 0; i < 4; ++i) {
        if (value[i] < targetValue[i]) {
            value[i] += speed;
            if (value[i] > targetValue[i]) value[i] = targetValue[i];
        } else if (value[i] > targetValue[i]) {
            value[i] -= speed;
            if (value[i] < targetValue[i]) value[i] = targetValue[i];
        }
    }
}

void Powers::drawIcons(int startX, int startY, int size, int spacing) {
    const int iconSpacing = spacing + 60;
    const float iconScaleFactor = 1.3f;

    for (int i = 0; i < 4; ++i) {
        if (!icons[i].id) continue;

        float scale = (static_cast<float>(size) / icons[i].width) * iconScaleFactor;
        float posX = static_cast<float>(startX + i * iconSpacing);
        float posY = static_cast<float>(startY);

        DrawTextureEx(icons[i], { posX, posY }, 0.0f, scale, WHITE);

        int barWidth = static_cast<int>(size * iconScaleFactor);
        int barHeight = 12;
        float filled = value[i] / 100.0f;
        DrawRectangle(posX, posY + size * iconScaleFactor + 5, barWidth, barHeight, GRAY);
        DrawRectangle(posX, posY + size * iconScaleFactor + 5, barWidth * filled, barHeight, GREEN);
    }
}