#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int SIZE = 5;
const int MINES = 5;

char board[SIZE][SIZE];
bool revealed[SIZE][SIZE];

void initBoard() {
    for(int i=0;i<SIZE;i++)
        for(int j=0;j<SIZE;j++) {
            board[i][j] = '0';
            revealed[i][j] = false;
        }

    int placed = 0;
    while(placed < MINES) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;

        if(board[x][y] != 'M') {
            board[x][y] = 'M';
            placed++;
        }
    }

    // calculate numbers
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<SIZE;j++) {
            if(board[i][j] == 'M') continue;

            int count = 0;
            for(int dx=-1;dx<=1;dx++) {
                for(int dy=-1;dy<=1;dy++) {
                    int ni = i + dx;
                    int nj = j + dy;

                    if(ni>=0 && ni<SIZE && nj>=0 && nj<SIZE &&
                       board[ni][nj]=='M')
                        count++;
                }
            }
            board[i][j] = count + '0';
        }
    }
}

void printBoard(bool showAll=false) {
    cout << "\n  ";
    for(int i=0;i<SIZE;i++) cout << i << " ";
    cout << endl;

    for(int i=0;i<SIZE;i++) {
        cout << i << " ";
        for(int j=0;j<SIZE;j++) {
            if(revealed[i][j] || showAll)
                cout << board[i][j] << " ";
            else
                cout << "# ";
        }
        cout << endl;
    }
}

int main() {
    srand(time(0));
    initBoard();

    int x, y;
    int safeCells = SIZE*SIZE - MINES;
    int revealedCount = 0;

    cout << "💣 Minesweeper Game\n";

    while(true) {
        printBoard();
        cout << "Enter row and column: ";
        cin >> x >> y;

        if(x<0 || x>=SIZE || y<0 || y>=SIZE) {
            cout << "Invalid input!\n";
            continue;
        }

        if(board[x][y] == 'M') {
            printBoard(true);
            cout << "💀 BOOM! You hit a mine!\n";
            break;
        }

        if(!revealed[x][y]) {
            revealed[x][y] = true;
            revealedCount++;
        }

        if(revealedCount == safeCells) {
            printBoard(true);
            cout << "🎉 You Win!\n";
            break;
        }
    }

    return 0;
}
