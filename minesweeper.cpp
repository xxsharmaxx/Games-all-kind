#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>

using namespace std;

// --- Game Configuration ---
const int WIDTH = 20;
const int HEIGHT = 15;
const int TOTAL_MINES = 40;

// Hides the blinking console cursor
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Moves cursor to top-left for flicker-free rendering
void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// --- Algorithm: Depth-First Search (Flood Fill) ---
// Recursively reveals adjacent empty cells until it hits a number
void floodFill(int x, int y, const vector<vector<int>>& board, vector<vector<bool>>& revealed, vector<vector<bool>>& flagged) {
    // Base cases: Out of bounds, already revealed, or flagged
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || revealed[y][x] || flagged[y][x]) {
        return;
    }

    revealed[y][x] = true;

    // If the cell is empty (0), recursively check all 8 surrounding neighbors
    if (board[y][x] == 0) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx != 0 || dy != 0) {
                    floodFill(x + dx, y + dy, board, revealed, flagged);
                }
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    hideCursor();
    system("cls");

    // Game State Matrices
    // board: -1 = Mine, 0 = Empty, 1-8 = Adjacent Mines
    vector<vector<int>> board(HEIGHT, vector<int>(WIDTH, 0));
    vector<vector<bool>> revealed(HEIGHT, vector<bool>(WIDTH, false));
    vector<vector<bool>> flagged(HEIGHT, vector<bool>(WIDTH, false));

    // 1. Plant Mines
    int minesPlanted = 0;
    while (minesPlanted < TOTAL_MINES) {
        int rx = rand() % WIDTH;
        int ry = rand() % HEIGHT;
        if (board[ry][rx] != -1) {
            board[ry][rx] = -1;
            minesPlanted++;
        }
    }

    // 2. Calculate Adjacency Numbers
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (board[y][x] == -1) continue;
            
            int mineCount = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int ny = y + dy;
                    int nx = x + dx;
                    if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH) {
                        if (board[ny][nx] == -1) mineCount++;
                    }
                }
            }
            board[y][x] = mineCount;
        }
    }

    // 3. Main Game Variables
    int cursorX = WIDTH / 2;
    int cursorY = HEIGHT / 2;
    bool gameOver = false;
    bool victory = false;

    // --- Main Game Loop ---
    while (!gameOver && !victory) {
        
        // Render Frame
        string frame = "   MINESWEEPER: [SPACE] Reveal | [F] Flag | [WASD] Move\n\n";
        
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                bool isCursor = (x == cursorX && y == cursorY);
                
                // Draw cursor brackets
                if (isCursor) frame += "[";
                else frame += " ";

                // Draw cell contents
                if (flagged[y][x]) {
                    frame += "P"; // Flag
                } else if (!revealed[y][x]) {
                    frame += "#"; // Unrevealed tile
                } else if (board[y][x] == -1) {
                    frame += "*"; // Mine
                } else if (board[y][x] == 0) {
                    frame += "."; // Empty space
                } else {
                    frame += to_string(board[y][x]); // Number
                }

                if (isCursor) frame += "]";
                else frame += " ";
            }
            frame += "\n";
        }
        
        setCursorPosition(0, 0);
        cout << frame;

        // Input Handling
        char key = _getch();
        switch (key) {
            case 'w': case 'W': if (cursorY > 0) cursorY--; break;
            case 's': case 'S': if (cursorY < HEIGHT - 1) cursorY++; break;
            case 'a': case 'A': if (cursorX > 0) cursorX--; break;
            case 'd': case 'D': if (cursorX < WIDTH - 1) cursorX++; break;
            
            case 'f': case 'F': // Flagging
                if (!revealed[cursorY][cursorX]) {
                    flagged[cursorY][cursorX] = !flagged[cursorY][cursorX];
                }
                break;
                
            case ' ': // Reveal
                if (!flagged[cursorY][cursorX] && !revealed[cursorY][cursorX]) {
                    if (board[cursorY][cursorX] == -1) {
                        gameOver = true; // Hit a mine!
                    } else {
                        floodFill(cursorX, cursorY, board, revealed, flagged);
                    }
                }
                break;
        }

        // Win Condition Check: Are all non-mine tiles revealed?
        int revealedCount = 0;
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (revealed[y][x]) revealedCount++;
            }
        }
        if (revealedCount == (WIDTH * HEIGHT) - TOTAL_MINES) {
            victory = true;
        }
    }

    // --- End Sequence ---
    setCursorPosition(0, HEIGHT + 4);
    if (victory) {
        cout << "======================================\n";
        cout << "   🏆 MISSION ACCOMPLISHED! 🏆\n";
        cout << "   You cleared the minefield.\n";
        cout << "======================================\n";
    } else {
        cout << "======================================\n";
        cout << "   💥 BOOM! YOU HIT A MINE! 💥\n";
        cout << "   Game Over.\n";
        cout << "======================================\n";
    }

    return 0;
}
