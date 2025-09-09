#include "core/game/Window.hpp"

int main() {

    Window window(
        600,             // width
        800,             // height
        "EcoTides",   // title
        60,              // target FPS
        true,            // antialiasing
        false,           // resizable
        false,           // full screen
        false,           // undecorated
        false,           // always on top
        false,           // invisible background
        false,           // always run
        false,           // load resources
        false            // init audio
    );
    window.run();
    return 0;
}