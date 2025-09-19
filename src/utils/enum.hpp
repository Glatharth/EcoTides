#pragma once

enum class EventType {
    UNKNOWN,
    CREATE,
    MODIFY,
    DELETE,
};

enum class ScreenState {
    MENU,
    GAME,
    OPTIONS,
    VICTORY,
    DEFEAT
};
