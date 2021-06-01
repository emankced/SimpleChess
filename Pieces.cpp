#include "Pieces.h"

#include "Board.h"

using namespace SimpleChess;

Piece::Piece(Color color) : color(color) {}

const Color Piece::getColor() const {
    return this->color;
}

void Piece::setMoved() {
    this->moved = true;
}

Pawn::Pawn(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Pawn::getAllAvailableFields(int x, int y, Board const &board) {
    std::vector<std::pair<int, int>> availableFields;

    int x_ = x;
    int y_ = y;
    auto validCoord = [&](){ return validCoordinate(x_, y_); };

    // test if the piece is at the position
    if(!validCoord() || board.at(x_, y_) != this) {
        return availableFields;
    }

    int direction = (getColor() == white ? 1 : -1);
    y_ = y + direction;

    if(!validCoord() || board.at(x_, y_) != nullptr) {
        return availableFields;
    }

    availableFields.emplace_back(x_, y_);

    y_ += direction;
    if(validCoord() && !this->moved && board.at(x_, y_) == nullptr) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x+1;
    y_ = y + direction;
    if(validCoord() && board.at(x_, y_) != nullptr && board.at(x_, y_)->getColor() != this->getColor()) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x-1;
    if(validCoord() && board.at(x_, y_) != nullptr && board.at(x_, y_)->getColor() != this->getColor()) {
        availableFields.emplace_back(x_, y_);
    }

    // TODO add en passant rules

    return availableFields;
}

const char Pawn::getChar() const {
    return 'p';
}
