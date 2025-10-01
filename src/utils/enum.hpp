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

enum class CardIntensityValue {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3
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