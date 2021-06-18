#include <iostream>
#include <limits>

#include "Chess.h"

using namespace std;
using namespace SimpleChess;

void printBoard(Chess &chess) {
    Board const &board = chess.getBoard();

    cout << "    ";
    for(int col = 0; col < BOARD_WIDTH; ++col) {
        cout << static_cast<char>('a'+col);
        if(col != BOARD_WIDTH-1) {
            cout << " ";
        }
    }
    cout << endl;

    cout << "   ";
    for(int col = 0; col < BOARD_WIDTH * 2 + 1; ++col) {
        cout << "-";
    }
    cout << endl;

    for(int row = BOARD_HEIGHT-1; row >= 0; --row) {
        cout << static_cast<char>('1'+row) << " | ";
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
        cout << " | " << static_cast<char>('1'+row) << endl;
    }

    cout << "   ";
    for(int col = 0; col < BOARD_WIDTH * 2 + 1; ++col) {
        cout << "-";
    }
    cout << endl;

    cout << "    ";
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
            cout << "Please choose how to promote your pawn. You can choose from: q r b n";
            cout << endl << "Please choose one: ";

            string input;
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if(input.size() != 1 || !chess.promotePawn(input.at(0))) {
                cout << "This promotion is not possible!" << endl;
            }
        } else {
            printBoard(chess);
            if(chess.getState() == SimpleChess::check) {
                cout << (chess.whoseTurnIsIt() == white ? "White" : "Black") << " is in check!" << endl;
            }

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

    cout << (chess.whoseTurnIsIt() == white ? "Black" : "White") << " won the game!" << endl;
    cout << (chess.whoseTurnIsIt() == white ? "White" : "Black") << " is " << (chess.getState() == checkMate ? "check" : "stall") << " mate." << endl;

    return 0;
}
