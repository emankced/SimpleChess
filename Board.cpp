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

bool Board::isLocationEndangered(int x, int y, Color ownColor, int turn) const { // this is meant to be used to test for check
    if(!validCoordinate(x, y)) {
        throw std::range_error("Coordinates " + std::to_string(x) + " " + std::to_string(y) + " are not on the board!");
    }

    for(int y_ = 0; y_ < BOARD_HEIGHT; ++y_) {
        for(int x_ = 0; x_ < BOARD_WIDTH; ++x_) {
            Piece const *p = this->at(x_, y_);
            if(p != nullptr && p->getColor() != ownColor && dynamic_cast<const King*>(p) == nullptr) {
                auto availableFields = p->getAllAvailableFields(x_, y_, *this, turn);
                if(std::find(availableFields.begin(), availableFields.end(), std::make_pair(x, y)) != availableFields.end()) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::isCheck(Color color, int turn) const  {
    for(int y = 0; y < BOARD_HEIGHT; ++y) {
        for(int x = 0; x < BOARD_WIDTH; ++x) {
            Piece const* p = this->at(x, y);
            if(p != nullptr && p->getColor() == color && dynamic_cast<King const*>(p) != nullptr) {
                return this->isLocationEndangered(x, y, color, turn);
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
