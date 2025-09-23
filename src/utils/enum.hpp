#pragma once

enum class ScreenState {
    MENU,
    GAME,
    VICTORY,
    DEFEAT,
    OPTIONS
};

enum class PowerType {
    ECONOMY,
    AWARENESS,
    TRASH_COLLECTION,
    TRASH_ACCUMULATION,
    COUNT
};

enum class EventType {
    CREATE,
    MODIFY,
    DELETE,
    UNKNOWN // caso não seja nenhum
};