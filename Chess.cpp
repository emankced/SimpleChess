#include "Chess.h"

#include <algorithm>

using namespace SimpleChess;

Board const & Chess::getBoard() const {
    return this->board;
}

State Chess::getState() {
    // TODO
    bool isCheck = this->board.isCheck(this->whoseTurnIsIt());
    bool isMate = this->board.isCheck(this->whoseTurnIsIt());

    if(isCheck && isMate) {
        return checkMate;
    } else if(isCheck) {
        return check;
    } else if(isMate) {
        return stallMate;
    }

    // TODO wish logic

    return normal;
}

Color Chess::whoseTurnIsIt() const {
    return static_cast<Color>(this->turn % 2);
}

int Chess::getTurn() {
    return this->turn;
}

bool Chess::move(int srcX, int srcY, int dstX, int dstY) {
    Piece *(&piece) = this->board.get(srcX, srcY);
    if(piece == nullptr) {
        return false;
    }

    // destination cannot be the king
    Piece const *dst = this->board.at(dstX, dstY);
    if(dst != nullptr && dynamic_cast<King const*>(dst) != nullptr) {
        return false;
    }

    // TODO check that the destination is not the king

    if(piece->getColor() != whoseTurnIsIt()) {
        return false;
    }

    auto allAvailableFields = piece->getAllAvailableFields(srcX, srcY, this->board, this->turn);

    // check if the destination field is unreachable
    if(std::find(allAvailableFields.begin(), allAvailableFields.end(), std::pair<int, int>(dstX, dstY)) == allAvailableFields.end()) {
        return false;
    }

    if(!piece->move(srcX, srcY, dstX, dstY, this->board, this->turn)) {
        return false;
    }

    ++this->turn;
    return true;
}
