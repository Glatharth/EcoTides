#pragma once

enum class EventType {
    UNKNOWN,
    CREATE,
    MODIFY,
    DELETE,
};

enum class ResourceType {
    ECONOMY,
    POPULATION_AWARENESS,
    WASTE_COLLECTION,
    WASTE_ACCUMULATION
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