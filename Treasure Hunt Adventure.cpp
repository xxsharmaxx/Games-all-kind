#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int SIZE = 10;

char board[SIZE][SIZE];

int playerX = 0, playerY = 0;
int enemyX, enemyY;
int treasureX, treasureY;

int health = 3;
int score = 0;

void initialize() {
    srand(time(0));

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = '.';

    playerX = 0;
    playerY = 0;

    treasureX = rand() % SIZE;
    treasureY = rand() % SIZE;

    enemyX = rand() % SIZE;
    enemyY = rand() % SIZE;
}

void draw() {

    system("cls");

    for (int i = 0; i < SIZE; i++) {

        for (int j = 0; j < SIZE; j++) {

            if (i == playerX && j == playerY)
                cout << "P ";

            else if (i == treasureX && j == treasureY)
                cout << "$ ";

            else if (i == enemyX && j == enemyY)
                cout << "Z ";

            else
                cout << ". ";
        }

        cout << endl;
    }

    cout << "\nHealth : " << health;
    cout << "\nScore  : " << score;

    cout << "\n\nMove (W A S D): ";
}

void enemyMove() {

    int dir = rand() % 4;

    if (dir == 0 && enemyX > 0)
        enemyX--;

    if (dir == 1 && enemyX < SIZE - 1)
        enemyX++;

    if (dir == 2 && enemyY > 0)
        enemyY--;

    if (dir == 3 && enemyY < SIZE - 1)
        enemyY++;
}

int main() {

    initialize();

    while (true) {

        draw();

        char move;
        cin >> move;

        if (move == 'w' || move == 'W')
            if (playerX > 0)
                playerX--;

        if (move == 's' || move == 'S')
            if (playerX < SIZE - 1)
                playerX++;

        if (move == 'a' || move == 'A')
            if (playerY > 0)
                playerY--;

        if (move == 'd' || move == 'D')
            if (playerY < SIZE - 1)
                playerY++;

        enemyMove();

        if (playerX == treasureX && playerY == treasureY) {

            score += 10;

            treasureX = rand() % SIZE;
            treasureY = rand() % SIZE;
        }

        if (playerX == enemyX && playerY == enemyY) {

            health--;

            enemyX = rand() % SIZE;
            enemyY = rand() % SIZE;

            if (health == 0) {

                cout << "\n\nGame Over!";
                cout << "\nFinal Score : " << score;
                break;
            }
        }

        if (score >= 50) {

            cout << "\n\nCongratulations! You found enough treasure!";
            cout << "\nFinal Score : " << score;
            break;
        }
    }

    return 0;
}
