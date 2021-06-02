#include <iostream>

#include "Chess.h"

using namespace std;
using namespace SimpleChess;

void printBoard(Chess &chess) {
    Board const &board = chess.getBoard();

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
            auto piece = board.at(col, row);
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
        chess.move(0, 6, 0, 4);

        chess.move(1, 3, 1, 5);
        chess.move(1, 3, 0, 4);

        chess.move(1, 6, 1, 4);
        chess.move(0, 4, 1, 5);

        chess.move(1, 7, 2, 5);
        chess.move(1, 5, 1, 6);

        chess.move(2, 5, 3, 7);
        chess.move(6, 0, 7, 2);

        chess.move(3, 7, 1, 6);
        chess.move(7, 0, 2, 0);

        chess.move(0, 7, 0, 1);

        printBoard(chess);

        // TODO
        break;
    }

    return 0;
}
