#include <iostream>
#include <vector>
#include <conio.h>    // For _kbhit() and _getch() (Windows only)
#include <windows.h>  // For Sleep() and SetConsoleCursorPosition (Windows only)
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

// --- Game Settings ---
const int WIDTH = 40;
const int HEIGHT = 20;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

struct Point {
    int x, y;
};

// Hides the blinking console cursor
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Repositions the cursor to top-left to avoid screen flickering
void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    hideCursor();
    system("cls");

    // Snake & Game State Variables
    bool gameOver = false;
    Point head = { WIDTH / 2, HEIGHT / 2 };
    vector<Point> tail;
    Point food = { rand() % WIDTH, rand() % HEIGHT };
    Direction dir = RIGHT;
    int score = 0;

    // --- Main Game Loop ---
    while (!gameOver) {
        // 1. Non-Blocking Key Input
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'a': case 'A': if (dir != RIGHT) dir = LEFT; break;
                case 'd': case 'D': if (dir != LEFT) dir = RIGHT; break;
                case 'w': case 'W': if (dir != DOWN) dir = UP; break;
                case 's': case 'S': if (dir != UP) dir = DOWN; break;
                case 'x': case 'X': gameOver = true; break; // Exit game
            }
        }

        // 2. Update Movement Logic
        Point prevHead = head;
        switch (dir) {
            case LEFT:  head.x--; break;
            case RIGHT: head.x++; break;
            case UP:    head.y--; break;
            case DOWN:  head.y++; break;
            default: break;
        }

        // Wall Collision Check
        if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
            gameOver = true;
        }

        // Self-Collision Check
        for (const auto& segment : tail) {
            if (segment.x == head.x && segment.y == head.y) {
                gameOver = true;
            }
        }

        // Move Body Tail
        if (!gameOver) {
            if (!tail.empty()) {
                for (size_t i = tail.size() - 1; i > 0; i--) {
                    tail[i] = tail[i - 1];
                }
                tail[0] = prevHead;
            }

            // Food Collision & Snake Growth
            if (head.x == food.x && head.y == food.y) {
                score += 10;
                tail.push_back(prevHead); // Grow tail
                food.x = rand() % WIDTH;
                food.y = rand() % HEIGHT;
            }
        }

        // 3. Render Frame
        string frame = "";
        
        // Top Border
        frame += string(WIDTH + 2, '#') + "\n";

        for (int y = 0; y < HEIGHT; y++) {
            frame += "#"; // Left Border
            for (int x = 0; x < WIDTH; x++) {
                if (x == head.x && y == head.y) {
                    frame += "O"; // Snake Head
                } else if (x == food.x && y == food.y) {
                    frame += "*"; // Food Item
                } else {
                    bool isTail = false;
                    for (const auto& segment : tail) {
                        if (segment.x == x && segment.y == y) {
                            frame += "o"; // Snake Body
                            isTail = true;
                            break;
                        }
                    }
                    if (!isTail) frame += " ";
                }
            }
            frame += "#\n"; // Right Border
        }

        // Bottom Border & Controls Display
        frame += string(WIDTH + 2, '#') + "\n";
        frame += " SCORE: " + to_string(score) + "   |   Controls: [W][A][S][D]  (Press [X] to Quit)\n";

        // Draw buffer to screen
        setCursorPosition(0, 0);
        cout << frame;

        // Game Speed (lower number = faster game)
        Sleep(70);
    }

    // Game Over Message
    setCursorPosition(0, HEIGHT + 4);
    cout << "\n==============================\n";
    cout << "   💥 GAME OVER! 💥\n";
    cout << "   Final Score: " << score << "\n";
    cout << "==============================\n\n";

    return 0;
}
