#include "common.h"

#include <algorithm>

bool SimpleChess::validCoordinate(int x, int y) {
    return std::min(x, y) >= 0 && x < BOARD_WIDTH && y < BOARD_HEIGHT;
};
