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

void Piece::countMove(int turn) {
    this->lastTurnMoved = turn;
    ++this->moveCount;
}

bool Piece::move(int srcX, int srcY, int dstX, int dstY, Board &board, int turn) {
    Piece *(&srcPtr) = board.get(srcX, srcY);
    if(this != srcPtr) {
        return false;
    }

    Piece *(&dstPtr) = board.get(dstX, dstY);

    dstPtr = this;
    srcPtr = nullptr;
    this->countMove(turn);
    return true;
}


Pawn::Pawn(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Pawn::getAllFields(int x, int y, Board const &board, int turn) const {
    std::vector<std::pair<int, int>> availableFields;
    int x_ = x;
    int y_ = y;
    auto validCoord = [&](){ return validCoordinate(x_, y_); };
    auto attackCheck = [&](){ return validCoordinate(x_, y_) && board.at(x_, y_) != nullptr && board.at(x_, y_)->getColor() != this->getColor(); };

    int direction = (getColor() == white ? 1 : -1);
    y_ = y + direction;

    if(validCoord() && board.at(x_, y_) == nullptr) {
        availableFields.emplace_back(x_, y_);

        y_ += direction;
        if(validCoord() && this->getMoveCount() == 0 && board.at(x_, y_) == nullptr) {
            availableFields.emplace_back(x_, y_);
        }
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
    } else if(srcX != dstX) { // test for en passant
        Piece *(&enPassantPtr) = board.get(dstX, srcY);
        enPassantPtr = nullptr;
    }

    dstPtr = this;
    srcPtr = nullptr;
    this->countMove(turn);
    return true;
}

Knight::Knight(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Knight::getAllFields(int x, int y, Board const &board, int turn) const {
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
    return 'n';
}

Rogue::Rogue(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> Rogue::getAllFields(int x, int y, const Board& board, int turn) const {
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

std::vector<std::pair<int, int>> Bishop::getAllFields(int x, int y, const Board& board, int turn) const {
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

std::vector<std::pair<int, int>> Queen::getAllFields(int x, int y, const Board& board, int turn) const {
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
    return 'q';
}

King::King(Color color) : Piece(color) {}

std::vector<std::pair<int, int>> King::getAllFields(int x, int y, const Board& board, int turn) const {
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

    // test for castling
    if(this->getMoveCount() == 0 && !board.isLocationEndangered(x, y, this->getColor())) {
        // first left castling
        bool castling = true;
        for(int x_ = 1; x_ < x; ++x_) {
            if(board.at(x_, y) != nullptr || board.isLocationEndangered(x_, y, this->getColor())) {
                castling = false;
                break;
            }
        }

        Piece const *p = board.at(0, y);
        if(castling && p != nullptr && p->getMoveCount() == 0) {
            availableFields.emplace_back(2, y);
        }

        // then right castling
        castling = true;
        for(int x_ = x+1; x_ < BOARD_WIDTH-1; ++x_) {
            if(board.at(x_, y) != nullptr || board.isLocationEndangered(x_, y, this->getColor())) {
                castling = false;
                break;
            }
        }

        p = board.at(7, y);
        if(castling && p != nullptr && p->getMoveCount() == 0) {
            availableFields.emplace_back(6, y);
        }
    }

    return availableFields;
}

const char King::getChar() const {
    return 'k';
}

bool King::move(int srcX, int srcY, int dstX, int dstY, Board &board, int turn) {
    Piece *(&srcPtr) = board.get(srcX, srcY);
    if(this != srcPtr) {
        return false;
    }

    Piece *(&dstPtr) = board.get(dstX, dstY);
    if(dstPtr != nullptr) {
        dstPtr->countMove(turn);
    }

    // test for castling
    else if(static_cast<unsigned>(dstX - srcX) > 1) {
        if(dstX == 2) {
            Piece *(&rookPtr) = board.get(0, srcY);
            board.set(3, srcY, rookPtr);
            rookPtr->countMove(turn);
            rookPtr = nullptr;
        } else {
            Piece *(&rookPtr) = board.get(7, srcY);
            board.set(5, srcY, rookPtr);
            rookPtr->countMove(turn);
            rookPtr = nullptr;
        }
    }

    dstPtr = this;
    srcPtr = nullptr;
    this->countMove(turn);
    return true;
}

std::vector<std::pair<int, int>> Piece::getAllAvailableFields(int x, int y, Board const &board, int turn) const {
    if(board.at(x, y) != this) {
        throw std::invalid_argument("This piece could not be found at the location.");
    }

    auto availableFields = this->getAllFields(x, y, board, turn);

    auto availableFieldsCpy = std::move(availableFields);
    for(auto const &field : availableFieldsCpy) {
        Board board_cpy = board;
        board_cpy.get(x, y)->move(x, y, field.first, field.second, board_cpy, turn);

        if(!board_cpy.isCheck(this->getColor())) {
            availableFields.push_back(field);
        }
    }

    return availableFields;
}

const char Piece::getSymbol() const {
    char c = this->getChar();
    if(this->getColor() == white) {
        c = std::toupper(c);
    }

    return c;
}
