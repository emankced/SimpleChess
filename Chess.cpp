#include "Chess.h"

#include <algorithm>

using namespace SimpleChess;

Chess::Chess() {
    for(int i = 0; i < BOARD_WIDTH; ++i) {
        this->board[i] = nullptr; // TODO
    }

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
}

Piece const * const (& Chess::getBoard())[BOARD_SIZE] {
    return this->board;
}

State Chess::getState() {
    // TODO
    return normal;
}

Color Chess::whoseTurnIsIt() {
    return static_cast<Color>(this->turn % 2);
}

int Chess::getTurn() {
    return this->turn;
}

bool Chess::move(int srcX, int srcY, int dstX, int dstY) {
    Piece *(&piece) = this->board[srcX + BOARD_WIDTH*srcY];
    if(piece == nullptr) {
        return false;
    }

    // TODO check that the destination is not the king

    if(piece->getColor() != whoseTurnIsIt()) {
        return false;
    }

    auto allAvailableFields = piece->getAllAvailableFields(srcX, srcY, this->board);

    // check if the destination field is unreachable
    if(std::find(allAvailableFields.begin(), allAvailableFields.end(), std::pair<int, int>(dstX, dstY)) == allAvailableFields.end()) {
        return false;
    }

    Piece *(&dstPtr) = this->board[dstX + BOARD_WIDTH*dstY];
    if(dstPtr != nullptr) {
        this->fallenPieces.push_back(dstPtr);
    }

    dstPtr = piece;
    piece = nullptr;
    return true;
}
