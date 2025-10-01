#include "World.hpp"
#include "Window.hpp"
#include <raylib-cpp.hpp>

/**
*@class Window
*@brief Represents the game window with customizable settings and an internal game loop.
*/

/**
* @brief Constructs a new Window object with the specified settings.
*/
Window::Window(int width, int height, const std::string& title, int targetFPS,
                       bool antialiasing, bool resizable, bool fullScreen, bool undecorated,
                       bool alwaysOnTop, bool invisibleBackground, bool alwaysRun,
                       bool loadResources, bool initAudio)
    : width(width), height(height), title(title), targetFPS(targetFPS),
      antialiasing(antialiasing), resizable(resizable), fullScreen(fullScreen),
      undecorated(undecorated), alwaysOnTop(alwaysOnTop),
      invisibleBackground(invisibleBackground), alwaysRun(alwaysRun),
      loadResources(loadResources), initAudio(initAudio), initialized(false), world(nullptr) {}

/**
 * @brief Destroys the Window object and frees associated resources.
 */
Window::~Window() {
    destroy();
}

/**
 * @brief Initializes the Window, starts the game loop and, when it
 * finishes, the window will be finished and destroyed too.
 */
void Window::init() {
    if (initialized) return;
    initialized = true;

    if (antialiasing) SetConfigFlags(FLAG_MSAA_4X_HINT);
    if (resizable) SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (fullScreen) SetConfigFlags(FLAG_FULLSCREEN_MODE);
    if (undecorated) SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    if (alwaysOnTop) SetConfigFlags(FLAG_WINDOW_TOPMOST);
    if (invisibleBackground) SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    if (alwaysRun) SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);

    InitWindow(width, height, title.c_str());
    if (initAudio) InitAudioDevice();
    SetTargetFPS(targetFPS);

    world = new World();
}

/**
 * @brief Initializes the Window, starts the game loop.
 */
void Window::run() {
    init();
    while (!WindowShouldClose()) {
        world->update(GetFrameTime());
        world->draw();
    }
    destroy();
}

/**
* @brief Destroys the window and stops the audio.
*/
void Window::destroy() {
    if (world) {
        delete world;
        world = nullptr;
    }
    if (initAudio) CloseAudioDevice();
    if (initialized) {
        CloseWindow();
        initialized = false;
    }
}
