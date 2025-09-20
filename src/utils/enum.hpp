#pragma once

enum class ScreenState {
    MENU,
    GAME,
    OPTIONS,
    VICTORY,
    DEFEAT
};

enum class PowerType {
    ECONOMY,
    AWARENESS,
    TRASH_COLLECTION,
    TRASH_ACCUMULATION,
    COUNT // usado só para saber quantos são
};