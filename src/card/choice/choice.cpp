#include "choice.hpp"

int Choice::computeChange(Decision decision, int baseValue, bool isLargeChange) {
    int changeAmount = isLargeChange ? largeChange : smallChange;
    if (decision == Decision::Yes) {
        return baseValue + changeAmount;
    }
    else {
        return baseValue - changeAmount;
    }
}