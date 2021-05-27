#include "Pieces.h"

using namespace SimpleChess;

Piece::Piece(Color color) : color(color) {}

const Color Piece::getColor() const {
    return this->color;
}

Pawn::Pawn(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Pawn::getAllAvailableFields(int x, int y, Piece const * const (&board)[BOARD_SIZE]) {
    std::vector<std::pair<int, int>> availableFields;

    int x_ = x;
    int y_ = y;
    auto validCoord = [&x_, &y_](){ return std::min(x_, y_) >= 0 && x_ < BOARD_WIDTH && y_ < BOARD_HEIGHT; };

    // test if the piece is at the position
    if(!validCoord() || board[x + y*BOARD_WIDTH] != this) {
        return availableFields;
    }

    int direction = (getColor() == white ? 1 : -1);
    y_ = y + direction;

    if(!validCoord() || board[x_ + y_*BOARD_WIDTH] != nullptr) {
        return availableFields;
    }

    availableFields.emplace_back(x_, y_);

    y_ += direction;
    if(validCoord() && !this->moved && board[x_ + y_*BOARD_WIDTH] == nullptr) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x+1;
    y_ = y + direction;
    if(validCoord() && board[x_ + y_*BOARD_WIDTH] != nullptr && board[x_ + y_*BOARD_WIDTH]->getColor() != this->getColor()) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x-1;
    if(validCoord() && board[x_ + y_*BOARD_WIDTH] != nullptr && board[x_ + y_*BOARD_WIDTH]->getColor() != this->getColor()) {
        availableFields.emplace_back(x_, y_);
    }

    y_ += direction;
    if(validCoord() && !this->moved && board[x_ + y_*BOARD_WIDTH] != nullptr && board[x_ + y_*BOARD_WIDTH]->getColor() != this->getColor()) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x+1;
    if(validCoord() && !this->moved && board[x_ + y_*BOARD_WIDTH] != nullptr && board[x_ + y_*BOARD_WIDTH]->getColor() != this->getColor()) {
        availableFields.emplace_back(x_, y_);
    }

    return availableFields;
}

const char Pawn::getChar() const {
    return 'p';
}
