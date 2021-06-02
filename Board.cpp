#include "Board.h"

#include <string>
#include <stdexcept>

using namespace SimpleChess;

Board::Board() {
    for(int i = 0; i < BOARD_WIDTH; ++i) {
        this->board[i] = nullptr; // TODO
    }

    this->pieces.push_back(std::make_unique<Knight>(white));
    this->set(1, 0, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Knight>(white));
    this->set(6, 0, this->pieces.back().get());

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

    for(int i = BOARD_SIZE - BOARD_WIDTH; i < BOARD_SIZE; ++i) {
        this->board[i] = nullptr; // TODO
    }

    this->pieces.push_back(std::make_unique<Knight>(black));
    this->set(1, 7, this->pieces.back().get());

    this->pieces.push_back(std::make_unique<Knight>(black));
    this->set(6, 7, this->pieces.back().get());
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

