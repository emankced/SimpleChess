#ifndef CHESS_H
#define CHESS_H

#include "Pieces.h"
#include "Board.h"

namespace SimpleChess {
    class Chess {
    private:
        Board board;
        int turn = 0;

    public:
        Board const & getBoard() const;
        State getState();
        Color whoseTurnIsIt();
        int getTurn();
        bool move(int srcX, int srcY, int dstX, int dstY);
    };
}
#endif
