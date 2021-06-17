#include "Board.h"

#include <algorithm>
#include <string>
#include <stdexcept>

using namespace SimpleChess;

Board::Board() {
    this->pieces.push_back(std::make_unique<Rogue>(white));
    this->set(0, 0, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Knight>(white));
    this->set(1, 0, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Bishop>(white));
    this->set(2, 0, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Queen>(white));
    this->set(3, 0, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<King>(white));
    this->set(4, 0, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Bishop>(white));
    this->set(5, 0, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Knight>(white));
    this->set(6, 0, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Rogue>(white));
    this->set(7, 0, this->pieces.back().get());

    for(int i = 0; i < BOARD_WIDTH; ++i) {
        this->pieces.push_back(std::make_unique<Pawn>(white));
        board[BOARD_WIDTH + i] = this->pieces.back().get();
    }

    for(int i = 2*BOARD_WIDTH; i < BOARD_SIZE - 2*BOARD_WIDTH; ++i) {
        this->board[i] = nullptr;
    }

    for(int i = BOARD_SIZE - 2*BOARD_WIDTH; i < BOARD_SIZE - BOARD_WIDTH; ++i) {
        this->pieces.push_back(std::make_unique<Pawn>(black));
        board[i] = this->pieces.back().get();
    }

    this->pieces.push_back(std::make_unique<Rogue>(black));
    this->set(0, 7, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Knight>(black));
    this->set(1, 7, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Bishop>(black));
    this->set(2, 7, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Queen>(black));
    this->set(3, 7, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<King>(black));
    this->set(4, 7, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Bishop>(black));
    this->set(5, 7, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Knight>(black));
    this->set(6, 7, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Rogue>(black));
    this->set(7, 7, this->pieces.back().get());
}

Piece const * Board::at(int x, int y) const {
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    return this->board[x + y*BOARD_WIDTH];
}

Piece * & Board::get(int x, int y) {
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    return this->board[x + y*BOARD_WIDTH];
}

void Board::set(int x, int y, Piece* piece) {
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    this->board[x + y*BOARD_WIDTH] = piece;
}

bool Board::isLocationEndangered(int x, int y, Color ownColor) const { // this is meant to be used to test for check
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    auto straightLine = [&](int x_, int y_) {
        int x_inc = x_ - x;
        int y_inc = y_ - y;
        for(; (x_inc != 0 ? (x_inc > 0 ? x_ < BOARD_WIDTH : x_ >= 0) : (y_inc > 0 ? y_ < BOARD_HEIGHT : y_ >= 0)); (x_inc != 0 ? x_+=x_inc : y_+=y_inc)) {
            Piece const *p = this->at(x_, y_);
            if(p != nullptr) {
                if(p->getColor() != ownColor && (dynamic_cast<Rogue const*>(p) != nullptr || dynamic_cast<Queen const*>(p) != nullptr)) {
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
            Piece const *p = this->at(x_, y_);
            if(p != nullptr) {
                if(p->getColor() != ownColor && (dynamic_cast<Bishop const*>(p) != nullptr || dynamic_cast<Queen const*>(p) != nullptr)) {
                    return true;
                }
                break;
            }
        }

        return false;
    };

    auto pawn = [&]() {
        int direction = (ownColor == white ? -1 : 1);
        int x_ = x + 1;
        int y_ = y + direction;

        if(validCoordinate(x_, y_)) {
            Piece const *p = this->at(x_, y_);
            if(p != nullptr && dynamic_cast<Pawn const*>(p) != nullptr) {
                return true;
            }
        }

        x_ = x - 1;
        if(validCoordinate(x_, y_)) {
            Piece const *p = this->at(x_, y_);
            if(p != nullptr && dynamic_cast<Pawn const*>(p) != nullptr) {
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
                Piece const *p = this->at(x_, y_);
                if(p != nullptr && p->getColor() != ownColor && dynamic_cast<Knight const*>(p) != nullptr) {
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

                Piece const *p = this->at(x_, y_);
                if(p != nullptr && p->getColor() != ownColor && dynamic_cast<King const*>(p) != nullptr) {
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
            Piece const* p = this->at(x, y);
            if(p != nullptr && p->getColor() == color && dynamic_cast<King const*>(p) != nullptr) {
                return this->isLocationEndangered(x, y, color);
            }
        }
    }

    return true;
}

bool Board::isMate(Color color, int turn) const {
    for(int y = 0; y < BOARD_HEIGHT; ++y) {
        for(int x = 0; x < BOARD_WIDTH; ++x) {
            Piece const* p = this->at(x, y);
            if(p != nullptr && p->getColor() == color && p->getAllAvailableFields(x, y, *this, turn).size() > 0) {
                return false;
            }
        }
    }

    return true;
}

Board::Board(const Board &other) {
    this->fallenPieces = other.fallenPieces;

    for(auto const& p : other.pieces) {
        if(dynamic_cast<Pawn*>(p.get()) != nullptr) {
            this->pieces.push_back(std::make_unique<Pawn>(*static_cast<Pawn*>(p.get())));
        } else if(dynamic_cast<Rogue*>(p.get()) != nullptr) {
            this->pieces.push_back(std::make_unique<Rogue>(*static_cast<Rogue*>(p.get())));
        } else if(dynamic_cast<Knight*>(p.get()) != nullptr) {
            this->pieces.push_back(std::make_unique<Knight>(*static_cast<Knight*>(p.get())));
        } else if(dynamic_cast<Bishop*>(p.get()) != nullptr) {
            this->pieces.push_back(std::make_unique<Bishop>(*static_cast<Bishop*>(p.get())));
        } else if(dynamic_cast<Queen*>(p.get()) != nullptr) {
            this->pieces.push_back(std::make_unique<Queen>(*static_cast<Queen*>(p.get())));
        } else if(dynamic_cast<King*>(p.get()) != nullptr) {
            this->pieces.push_back(std::make_unique<King>(*static_cast<King*>(p.get())));
        }
    }

    for(int y = 0; y < BOARD_HEIGHT; ++y) {
        for(int x = 0; x < BOARD_WIDTH; ++x) {
            Piece const *p = other.at(x, y);
            if(p != nullptr) {
                for(int i = 0; i < other.pieces.size(); ++i) {
                    if(other.pieces.at(i).get() == p) {
                        this->set(x, y, this->pieces.at(i).get());
                    }
                }
            } else {
                this->set(x, y, nullptr);
            }
        }
    }
}

void Board::addToFallenPieces(SimpleChess::Piece* piece) {
    if(piece != nullptr)
        this->fallenPieces.push_back(piece);
}

std::vector<Piece*> Board::getFallenPieces(Color color) const {
    std::vector<Piece*> filteredFallenPieces;

    for(auto const &piece : this->fallenPieces) {
        if(piece->getColor() == color && dynamic_cast<Pawn*>(piece) == nullptr) {
            filteredFallenPieces.push_back(piece);
        }
    }

    return filteredFallenPieces;
}
