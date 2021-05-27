#include "Pieces.h"

using namespace SimpleChess;

Piece::Piece(Color color) : color(color) {}

Color Piece::getColor() {
    return this->color;
}

Pawn::Pawn(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Pawn::getAllAvailableFields(int x, int y, Piece const * const (&board)[BOARD_SIZE]) {
    std::vector<std::pair<int, int>> availableFields;

    // test if the piece is at the position
    if(board[x + y*BOARD_WIDTH] != this) {
        return availableFields;
    }

    // TODO

    return availableFields;
}

const char Pawn::getChar() const {
    return 'p';
}
