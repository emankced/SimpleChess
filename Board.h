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
        std::vector<Piece*> fallenPieces;

    public:
        Board();
        Board(const Board &other);

        Piece const * at(int x, int y) const;
        Piece * & get(int x, int y);
        void set(int x, int y, Piece* piece);
        void promote(int x, int y, char c);
        bool isLocationEndangered(int x, int y, Color ownColor) const;
        bool isCheck(Color color) const;
        bool isMate(Color color, int turn) const;

        void addToFallenPieces(Piece* piece);
        std::vector<Piece*> getFallenPieces(Color color) const;
    };
}

#endif
