#include "Chess.h"

#include <algorithm>

using namespace SimpleChess;

Board const & Chess::getBoard() const {
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
    Piece *(&piece) = this->board.get(srcX, srcY);// this->board[srcX + BOARD_WIDTH*srcY];
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

    Piece *(&dstPtr) = this->board.get(dstX, dstY);
    if(dstPtr != nullptr) {
        this->board.fallenPieces.push_back(dstPtr);
    }

    dstPtr = piece;
    piece = nullptr;
    dstPtr->setMoved();
    ++this->turn;
    return true;
}
