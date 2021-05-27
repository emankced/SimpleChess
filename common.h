#ifndef COMMON_H
#define COMMON_H

namespace SimpleChess {
    const int BOARD_WIDTH = 8;
    const int BOARD_HEIGHT = 8;
    const int BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;
    enum Color { white = 0, black = 1 };
    enum State { normal, chess, chessMate, stallMate, wish };
}

#endif
