#pragma once
#include <string>
class World;

class Window {
public:
    Window(int width, int height, const std::string& title, int targetFPS,
               bool antialiasing, bool resizable, bool fullScreen, bool undecorated,
               bool alwaysOnTop, bool invisibleBackground, bool alwaysRun,
               bool loadResources, bool initAudio);
    ~Window();

    void run();

private:
    int width, height, targetFPS;
    std::string title;
    bool antialiasing, resizable, fullScreen, undecorated, alwaysOnTop;
    bool invisibleBackground, alwaysRun, loadResources, initAudio;
    bool initialized;
    World* world;

    void init();
    void destroy();
};
