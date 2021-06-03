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
    auto checkAndEmplace = [&](){
        if(validCoordinate(x_, y_) && (board.at(x_, y_) == nullptr || board.at(x_, y_)->getColor() != this->getColor())) {
            availableFields.emplace_back(x_, y_);

        }
    };

    checkAndEmplace();

    y_ = y - 1;
    checkAndEmplace();

    x_ = x - 2;
    checkAndEmplace();

    y_ = y + 1;
    checkAndEmplace();

    x_ = x + 1;
    y_ = y + 2;
    checkAndEmplace();

    x_ = x - 1;
    checkAndEmplace();

    y_ = y - 2;
    checkAndEmplace();

    x_ = x + 1;
    checkAndEmplace();

    return availableFields;
}

const char Knight::getChar() const {
    return 'k';
}

Rogue::Rogue(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Rogue::getAllAvailableFields(int x, int y, const SimpleChess::Board& board, int turn) {
    if(board.at(x, y) != this) {
        throw std::invalid_argument("This piece could not be found at the location.");
    }

    std::vector<std::pair<int, int>> availableFields;

    auto loop = [&](int x_, int y_) {
        int x_inc = x_ - x;
        int y_inc = y_ - y;
        for(; (x_inc != 0 ? (x_inc > 0 ? x_ < BOARD_WIDTH : x_ >= 0) : (y_inc > 0 ? y_ < BOARD_HEIGHT : y_ >= 0)); (x_inc != 0 ? x_+=x_inc : y_+=y_inc)) {
            Piece const *p = board.at(x_, y_);
            if(p != nullptr) {
                if(p->getColor() != this->getColor()) {
                    availableFields.emplace_back(x_, y_);
                }
                break;
            }
            availableFields.emplace_back(x_, y_);
        }
    };

    loop(x+1, y);
    loop(x-1, y);
    loop(x, y+1);
    loop(x, y-1);

    return availableFields;
}

const char Rogue::getChar() const {
    return 'r';
}

Bishop::Bishop(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Bishop::getAllAvailableFields(int x, int y, const SimpleChess::Board& board, int turn) {
    if(board.at(x, y) != this) {
        throw std::invalid_argument("This piece could not be found at the location.");
    }

    std::vector<std::pair<int, int>> availableFields;

    auto loop = [&](int x_, int y_) {
        int x_inc = x_ - x;
        int y_inc = y_ - y;
        for(; (x_inc > 0 ? x_ < BOARD_WIDTH : x_ >= 0) && (y_inc > 0 ? y_ < BOARD_HEIGHT : y_ >= 0); x_+=x_inc, y_+=y_inc) {
            Piece const *p = board.at(x_, y_);
            if(p != nullptr) {
                if(p->getColor() != this->getColor()) {
                    availableFields.emplace_back(x_, y_);
                }
                break;
            }
            availableFields.emplace_back(x_, y_);
        }
    };

    loop(x+1, y+1);
    loop(x+1, y-1);
    loop(x-1, y+1);
    loop(x-1, y-1);

    return availableFields;
}

const char Bishop::getChar() const {
    return 'b';
}

Queen::Queen(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Queen::getAllAvailableFields(int x, int y, const SimpleChess::Board& board, int turn) {
    if(board.at(x, y) != this) {
        throw std::invalid_argument("This piece could not be found at the location.");
    }

    std::vector<std::pair<int, int>> availableFields;

    auto loop = [&](int x_, int y_) {
        int x_inc = x_ - x;
        int y_inc = y_ - y;
        for(; (x_inc == 0 ? true : (x_inc > 0 ? x_ < BOARD_WIDTH : x_ >= 0)) && (y_inc == 0 ? true : (y_inc > 0 ? y_ < BOARD_HEIGHT : y_ >= 0)); (x_inc != 0 ? x_+=x_inc : 0), (y_inc != 0 ? y_+=y_inc : 0)) {
            Piece const *p = board.at(x_, y_);
            if(p != nullptr) {
                if(p->getColor() != this->getColor()) {
                    availableFields.emplace_back(x_, y_);
                }
                break;
            }
            availableFields.emplace_back(x_, y_);
        }
    };

    loop(x+1, y);
    loop(x-1, y);
    loop(x, y+1);
    loop(x, y-1);

    loop(x+1, y+1);
    loop(x+1, y-1);
    loop(x-1, y+1);
    loop(x-1, y-1);

    return availableFields;
}

const char Queen::getChar() const {
    return 'Q';
}

King::King(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> King::getAllAvailableFields(int x, int y, const SimpleChess::Board& board, int turn) {
    if(board.at(x, y) != this) {
        throw std::invalid_argument("This piece could not be found at the location.");
    }

    std::vector<std::pair<int, int>> availableFields;

    for(int x_ = std::max(x-1, 0); x_ < std::min(x+2, BOARD_WIDTH); ++x_) {
        for(int y_ = std::max(y-1, 0); y_ < std::min(y+2, BOARD_HEIGHT); ++y_) {
            if(x_ == x && y_ == y) {
                continue;
            }

            Piece const *p = board.at(x_, y_);
            if(p == nullptr || p->getColor() != this->getColor()) {
                availableFields.emplace_back(x_, y_);
            }
        }
    }

    return availableFields;
}

const char King::getChar() const {
    return 'K';
}

bool King::move(int srcX, int srcY, int dstX, int dstY, Board &board, int turn) {
    // TODO
    return false;
}
