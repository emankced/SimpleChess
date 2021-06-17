#include <iostream>
#include <limits>

#include "Chess.h"

using namespace std;
using namespace SimpleChess;

void printBoard(Chess &chess) {
    Board const &board = chess.getBoard();

    cout << "  ";
    for(int col = 0; col < BOARD_WIDTH; ++col) {
        cout << static_cast<char>('a'+col);
        if(col != BOARD_WIDTH-1) {
            cout << " ";
        }
    }
    cout << endl;

    cout << "  ";
    for(int col = 0; col < BOARD_WIDTH * 2 - 1; ++col) {
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
                cout << piece->getSymbol();
            }

            if(col != BOARD_WIDTH-1) {
                cout << " ";
            }
        }
        cout << "|" << static_cast<char>('1'+row) << endl;
    }

    cout << "  ";
    for(int col = 0; col < BOARD_WIDTH * 2 - 1; ++col) {
        cout << "-";
    }
    cout << endl;

    cout << "  ";
    for(int col = 0; col < BOARD_WIDTH; ++col) {
        cout << static_cast<char>('a'+col);
        if(col != BOARD_WIDTH-1) {
            cout << " ";
        }
    }
    cout << endl;
}

pair<int, int> stringToCoordinates(string const &str) {
    int x = -1;
    int y = -1;

    if(str.size() == 2) {
        char c = std::tolower(str.at(0));
        char n = str.at(1);

        x = (int) c - (int) 'a';
        y = (int) n - (int) '1';
    }

    return make_pair(x, y);
}

int main(int argc, char *argv[]) {
    cout << "SimpleChess" << endl << "===========" << endl << endl;

    Chess chess;

    // game loop
    while(chess.getState() != SimpleChess::checkMate && chess.getState() != SimpleChess::stallMate) {
        cout << "It's " << (chess.whoseTurnIsIt() == white ? "white" : "black") << "'s turn." << endl;

        if(chess.getState() == SimpleChess::wish) {
            auto fallenPiece = chess.getBoard().getFallenPieces(chess.whoseTurnIsIt());
            cout << "You have a free wish. You can choose between:";
            for(auto const &piece : fallenPiece) {
                cout << " " << piece->getSymbol();
            }
            cout << endl << "Please choose one: ";

            string input;
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            size_t n = stoi(input);
            if(n >= fallenPiece.size() || !chess.wishPiece(fallenPiece.at(n))) {
                cout << "This wish is not possible!" << endl;
            }
        } else {
            printBoard(chess);
            cout << "Please move: ";
            string input0, input1;
            cin >> input0 >> input1;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            auto coordSrc = stringToCoordinates(input0);
            auto coordDst = stringToCoordinates(input1);

            auto coordTest = [](pair<int, int> const &c) {
                return min(c.first, c.second) >= 0 && c.first < BOARD_WIDTH && c.second < BOARD_HEIGHT;
            };

            if(!coordTest(coordSrc) || !coordTest(coordDst) || !chess.move(coordSrc.first, coordSrc.second, coordDst.first, coordDst.second)) {
                cout << "Unable to move like this!" << endl;
            }
        }

        cout << endl;
    }

    return 0;
}
