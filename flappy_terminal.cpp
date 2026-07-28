#include <iostream>
#include <vector>
#include <string>
#include <conio.h>    // For _kbhit() and _getch() (Windows only)
#include <windows.h>  // For Sleep() and console manipulation (Windows only)
#include <cstdlib>
#include <ctime>

using namespace std;

// --- Game Settings ---
const int SCREEN_WIDTH = 50;
const int SCREEN_HEIGHT = 20;
const int GRAVITY = 1;
const int JUMP_STRENGTH = -3;
const int PIPE_SPEED = 2;
const int GAP_SIZE = 6;

// --- Utility Functions ---
// Hides the blinking console cursor for a smoother game experience
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Moves the cursor to the top-left instead of clearing the screen to prevent flickering
void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    hideCursor();
    system("cls");

    // Game Variables
    float birdY = SCREEN_HEIGHT / 2.0f;
    float birdVelocity = 0.0f;
    int pipeX = SCREEN_WIDTH;
    int pipeGapY = rand() % (SCREEN_HEIGHT - GAP_SIZE - 4) + 2;
    int score = 0;
    bool gameOver = false;

    // --- Main Game Loop ---
    while (!gameOver) {
        // 1. Handle Input (Non-blocking)
        if (_kbhit()) {
            char key = _getch();
            if (key == ' ') {
                birdVelocity = JUMP_STRENGTH; // Flap wings
            }
        }

        // 2. Update Physics
        birdVelocity += 0.5f; // Apply gravity (0.5 per frame)
        birdY += birdVelocity;

        // 3. Update Environment
        pipeX -= 2; // Move pipes to the left
        if (pipeX < 0) {
            pipeX = SCREEN_WIDTH;
            pipeGapY = rand() % (SCREEN_HEIGHT - GAP_SIZE - 4) + 2;
            score++;
        }

        // 4. Collision Detection
        // Floor and Ceiling collision
        if (birdY < 0 || birdY >= SCREEN_HEIGHT) {
            gameOver = true;
        }
        // Pipe collision
        int birdX = 10; // Bird is locked at X position 10
        if (birdX >= pipeX && birdX <= pipeX + 3) {
            if (birdY <= pipeGapY || birdY >= pipeGapY + GAP_SIZE) {
                gameOver = true;
            }
        }

        // 5. Render Screen (Frame Buffering)
        string frame = "";
        
        // Top Border
        frame += string(SCREEN_WIDTH, '=') + "\n";

        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                // Draw Bird
                if (x == birdX && y == static_cast<int>(birdY)) {
                    frame += "@";
                }
                // Draw Pipe
                else if (x >= pipeX && x <= pipeX + 3 && (y <= pipeGapY || y >= pipeGapY + GAP_SIZE)) {
                    frame += "#";
                }
                // Draw empty space
                else {
                    frame += " ";
                }
            }
            frame += "\n";
        }
        
        // Bottom Border and UI
        frame += string(SCREEN_WIDTH, '=') + "\n";
        frame += " SCORE: " + to_string(score) + "   |   Press SPACE to Flap!\n";

        // Draw the buffered frame to the screen
        setCursorPosition(0, 0);
        cout << frame;

        // 6. Frame Rate Control (approx 20 FPS)
        Sleep(50); 
    }

    // --- Game Over Screen ---
    setCursorPosition(0, SCREEN_HEIGHT + 3);
    cout << "\n💥 CRASH! 💥\n";
    cout << "GAME OVER! Final Score: " << score << "\n";
    
    // Pause before exiting
    cout << "Press any key to exit...";
    while (_kbhit()) _getch(); // Clear input buffer
    _getch(); 

    return 0;
}
