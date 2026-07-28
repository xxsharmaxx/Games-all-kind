#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to draw the current state of the board
void drawBoard(const vector<char>& board) {
    cout << "\n";
    cout << "     |     |     \n";
    cout << "  " << board[0] << "  |  " << board[1] << "  |  " << board[2] << "  \n";
    cout << "_____|_____|_____\n";
    cout << "     |     |     \n";
    cout << "  " << board[3] << "  |  " << board[4] << "  |  " << board[5] << "  \n";
    cout << "_____|_____|_____\n";
    cout << "     |     |     \n";
    cout << "  " << board[6] << "  |  " << board[7] << "  |  " << board[8] << "  \n";
    cout << "     |     |     \n\n";
}

// Function to check if a specific player has won
bool checkWin(const vector<char>& board, char player) {
    // All 8 possible winning combinations
    const int winCombinations[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    for (int i = 0; i < 8; i++) {
        if (board[winCombinations[i][0]] == player &&
            board[winCombinations[i][1]] == player &&
            board[winCombinations[i][2]] == player) {
            return true;
        }
    }
    return false;
}

// Function to check if the board is completely full (a tie)
bool checkTie(const vector<char>& board) {
    for (char cell : board) {
        if (cell != 'X' && cell != 'O') {
            return false; // There is still an empty spot (a number)
        }
    }
    return true;
}

int main() {
    // Initialize board with numbers 1-9 to guide player input
    vector<char> board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char currentPlayer = 'X';
    int choice;
    bool gameOngoing = true;

    cout << "==========================\n";
    cout << "   WELCOME TO TIC-TAC-TOE \n";
    cout << "==========================\n";

    while (gameOngoing) {
        drawBoard(board);
        cout << "Player " << currentPlayer << ", enter a number (1-9): ";
        cin >> choice;

        // Input validation
        if (cin.fail() || choice < 1 || choice > 9) {
            cin.clear(); // clear error flags
            cin.ignore(10000, '\n'); // discard invalid input
            cout << "Invalid input! Please enter a number between 1 and 9.\n";
            continue;
        }

        // Check if the chosen spot is already taken
        if (board[choice - 1] == 'X' || board[choice - 1] == 'O') {
            cout << "That spot is already taken! Choose another one.\n";
            continue;
        }

        // Place the player's mark on the board
        board[choice - 1] = currentPlayer;

        // Check for a win
        if (checkWin(board, currentPlayer)) {
            drawBoard(board);
            cout << "🎉 Congratulations! Player " << currentPlayer << " wins! 🎉\n";
            gameOngoing = false;
        } 
        // Check for a tie
        else if (checkTie(board)) {
            drawBoard(board);
            cout << "It's a tie! Well played both.\n";
            gameOngoing = false;
        } 
        // Switch turns
        else {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    return 0;
}
