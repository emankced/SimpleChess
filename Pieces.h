#ifndef PIECES_H
#define PIECES_H

#include <vector>

#include "common.h"

namespace SimpleChess {
    class Piece {
    private:
        const Color color;

    protected:
        bool moved = false;

    public:
        Piece(Color color);
        virtual ~Piece() = default;

        const Color getColor() const;
        void setMoved();
        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Piece const * const (&board)[BOARD_SIZE]) = 0;
        virtual const char getChar() const = 0;
    };

    class Pawn : public Piece {
    public:
        Pawn(Color color);

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Piece const * const (&board)[BOARD_SIZE]);
        virtual const char getChar() const;
    };
}

#endif
