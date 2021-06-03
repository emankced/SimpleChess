#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>

#include "common.h"
#include "Pieces.h"

namespace SimpleChess {
    class Board {
    private:
        std::vector<std::unique_ptr<Piece>> pieces;
        Piece* board[BOARD_SIZE];

    public:
        Board();

        Piece const * at(int x, int y) const;
        Piece * & get(int x, int y);
        void set(int x, int y, Piece* piece);
        bool isLocationEndangered(int x, int y, Color ownColor, int turn) const;

        std::vector<Piece*> fallenPieces;
    };
}

#endif
