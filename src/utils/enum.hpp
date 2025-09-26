#pragma once

enum class EventType {
    UNKNOWN,
    CREATE,
    MODIFY,
    DELETE,
};

enum class PowerType {
    ECONOMY = 0,
    AWARENESS,
    TRASH_COLLECTION,
    TRASH_ACCUMULATION,
    COUNT
};

enum class ScreenState {
    MENU,
    GAME,
    VICTORY,
    DEFEAT
};

enum class GameDifficulty {
    NORMAL,
    HARD,
    HARDCORE,
    ZOIO,
};