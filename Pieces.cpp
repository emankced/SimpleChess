#include "Pieces.h"

#include "Board.h"

using namespace SimpleChess;

Piece::Piece(Color color) : color(color) {}

const Color Piece::getColor() const {
    return this->color;
}

int Piece::getLastTurnMoved() const {
    return this->lastTurnMoved;
}

int Piece::getMoveCount() const {
    return this->moveCount;
}

bool Piece::move(int srcX, int srcY, int dstX, int dstY, Board &board, int turn) {
    Piece *(&srcPtr) = board.get(srcX, srcY);
    if(this != srcPtr) {
        return false;
    }

    Piece *(&dstPtr) = board.get(dstX, dstY);
    if(dstPtr != nullptr) {
        board.fallenPieces.push_back(dstPtr);
    }

    dstPtr = this;
    srcPtr = nullptr;
    this->lastTurnMoved = turn;
    ++this->moveCount;
    return true;
}


Pawn::Pawn(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Pawn::getAllAvailableFields(int x, int y, Board const &board, int turn) {
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
    if(validCoord() && this->moveCount == 0 && board.at(x_, y_) == nullptr) {
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

    // en passant rules
    if(y == BOARD_HEIGHT / 2 + (getColor() == white ? 0 : direction)) {
        x_ = x-1;
        y_ = y;
        if(validCoord() && dynamic_cast<Pawn const *>(board.at(x_, y_)) != nullptr) {
            Pawn const *p = static_cast<Pawn const *>(board.at(x_, y_));
            if(p->getMoveCount() == 1 && p->getLastTurnMoved() == turn-1) {
                availableFields.emplace_back(x_, y_ + direction);
            }
        }

        x_ = x+1;

        if(validCoord() && dynamic_cast<Pawn const *>(board.at(x_, y_)) != nullptr) {
            Pawn const *p = static_cast<Pawn const *>(board.at(x_, y_));
            if(p->getMoveCount() == 1 && p->getLastTurnMoved() == turn-1) {
                availableFields.emplace_back(x_, y_ + direction);
            }
        }
    }

    return availableFields;
}

const char Pawn::getChar() const {
    return 'p';
}

bool Pawn::move(int srcX, int srcY, int dstX, int dstY, Board &board, int turn) {
    Piece *(&srcPtr) = board.get(srcX, srcY);
    if(this != srcPtr) {
        return false;
    }

    Piece *(&dstPtr) = board.get(dstX, dstY);
    if(dstPtr != nullptr) {
        board.fallenPieces.push_back(dstPtr);
    } else if(srcX != dstX) { // test for en passant
        Piece *(&enPassantPtr) = board.get(dstX, srcY);
        board.fallenPieces.push_back(enPassantPtr);
        enPassantPtr = nullptr;
    }

    dstPtr = this;
    srcPtr = nullptr;
    this->lastTurnMoved = turn;
    ++this->moveCount;
    return true;
}
