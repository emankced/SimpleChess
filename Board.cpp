#include "Board.h"

#include <algorithm>
#include <string>
#include <stdexcept>

using namespace SimpleChess;

Board::Board() {
    std::unique_ptr<Piece> p = std::make_unique<Rogue>(white);
    this->set(0, 0, p);

    p = std::make_unique<Knight>(white);
    this->set(1, 0, p);

    p = std::make_unique<Bishop>(white);
    this->set(2, 0, p);

    p = std::make_unique<Queen>(white);
    this->set(3, 0, p);

    p = std::make_unique<King>(white);
    this->set(4, 0, p);

    p = std::make_unique<Bishop>(white);
    this->set(5, 0, p);

    p = std::make_unique<Knight>(white);
    this->set(6, 0, p);

    p = std::make_unique<Rogue>(white);
    this->set(7, 0, p);

    for(int i = 0; i < BOARD_WIDTH; ++i) {
        p = std::make_unique<Pawn>(white);
        this->set(i, 1, p);
    }

    /*
    for(int i = 2*BOARD_WIDTH; i < BOARD_SIZE - 2*BOARD_WIDTH; ++i) {
        this->board[i] = nullptr;
    }
    */

    for(int i = 0 ; i < BOARD_WIDTH; ++i) {
        p = std::make_unique<Pawn>(black);
        this->set(i, 6, p);
    }

    p = std::make_unique<Rogue>(black);
    this->set(0, 7, p);

    p = std::make_unique<Knight>(black);
    this->set(1, 7, p);

    p = std::make_unique<Bishop>(black);
    this->set(2, 7, p);

    p = std::make_unique<Queen>(black);
    this->set(3, 7, p);

    p = std::make_unique<King>(black);
    this->set(4, 7, p);

    p = std::make_unique<Bishop>(black);
    this->set(5, 7, p);

    p = std::make_unique<Knight>(black);
    this->set(6, 7, p);

    p = std::make_unique<Rogue>(black);
    this->set(7, 7, p);
}

const std::unique_ptr<Piece> & Board::at(int x, int y) const {
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    return this->board[x + y*BOARD_WIDTH];
}

std::unique_ptr<Piece> & Board::get(int x, int y) {
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    return this->board[x + y*BOARD_WIDTH];
}

void Board::set(int x, int y, std::unique_ptr<Piece> & piece) {
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    this->board[x + y*BOARD_WIDTH] = std::move(piece);
}

bool Board::isLocationEndangered(int x, int y, Color ownColor) const { // this is meant to be used to test for check
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    auto straightLine = [&](int x_, int y_) {
        int x_inc = x_ - x;
        int y_inc = y_ - y;
        for(; (x_inc != 0 ? (x_inc > 0 ? x_ < BOARD_WIDTH : x_ >= 0) : (y_inc > 0 ? y_ < BOARD_HEIGHT : y_ >= 0)); (x_inc != 0 ? x_+=x_inc : y_+=y_inc)) {
            auto & p = this->at(x_, y_);
            if(p) {
                if(p->getColor() != ownColor && (dynamic_cast<Rogue const*>(p.get()) != nullptr || dynamic_cast<Queen const*>(p.get()) != nullptr)) {
                    return true;
                }
                break;
            }
        }

        return false;
    };

    auto diagonalLine = [&](int x_, int y_) {
        int x_inc = x_ - x;
        int y_inc = y_ - y;
        for(; (x_inc > 0 ? x_ < BOARD_WIDTH : x_ >= 0) && (y_inc > 0 ? y_ < BOARD_HEIGHT : y_ >= 0); x_+=x_inc, y_+=y_inc) {
            auto & p = this->at(x_, y_);
            if(p) {
                if(p->getColor() != ownColor && (dynamic_cast<Bishop const*>(p.get()) != nullptr || dynamic_cast<Queen const*>(p.get()) != nullptr)) {
                    return true;
                }
                break;
            }
        }

        return false;
    };

    auto pawn = [&]() {
        int direction = (ownColor == white ? 1 : -1);
        int x_ = x + 1;
        int y_ = y + direction;

        if(validCoordinate(x_, y_)) {
            auto & p = this->at(x_, y_);
            if(p && p->getColor() != ownColor && dynamic_cast<Pawn const*>(p.get()) != nullptr) {
                return true;
            }
        }

        x_ = x - 1;
        if(validCoordinate(x_, y_)) {
            auto & p = this->at(x_, y_);
            if(p && p->getColor() != ownColor && dynamic_cast<Pawn const*>(p.get()) != nullptr) {
                return true;
            }
        }

        return false;
    };

    auto knight = [&]() {
        int x_ = x + 2;
        int y_ = y + 1;
        auto checkKnight = [&](){
            if(validCoordinate(x_, y_)) {
                auto & p = this->at(x_, y_);
                if(p && p->getColor() != ownColor && dynamic_cast<Knight const*>(p.get()) != nullptr) {
                    return true;
                }
            }
            return false;
        };

        if(checkKnight())
            return true;

        y_ = y - 1;
        if(checkKnight())
            return true;

        x_ = x - 2;
        if(checkKnight())
            return true;

        y_ = y + 1;
        if(checkKnight())
            return true;

        x_ = x + 1;
        y_ = y + 2;
        if(checkKnight())
            return true;

        x_ = x - 1;
        if(checkKnight())
            return true;

        y_ = y - 2;
        if(checkKnight())
            return true;

        x_ = x + 1;
        if(checkKnight())
            return true;

        return false;
    };

    auto king = [&]() {
        for(int y_ = std::max(y-1, 0); y_ < std::min(y+1, BOARD_HEIGHT); ++y_) {
            for(int x_ = std::max(x-1, 0); x_ < std::min(x+1, BOARD_WIDTH); ++x_) {
                if(x_ == x && y_ == y) {
                    continue;
                }

                auto & p = this->at(x_, y_);
                if(p && p->getColor() != ownColor && dynamic_cast<King const*>(p.get()) != nullptr) {
                    return true;
                }
            }
        }

        return false;
    };

    if(straightLine(x+1, y) || straightLine(x-1, y) || straightLine(x, y+1) || straightLine(x, y-1)
        || diagonalLine(x+1, y+1) || diagonalLine(x+1, y-1) || diagonalLine(x-1, y+1) || diagonalLine(x-1, y-1)
        || pawn() || knight() || king()) {
        return true;
    }

    return false;
}

