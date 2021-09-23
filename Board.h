#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>

#include "common.h"
#include "Pieces.h"

namespace SimpleChess {
    class Board {
    private:
        std::unique_ptr<Piece> board[BOARD_SIZE];

    public:
        Board();
        Board(const Board &other);

        const std::unique_ptr<Piece> & at(int x, int y) const;
        std::unique_ptr<Piece> & get(int x, int y);
        void set(int x, int y, std::unique_ptr<Piece> & piece);
        void promote(int x, int y, char c);
        bool isLocationEndangered(int x, int y, Color ownColor) const;
        bool isCheck(Color color) const;
        bool isMate(Color color, int turn) const;
    };
}

#endif
