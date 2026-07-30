#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <conio.h>
#include <windows.h>

using namespace std;

// --- Screen & Engine Settings ---
const int SCREEN_WIDTH = 100;
const int SCREEN_HEIGHT = 40;
const int MAP_WIDTH = 16;
const int MAP_HEIGHT = 16;
const float FOV = 3.14159f / 4.0f; // Field of View (45 degrees)
const float DEPTH = 16.0f;         // Maximum drawing distance

// Hide the blinking cursor for smooth rendering
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Move cursor to top-left to avoid flickering
void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    hideCursor();
    system("cls"); // Clear terminal once

    // Player State
    float playerX = 2.0f;
    float playerY = 2.0f;
    float playerAngle = 0.0f;

    // 2D Map (16x16)
    // # = Wall, . = Empty Space
    wstring map = L"";
    map += L"################";
    map += L"#..............#";
    map += L"#.......########";
    map += L"#..............#";
    map += L"#......##......#";
    map += L"#......##......#";
    map += L"#..............#";
    map += L"###............#";
    map += L"#......####....#";
    map += L"#......#.......#";
    map += L"#......#.......#";
    map += L"#..............#";
    map += L"#......#########";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    bool gameRunning = true;

    while (gameRunning) {
        // --- 1. Player Input ---
        if (_kbhit()) {
            char key = _getch();
            
            // Rotation
            if (key == 'a' || key == 'A') playerAngle -= 0.1f;
            if (key == 'd' || key == 'D') playerAngle += 0.1f;
            
            // Forward / Backward movement with collision detection
            if (key == 'w' || key == 'W') {
                playerX += sinf(playerAngle) * 0.5f;
                playerY += cosf(playerAngle) * 0.5f;
                // Wall Collision: If we step into a wall, step back
                if (map[(int)playerY * MAP_WIDTH + (int)playerX] == '#') {
                    playerX -= sinf(playerAngle) * 0.5f;
                    playerY -= cosf(playerAngle) * 0.5f;
                }
            }
            if (key == 's' || key == 'S') {
                playerX -= sinf(playerAngle) * 0.5f;
                playerY -= cosf(playerAngle) * 0.5f;
                // Wall Collision
                if (map[(int)playerY * MAP_WIDTH + (int)playerX] == '#') {
                    playerX += sinf(playerAngle) * 0.5f;
                    playerY += cosf(playerAngle) * 0.5f;
                }
            }
            // Exit
            if (key == 'x' || key == 'X') gameRunning = false;
        }

        // --- 2. Render 3D Frame Buffer ---
        string frame = "";

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            // For each column, calculate the projected ray angle into world space
            float rayAngle = (playerAngle - FOV / 2.0f) + ((float)x / (float)SCREEN_WIDTH) * FOV;

            float distanceToWall = 0.0f;
            bool hitWall = false;

            // Unit vector for ray in player space
            float eyeX = sinf(rayAngle);
            float eyeY = cosf(rayAngle);

            // Raycasting Loop: Step out from player until we hit a wall
            while (!hitWall && distanceToWall < DEPTH) {
                distanceToWall += 0.1f;
                
                int testX = (int)(playerX + eyeX * distanceToWall);
                int testY = (int)(playerY + eyeY * distanceToWall);

                // Test if ray is out of bounds
                if (testX < 0 || testX >= MAP_WIDTH || testY < 0 || testY >= MAP_HEIGHT) {
                    hitWall = true; 
                    distanceToWall = DEPTH;
                } else {
                    // Test if ray hits a wall block
                    if (map[testY * MAP_WIDTH + testX] == '#') {
                        hitWall = true;
                    }
                }
            }

            // Calculate distance to ceiling and floor
            int ceiling = (float)(SCREEN_HEIGHT / 2.0) - SCREEN_HEIGHT / ((float)distanceToWall);
            int floor = SCREEN_HEIGHT - ceiling;

            // ASCII Shading based on distance (Fog effect)
            char wallShade;
            if (distanceToWall <= DEPTH / 4.0f)     wallShade = '#'; // Close
            else if (distanceToWall < DEPTH / 3.0f) wallShade = '8';
            else if (distanceToWall < DEPTH / 2.0f) wallShade = '+';
            else if (distanceToWall < DEPTH)        wallShade = ':';
            else                                    wallShade = '.'; // Too far

            // Draw the column pixel by pixel
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                if (y <= ceiling) {
                    frame += " "; // Sky
                } else if (y > ceiling && y <= floor) {
                    frame += wallShade; // Wall
                } else {
                    // Draw Floor with shading based on depth
                    float b = 1.0f - (((float)y - SCREEN_HEIGHT / 2.0f) / ((float)SCREEN_HEIGHT / 2.0f));
                    if (b < 0.25)      frame += "x";
                    else if (b < 0.5)  frame += "-";
                    else if (b < 0.75) frame += ".";
                    else               frame += " ";
                }
            }
            frame += "\n";
        }

        // --- 3. Draw Mini-Map (HUD) ---
        // Overwrite the top left of the frame buffer with the 2D map
        int mapOffset = 0;
        for (int my = 0; my < MAP_HEIGHT; my++) {
            for (int mx = 0; mx < MAP_WIDTH; mx++) {
                char mapChar = map[my * MAP_WIDTH + mx];
                // Represent player on the minimap
                if ((int)playerX == mx && (int)playerY == my) {
                    mapChar = 'P';
                }
                // Calculate position in the 1D frame string
                int framePos = my * (SCREEN_WIDTH + 1) + mx + 2; 
                frame[framePos] = mapChar;
            }
        }

        // Add instructions to the bottom
        frame += " CONTROLS: [W/S] Move Forward/Back | [A/D] Turn Left/Right | [X] Exit\n";

        // Push frame buffer to screen
        setCursorPosition(0, 0);
        cout << frame;
    }

    return 0;
}