bool Board::isCheck(Color color) const  {
    for(int y = 0; y < BOARD_HEIGHT; ++y) {
        for(int x = 0; x < BOARD_WIDTH; ++x) {
            auto & p = this->at(x, y);
            if(p && p->getColor() == color && dynamic_cast<King const*>(p.get()) != nullptr) {
                return this->isLocationEndangered(x, y, color);
            }
        }
    }

    return true;
}

bool Board::isMate(Color color, int turn) const {
    for(int y = 0; y < BOARD_HEIGHT; ++y) {
        for(int x = 0; x < BOARD_WIDTH; ++x) {
            auto & p = this->at(x, y);
            if(p && p->getColor() == color && p->getAllAvailableFields(x, y, *this, turn).size() > 0) {
                return false;
            }
        }
    }

    return true;
}

Board::Board(const Board &other) {
    for(int y = 0; y < BOARD_HEIGHT; ++y) {
        for(int x = 0; x < BOARD_WIDTH; ++x) {
            auto & p = other.at(x, y);
            if(p) {
                std::unique_ptr<Piece> own_p;
                if(dynamic_cast<Pawn*>(p.get()) != nullptr) {
                    own_p = std::make_unique<Pawn>(*static_cast<Pawn*>(p.get()));
                } else if(dynamic_cast<Rogue*>(p.get()) != nullptr) {
                    own_p = std::make_unique<Rogue>(*static_cast<Rogue*>(p.get()));
                } else if(dynamic_cast<Knight*>(p.get()) != nullptr) {
                    own_p = std::make_unique<Knight>(*static_cast<Knight*>(p.get()));
                } else if(dynamic_cast<Bishop*>(p.get()) != nullptr) {
                    own_p = std::make_unique<Bishop>(*static_cast<Bishop*>(p.get()));
                } else if(dynamic_cast<Queen*>(p.get()) != nullptr) {
                    own_p = std::make_unique<Queen>(*static_cast<Queen*>(p.get()));
                } else if(dynamic_cast<King*>(p.get()) != nullptr) {
                    own_p = std::make_unique<King>(*static_cast<King*>(p.get()));
                }

                this->set(x, y, own_p);
            }
        }
    }
}

void Board::promote(int x, int y, char c) {
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }
    if(dynamic_cast<Pawn const*>(this->at(x, y).get()) == nullptr) {
        throw std::logic_error("Only pawns can be promoted!");
    }

    Color color = this->at(x, y)->getColor();

    std::unique_ptr<Piece> p;

    switch(c) {
        case 'q':
            p = std::make_unique<Queen>(color);
            break;
        case 'n':
            p = std::make_unique<Knight>(color);
            break;
        case 'r':
            p = std::make_unique<Rogue>(color);
            break;
        case 'b':
            p = std::make_unique<Bishop>(color);
            break;
        default:
            throw std::logic_error("A pawn can only be promoted to queens, knights, rogues or bishops!");
    }

    this->set(x, y, p);
}
