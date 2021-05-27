#ifndef PIECES_H
#define PIECES_H

#include <vector>

#include "common.h"

namespace SimpleChess {
    class Piece {
    private:
        const Color color;

    public:
        Piece(Color color);

        const Color getColor() const;
        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Piece const * const (&board)[BOARD_SIZE]) = 0;
        virtual const char getChar() const = 0;
    };

    class Pawn : public Piece {
    private:
        bool moved = false;

    public:
        Pawn(Color color);

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Piece const * const (&board)[BOARD_SIZE]);
        virtual const char getChar() const;
    };
}

#endif
