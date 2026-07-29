#include <iostream>
#include <vector>
#include <string>
#include <conio.h>    // For _kbhit() and _getch()
#include <windows.h>  // For Sleep() and SetConsoleCursorPosition
#include <cstdlib>
#include <ctime>

using namespace std;

// --- Game Settings ---
const int WIDTH = 40;
const int HEIGHT = 20;

struct Entity {
    int x, y;
};

// Utility to hide the blinking cursor
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Utility to overwrite the screen smoothly
void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    hideCursor();
    system("cls");

    // Game State
    bool gameOver = false;
    int score = 0;
    
    Entity player = { WIDTH / 2, HEIGHT - 2 };
    vector<Entity> bullets;
    vector<Entity> enemies;

    int enemySpawnTimer = 0;

    // --- Main Game Loop ---
    while (!gameOver) {
        // 1. Input Handling
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'a': case 'A': if (player.x > 1) player.x--; break;
                case 'd': case 'D': if (player.x < WIDTH - 2) player.x++; break;
                case ' ': // Spacebar to shoot
                    bullets.push_back({player.x, player.y - 1});
                    break;
                case 'x': case 'X': gameOver = true; break;
            }
        }

        // 2. Update Bullets
        for (size_t i = 0; i < bullets.size(); ) {
            bullets[i].y--; // Move bullet up
            if (bullets[i].y < 0) {
                bullets.erase(bullets.begin() + i); // Remove if off-screen
            } else {
                i++;
            }
        }

        // 3. Update Enemies
        enemySpawnTimer++;
        if (enemySpawnTimer > 5) { // Spawn rate
            enemies.push_back({rand() % (WIDTH - 2) + 1, 0});
            enemySpawnTimer = 0;
        }

        for (size_t i = 0; i < enemies.size(); ) {
            // Move enemies down slowly (only update their Y every few frames to simulate speed)
            if (rand() % 2 == 0) enemies[i].y++; 

            // Check if enemy hit the bottom (Game Over)
            if (enemies[i].y >= HEIGHT - 1) {
                gameOver = true;
            }
            
            // Check if enemy hit the player (Game Over)
            if (enemies[i].x == player.x && enemies[i].y == player.y) {
                gameOver = true;
            }
            i++;
        }

        // 4. Collision Detection (Bullets hitting Enemies)
        for (size_t i = 0; i < bullets.size(); ) {
            bool bulletHit = false;
            for (size_t j = 0; j < enemies.size(); ) {
                if (bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                    score += 50; // Boom!
                    enemies.erase(enemies.begin() + j); // Destroy enemy
                    bulletHit = true;
                    break; // Bullet is destroyed, stop checking this bullet
                } else {
                    j++;
                }
            }
            
            if (bulletHit) {
                bullets.erase(bullets.begin() + i); // Destroy bullet
            } else {
                i++;
            }
        }

        // 5. Render Screen
        string frame = "";

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                    frame += "*"; // Draw Borders
                } else if (x == player.x && y == player.y) {
                    frame += "^"; // Draw Player Ship
                } else {
                    bool drawn = false;
                    
                    // Draw Bullets
                    for (const auto& b : bullets) {
                        if (b.x == x && b.y == y) {
                            frame += "|";
                            drawn = true;
                            break;
                        }
                    }
                    
                    // Draw Enemies
                    if (!drawn) {
                        for (const auto& e : enemies) {
                            if (e.x == x && e.y == y) {
                                frame += "V";
                                drawn = true;
                                break;
                            }
                        }
                    }

                    if (!drawn) frame += " ";
                }
            }
            frame += "\n";
        }
        
        frame += " SCORE: " + to_string(score) + "   |   [A/D] Move   [SPACE] Shoot\n";

        setCursorPosition(0, 0);
        cout << frame;

        // Game Speed
        Sleep(40);
    }

    // --- Game Over Sequence ---
    setCursorPosition(0, HEIGHT + 2);
    cout << "======================================\n";
    cout << "          MISSION FAILED! \n";
    cout << "   The alien swarm breached Earth.\n";
    cout << "          Final Score: " << score << "\n";
    cout << "======================================\n";

    return 0;
}
