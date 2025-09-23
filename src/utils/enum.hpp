#pragma once

enum class ScreenState {
    MENU,
    GAME,
    VICTORY,
    DEFEAT,
    OPTIONS
};

enum class PowerType {
    ECONOMY = 0,
    AWARENESS,
    TRASH_COLLECTION,
    TRASH_ACCUMULATION,
    COUNT
};

enum class EventType {
    CREATE,
    MODIFY,
    DELETE,
    UNKNOWN
};