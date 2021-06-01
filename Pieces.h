#ifndef PIECES_H
#define PIECES_H

#include <vector>

#include "common.h"

namespace SimpleChess {
    class Board;

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
        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board) = 0;
        virtual const char getChar() const = 0;
    };

    class Pawn : public Piece {
    public:
        Pawn(Color color);
        virtual ~Pawn() = default;

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board);
        virtual const char getChar() const;
    };
}

#endif
