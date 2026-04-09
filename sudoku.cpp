#include <iostream>
using namespace std;

const int N = 9;

// Predefined puzzle
int puzzle[N][N] = {
    {3, 0, 6, 5, 0, 8, 4, 0, 0},
    {5, 2, 0, 0, 0, 0, 0, 0, 0},
    {0, 8, 7, 0, 0, 0, 0, 3, 1},
    {0, 0, 3, 0, 1, 0, 0, 8, 0},
    {9, 0, 0, 8, 6, 3, 0, 0, 5},
    {0, 5, 0, 0, 9, 0, 6, 0, 0},
    {1, 3, 0, 0, 0, 0, 2, 5, 0},
    {0, 0, 0, 0, 0, 0, 0, 7, 4},
    {0, 0, 5, 2, 0, 6, 3, 0, 0}
};

// Correct solution
int solution[N][N] = {
    {3,1,6,5,7,8,4,9,2},
    {5,2,9,1,3,4,7,6,8},
    {4,8,7,6,2,9,5,3,1},
    {2,6,3,4,1,5,9,8,7},
    {9,7,4,8,6,3,1,2,5},
    {8,5,1,7,9,2,6,4,3},
    {1,3,8,9,4,7,2,5,6},
    {6,9,2,3,5,1,8,7,4},
    {7,4,5,2,8,6,3,1,9}
};

// Print board
void printBoard(int board[N][N]) {
    cout << "\n-------------------------\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j % 3 == 0) cout << "| ";
            cout << board[i][j] << " ";
        }
        cout << "|\n";
        if ((i + 1) % 3 == 0)
            cout << "-------------------------\n";
    }
}

int main() {
    int user[N][N];

    cout << "🧩 Sudoku Puzzle:\n";
    printBoard(puzzle);

    cout << "\nFill the missing values (0 cells)\n";

    // Copy puzzle to user board
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            user[i][j] = puzzle[i][j];

    // Take user input only for empty cells
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (puzzle[i][j] == 0) {
                cout << "Enter value for row " << i+1 << " col " << j+1 << ": ";
                cin >> user[i][j];
            }
        }
    }

    // Check solution
    bool correct = true;

    for (int i = 0; i < N && correct; i++) {
        for (int j = 0; j < N; j++) {
            if (user[i][j] != solution[i][j]) {
                correct = false;
                break;
            }
        }
    }

    cout << "\nYour Solution:\n";
    printBoard(user);

    if (correct)
        cout << "\n🎉 Correct Solution!\n";
    else
        cout << "\n❌ Wrong Solution. Try again!\n";

    system("pause");
    return 0;
}