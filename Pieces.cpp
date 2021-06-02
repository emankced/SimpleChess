#include "Pieces.h"

#include <stdexcept>

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
    // test if the piece is at the position
    if(board.at(x, y) != this) {
        throw std::invalid_argument("This piece could not be found at the location.");
    }

    std::vector<std::pair<int, int>> availableFields;
    int x_ = x;
    int y_ = y;
    auto validCoord = [&](){ return validCoordinate(x_, y_); };
    auto attackCheck = [&](){ return validCoordinate(x_, y_) && board.at(x_, y_) != nullptr && board.at(x_, y_)->getColor() != this->getColor(); };

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
    if(attackCheck()) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x-1;
    if(attackCheck()) {
        availableFields.emplace_back(x_, y_);
    }

    // en passant rules
    if(y == BOARD_HEIGHT / 2 + (getColor() == white ? 0 : direction)) {
        x_ = x-1;
        y_ = y;
        Pawn const *p = nullptr;
        auto turnAndMoveCountCheck = [&]() { return p != nullptr && p->getMoveCount() == 1 && p->getLastTurnMoved() == turn-1; };

        if(validCoord()) {
            p = dynamic_cast<Pawn const *>(board.at(x_, y_));

            if(turnAndMoveCountCheck()) {
                availableFields.emplace_back(x_, y_ + direction);
            }
        }

        x_ = x+1;
        if(validCoord()) {
            p = dynamic_cast<Pawn const *>(board.at(x_, y_));

            if(turnAndMoveCountCheck()) {
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

Knight::Knight(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Knight::getAllAvailableFields(int x, int y, Board const &board, int turn) {
    if(board.at(x, y) != this) {
        throw std::invalid_argument("This piece could not be found at the location.");
    }

    std::vector<std::pair<int, int>> availableFields;

    int x_ = x + 2;
    int y_ = y + 1;
    auto check = [&](){ return validCoordinate(x_, y_) && (board.at(x_, y_) == nullptr || board.at(x_, y_)->getColor() != this->getColor()); };

    if(check()) {
        availableFields.emplace_back(x_, y_);
    }

    y_ = y - 1;
    if(check()) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x - 2;
    if(check()) {
        availableFields.emplace_back(x_, y_);
    }

    y_ = y + 1;
    if(check()) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x + 1;
    y_ = y + 2;
    if(check()) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x - 1;
    if(check()) {
        availableFields.emplace_back(x_, y_);
    }

    y_ = y - 2;
    if(check()) {
        availableFields.emplace_back(x_, y_);
    }

    x_ = x + 1;
    if(check()) {
        availableFields.emplace_back(x_, y_);
    }

    return availableFields;
}

const char Knight::getChar() const {
    return 'k';
}

Rogue::Rogue(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> SimpleChess::Rogue::getAllAvailableFields(int x, int y, const SimpleChess::Board& board, int turn) {
    if(board.at(x, y) != this) {
        throw std::invalid_argument("This piece could not be found at the location.");
    }

    std::vector<std::pair<int, int>> availableFields;

    for(int x_ = x+1; x_ < BOARD_WIDTH; ++x_) {
        Piece const *p = board.at(x_, y);
        if(p != nullptr) {
            if(p->getColor() != this->getColor()) {
                availableFields.emplace_back(x_, y);
            }
            break;
        }
        availableFields.emplace_back(x_, y);
    }

    for(int x_ = x-1; x_ >= 0; --x_) {
        Piece const *p = board.at(x_, y);
        if(p != nullptr) {
            if(p->getColor() != this->getColor()) {
                availableFields.emplace_back(x_, y);
            }
            break;
        }
        availableFields.emplace_back(x_, y);
    }

    for(int y_ = y+1; y_ < BOARD_HEIGHT; ++y_) {
        Piece const *p = board.at(x, y_);
        if(p != nullptr) {
            if(p->getColor() != this->getColor()) {
                availableFields.emplace_back(x, y_);
            }
            break;
        }
        availableFields.emplace_back(x, y_);
    }

    for(int y_ = y-1; y_ >= 0; --y_) {
        Piece const *p = board.at(x, y_);
        if(p != nullptr) {
            if(p->getColor() != this->getColor()) {
                availableFields.emplace_back(x, y_);
            }
            break;
        }
        availableFields.emplace_back(x, y_);
    }

    return availableFields;
}

const char Rogue::getChar() const {
    return 'r';
}

