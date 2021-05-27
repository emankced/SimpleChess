#ifndef CHESS_H
#define CHESS_H

#include <memory>
#include <vector>

#include "Pieces.h"

namespace SimpleChess {
    class Chess {
    private:
        std::vector<std::unique_ptr<Piece>> pieces;
        std::vector<Piece*> fallenPieces;
        Piece* board[BOARD_SIZE];

        int turn = 0;

    public:
        Chess();

        Piece const * const (& getBoard())[BOARD_SIZE];
        State getState();
        Color whoseTurnIsIt();
        int getTurn();
        bool move(int srcX, int srcY, int dstX, int dstY);
    };
}
#endif
