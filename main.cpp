#include <iostream>

#include "Chess.h"

using namespace std;
using namespace SimpleChess;

void printBoard(Chess &chess) {
    auto board = chess.getBoard();

    cout << "  ";
    for(int col = 0; col < BOARD_WIDTH; ++col) {
        cout << static_cast<char>('a'+col);
    }
    cout << endl;

    cout << "  ";
    for(int col = 0; col < BOARD_WIDTH; ++col) {
        cout << "-";
    }
    cout << endl;

    for(int row = BOARD_HEIGHT-1; row >= 0; --row) {
        cout << static_cast<char>('1'+row) << "|";
        for(int col = 0; col < BOARD_WIDTH; ++col) {
            auto piece = board[col + row*BOARD_WIDTH];
            if(piece == nullptr) {
                cout << " ";
            } else {
                cout << piece->getChar();
            }
        }
        cout << "|" << static_cast<char>('1'+row) << endl;
    }

    cout << "  ";
    for(int col = 0; col < BOARD_WIDTH; ++col) {
        cout << "-";
    }
    cout << endl;

    cout << "  ";
    for(int col = 0; col < BOARD_WIDTH; ++col) {
        cout << static_cast<char>('a'+col);
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    cout << "SimpleChess" << endl << "===========" << endl << endl;

    Chess chess;

    // game loop
    while(chess.getState() != SimpleChess::chessMate && chess.getState() != SimpleChess::stallMate) {
        cout << "It's " << (chess.whoseTurnIsIt() == white ? "white" : "black") << "'s turn." << endl;

        printBoard(chess);
        chess.move(1, 1, 1, 3);
        printBoard(chess);

        // TODO
        break;
    }

    return 0;
}
