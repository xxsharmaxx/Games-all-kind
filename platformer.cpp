#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

// --- Screen & Level Settings ---
const int SCREEN_WIDTH = 60;
const int SCREEN_HEIGHT = 15;

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

int main() {
    hideCursor();
    system("cls");

    // The Level Design (# = Floor/Wall, * = Coin, | = Goal Flag)
    vector<string> level = {
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                     ***                            ",
        "                                                                    #####                           ",
        "                                                                                                    ",
        "                                         ***                                                        ",
        "                                        #####                                                       ",
        "                                                                                                    ",
        "                 *                                  ***                       ##                    ",
        "                ###                                #####                     ####                   ",
        "                                                                            ######                  ",
        "                                   ##                              ##      ########            |    ",
        "###   ########   #######   ##########   #######   #########   #######################   ############",
        "###   ########   #######   ##########   #######   #########   #######################   ############",
        "###   ########   #######   ##########   #######   #########   #######################   ############"
    };

    int levelWidth = level[0].length();
    int levelHeight = level.size();

    // Player Physics State
    float pX = 1.0f;
    float pY = 10.0f;
    float vX = 0.0f;
    float vY = 0.0f;
    bool onGround = false;
    
    int score = 0;
    bool gameOver = false;
    bool won = false;

    // Helper to safely read from the map
    auto getTile = [&](float x, float y) {
        int ix = (int)x;
        int iy = (int)y;
        if (ix >= 0 && ix < levelWidth && iy >= 0 && iy < levelHeight)
            return level[iy][ix];
        return ' ';
    };

    // Helper to safely write to the map (for collecting coins)
    auto setTile = [&](float x, float y, char c) {
        int ix = (int)x;
        int iy = (int)y;
        if (ix >= 0 && ix < levelWidth && iy >= 0 && iy < levelHeight)
            level[iy][ix] = c;
    };

    float deltaTime = 0.05f; // Simulation step

    // --- Main Game Loop ---
    while (!gameOver && !won) {
        
        // 1. Asynchronous Input (allows pressing Jump while Running)
        vX = 0.0f;
        if (GetAsyncKeyState('A') & 0x8000) vX = -8.0f; // Move Left
        if (GetAsyncKeyState('D') & 0x8000) vX = 8.0f;  // Move Right
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (onGround) {
                vY = -12.0f; // Jump force
                onGround = false;
            }
        }
        if (GetAsyncKeyState('X') & 0x8000) gameOver = true;

        // 2. Apply Gravity
        vY += 20.0f * deltaTime;

        // 3. X Movement & Collision
        float newPX = pX + vX * deltaTime;
        if (vX <= 0) { // Moving Left
            if (getTile(newPX, pY) == '#' || getTile(newPX, pY + 0.9f) == '#') {
                newPX = (int)newPX + 1; // Snap to grid
                vX = 0;
            }
        } else { // Moving Right
            if (getTile(newPX + 1.0f, pY) == '#' || getTile(newPX + 1.0f, pY + 0.9f) == '#') {
                newPX = (int)newPX; // Snap to grid
                vX = 0;
            }
        }
        pX = newPX;

        // 4. Y Movement & Collision
        float newPY = pY + vY * deltaTime;
        if (vY <= 0) { // Moving Up (Jumping)
            if (getTile(pX, newPY) == '#' || getTile(pX + 0.9f, newPY) == '#') {
                newPY = (int)newPY + 1; // Bonk head
                vY = 0;
            }
        } else { // Moving Down (Falling)
            if (getTile(pX, newPY + 1.0f) == '#' || getTile(pX + 0.9f, newPY + 1.0f) == '#') {
                newPY = (int)newPY; // Land on ground
                vY = 0;
                onGround = true;
            } else {
                onGround = false;
            }
        }
        pY = newPY;

        // 5. Check Interactions (Coins, Goal, Death)
        char tileCenter = getTile(pX + 0.5f, pY + 0.5f);
        if (tileCenter == '*') {
            score += 10;
            setTile(pX + 0.5f, pY + 0.5f, ' '); // Remove coin
        } else if (tileCenter == '|') {
            won = true;
        }

        // Check if player fell into a pit
        if (pY > levelHeight) {
            gameOver = true;
        }

        // 6. Camera Tracking Logic
        int cameraX = (int)pX - SCREEN_WIDTH / 2;
        if (cameraX < 0) cameraX = 0; // Lock to left edge
        if (cameraX > levelWidth - SCREEN_WIDTH) cameraX = levelWidth - SCREEN_WIDTH; // Lock to right edge

        // 7. Render Screen
        string frame = "";
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                int worldX = cameraX + x;
                int worldY = y;
                
                // Draw Player
                if (worldX == (int)pX && worldY == (int)pY) {
                    frame += "@";
                } 
                else {
                    frame += getTile(worldX, worldY);
                }
            }
            frame += "\n";
        }
        frame += " SCORE: " + to_string(score) + "   |   [A/D] Run Left/Right   [SPACE] Jump\n";

        setCursorPosition(0, 0);
        cout << frame;

        // 8. Control Game Speed
        Sleep(50);
    }

    // --- End Screen ---
    setCursorPosition(0, SCREEN_HEIGHT + 2);
    if (won) {
        cout << "======================================\n";
        cout << "   🎉 YOU BEAT THE LEVEL! 🎉\n";
        cout << "   Final Score: " << score << "\n";
        cout << "======================================\n";
    } else {
        cout << "======================================\n";
        cout << "   💥 YOU FELL INTO THE ABYSS! 💥\n";
        cout << "   Final Score: " << score << "\n";
        cout << "======================================\n";
    }

    return 0;
}
