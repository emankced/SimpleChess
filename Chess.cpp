#include "Chess.h"

#include <algorithm>

using namespace SimpleChess;

Board const & Chess::getBoard() const {
    return this->board;
}

State Chess::getState() const {
    bool isCheck = this->board.isCheck(this->whoseTurnIsIt());
    bool isMate = this->board.isCheck(this->whoseTurnIsIt());

    if(isCheck && isMate) {
        return checkMate;
    } else if(isCheck) {
        return check;
    } else if(isMate) {
        return stallMate;
    }

    // wish logic
    Color color = this->whoseTurnIsIt();
    if(this->board.getFallenPieces(color).size() > 0) {
        int row = (color == white ? BOARD_HEIGHT-1 : 0);
        for(int col = 0; col < BOARD_WIDTH; ++col) {
            Piece const *piece = this->board.at(col, row);
            if(piece != nullptr && piece->getColor() == color && dynamic_cast<Pawn const*>(piece) != nullptr) {
                return wish;
            }
        }
    }

    return normal;
}

Color Chess::whoseTurnIsIt() const {
    return static_cast<Color>(this->turn % 2);
}

int Chess::getTurn() const {
    return this->turn;
}

bool Chess::move(int srcX, int srcY, int dstX, int dstY) {
    auto state = this->getState();
    if(state != normal && state != check) {
        return false;
    }

    Piece *(&piece) = this->board.get(srcX, srcY);
    if(piece == nullptr) {
        return false;
    }

    // destination cannot be the king
    Piece const *dst = this->board.at(dstX, dstY);
    if(dst != nullptr && dynamic_cast<King const*>(dst) != nullptr) {
        return false;
    }

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

    if(this->getState() != wish) {
        ++this->turn;
    }
    return true;
}

bool Chess::wishPiece(Piece* piece) {
    if(this->getState() != wish || piece == nullptr || piece->getColor() != this->whoseTurnIsIt()) {
        return false;
    }

    Color color = this->whoseTurnIsIt();
    auto pieces = this->board.getFallenPieces(color);
    if(std::find(pieces.begin(), pieces.end(), piece) == pieces.end()) {
        return false;
    }

    int row = (color == white ? BOARD_HEIGHT-1 : 0);
    for(int col = 0; col < BOARD_WIDTH; ++col) {
        Piece const *p = this->board.at(col, row);
        if(p == piece && p->getColor() == color && dynamic_cast<Pawn const*>(p) != nullptr) {
            this->board.set(col, row, piece);
            ++this->turn;
            return true;
        }
    }

    return false;
}
