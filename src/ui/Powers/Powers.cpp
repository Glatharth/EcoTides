#include "Powers.hpp"
#include <iostream>
#include <raylib-cpp.hpp>

/**
 * @class Powers
 * @brief Manages the resource bars (powers) such as economy, awareness, collection, and accumulation.
 * 
 * The Powers class maintains a set of four resource values with icons and supports their visual update,
 * animation (toward target values), and rendering on screen.
 */

/**
 * @brief Constructor. Initializes icons, values, and attempts to load textures for power icons.
 * 
 * Sets default values for each power to 50 and loads their associated icon textures from disk.
 * If any texture fails to load, logs the error to std::cerr.
 */
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

/**
 * @brief Resets all resource values and their targets to the default (50).
 */
void Powers::reset() {
    for (int i = 0; i < 4; ++i) {
        value[i] = 50;
        targetValue[i] = 50;
    }
}

/**
 * @brief Applies a change to a specific resource type.
 * 
 * The actual change is applied to the target value, allowing animated interpolation in update().
 * 
 * @param type The resource type to modify.
 * @param delta The change to apply (positive or negative).
 */
void Powers::applyChange(ResourceType type, int delta) {
    int idx = static_cast<int>(type);
    targetValue[idx] += delta;
    if (targetValue[idx] < 0) targetValue[idx] = 0;
    if (targetValue[idx] > 100) targetValue[idx] = 100;
}

/**
 * @brief Gets the current (animated) value of the specified resource.
 * 
 * @param type The resource type.
 * @return int The current interpolated value.
 */
int Powers::getValue(ResourceType type) const {
    return value[static_cast<int>(type)];
}

/**
 * @brief Smoothly updates current resource values toward their target values.
 * 
 * This function simulates an animation effect by moving the current value closer to the target value over time.
 * 
 * @param delta Time delta (in seconds) since the last frame.
 */
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

/**
 * @brief Draws the icons and corresponding progress bars for each resource.
 * 
 * Each icon is drawn at a calculated position, scaled to fit, with a colored progress bar below it.
 * 
 * @param startX X position for the first icon.
 * @param startY Y position for all icons.
 * @param size Base size (in pixels) of each icon.
 * @param spacing Spacing between each icon.
 */
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