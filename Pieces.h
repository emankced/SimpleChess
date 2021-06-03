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
        int lastTurnMoved = -1;
        int moveCount = 0;

    public:
        Piece(Color color);
        virtual ~Piece() = default;

        const Color getColor() const;
        int getLastTurnMoved() const;
        int getMoveCount() const;
        void countMove(int turn);

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board, int turn) = 0;
        virtual const char getChar() const = 0;
        virtual bool move(int srcX, int srcY, int dstX, int dstY, Board &board, int turn);
    };

    class Pawn : public Piece {
    public:
        Pawn(Color color);
        virtual ~Pawn() = default;

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board, int turn);
        virtual const char getChar() const;
        virtual bool move(int srcX, int srcY, int dstX, int dstY, Board &board, int turn);
    };

    class Knight : public Piece {
    public:
        Knight(Color color);
        virtual ~Knight() = default;

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board, int turn);
        virtual const char getChar() const;
    };

    class Rogue : public Piece {
    public:
        Rogue(Color color);
        virtual ~Rogue() = default;

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board, int turn);
        virtual const char getChar() const;
    };

    class Bishop : public Piece {
    public:
        Bishop(Color color);
        virtual ~Bishop() = default;

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board, int turn);
        virtual const char getChar() const;
    };

    class Queen : public Piece {
    public:
        Queen(Color color);
        virtual ~Queen() = default;

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board, int turn);
        virtual const char getChar() const;
    };

    class King : public Piece {
    public:
        King(Color color);
        virtual ~King() = default;

        virtual std::vector<std::pair<int, int>> getAllAvailableFields(int x, int y, Board const &board, int turn);
        virtual const char getChar() const;
        virtual bool move(int srcX, int srcY, int dstX, int dstY, Board &board, int turn);
    };
}

#endif
