#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <ctime>

using namespace std;

// --- Config ---
const int WIDTH = 28;
const int HEIGHT = 23;

// Hides blinking cursor
void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 100;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cci);
}

// Flicker-free console rendering
void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct Entity {
    int x, y;
    int dx, dy;
    int startX, startY;
};

int main() {
    srand(static_cast<unsigned>(time(0)));
    hideCursor();
    system("cls");

    // Level Design 
    // # = Wall, . = Dot, O = Power Pellet, - = Ghost Gate
    vector<string> map = {
        "############################",
        "#............##............#",
        "#.####.#####.##.#####.####.#",
        "#O####.#####.##.#####.####O#",
        "#.####.#####.##.#####.####.#",
        "#..........................#",
        "#.####.##.########.##.####.#",
        "#......##....##....##......#",
        "######.##### ## #####.######",
        "     #.##### ## #####.#     ",
        "     #.##          ##.#     ",
        "     #.## ###--### ##.#     ",
        "######.## #      # ##.######",
        "      .   #      #   .      ",
        "######.## ######## ##.######",
        "     #.##          ##.#     ",
        "     #.## ######## ##.#     ",
        "######.## ######## ##.######",
        "#............##............#",
        "#.####.#####.##.#####.####.#",
        "#O..##.......  .......##..O#",
        "###.##.##.########.##.##.###",
        "#......##....##....##......#",
        "############################"
    };

    // Count dots for Win Condition
    int totalDots = 0;
    for (const auto& row : map) {
        for (char c : row) {
            if (c == '.' || c == 'O') totalDots++;
        }
    }

    // Entities
    Entity pac = { 13, 20, -1, 0, 13, 20 };
    int nextDx = -1, nextDy = 0;

    vector<Entity> ghosts = {
        { 12, 13, 1, 0, 12, 13 },
        { 15, 13, -1, 0, 15, 13 },
        { 13, 14, 0, -1, 13, 14 }
    };

    int score = 0;
    bool gameOver = false;
    bool victory = false;
    
    // Power Pellet State
    int scaredTimer = 0;

    // --- Main Game Loop ---
    while (!gameOver && !victory) {
        // 1. Input Processing
        if (GetAsyncKeyState(VK_UP) & 0x8000)    { nextDx = 0; nextDy = -1; }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)  { nextDx = 0; nextDy = 1; }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)  { nextDx = -1; nextDy = 0; }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { nextDx = 1; nextDy = 0; }
        if (GetAsyncKeyState('X') & 0x8000)      gameOver = true;

        // 2. Pac-Man Movement (Grid locked, turning logic)
        // If the intended turn is valid, update current direction
        if (map[pac.y + nextDy][pac.x + nextDx] != '#' && map[pac.y + nextDy][pac.x + nextDx] != '-') {
            pac.dx = nextDx;
            pac.dy = nextDy;
        }

        // Move in current direction if valid
        if (map[pac.y + pac.dy][pac.x + pac.dx] != '#' && map[pac.y + pac.dy][pac.x + pac.dx] != '-') {
            pac.x += pac.dx;
            pac.y += pac.dy;
        }

        // Wrap-around tunnels
        if (pac.x < 0) pac.x = WIDTH - 1;
        else if (pac.x >= WIDTH) pac.x = 0;

        // 3. Eating Dots & Power Pellets
        if (map[pac.y][pac.x] == '.') {
            map[pac.y][pac.x] = ' ';
            score += 10;
            totalDots--;
        } else if (map[pac.y][pac.x] == 'O') {
            map[pac.y][pac.x] = ' ';
            score += 50;
            totalDots--;
            scaredTimer = 40; // Ghosts become scared for ~40 frames
        }

        if (totalDots == 0) victory = true;
        if (scaredTimer > 0) scaredTimer--;

        // 4. Ghost AI (Intersection Decision Making)
        for (auto& g : ghosts) {
            // Find valid moves
            vector<pair<int, int>> validMoves;
            int dirs[4][2] = {{0,-1}, {0,1}, {-1,0}, {1,0}};
            
            for (auto& d : dirs) {
                // Don't walk into walls
                if (map[g.y + d[1]][g.x + d[0]] != '#') {
                    // Prevent immediate 180-degree turnaround unless stuck
                    if (!(d[0] == -g.dx && d[1] == -g.dy)) {
                        validMoves.push_back({d[0], d[1]});
                    }
                }
            }

            // Decide move
            if (validMoves.size() > 0) {
                bool canKeepGoing = false;
                for (auto& m : validMoves) {
                    if (m.first == g.dx && m.second == g.dy) canKeepGoing = true;
                }

                // Keep going straight 70% of the time, or randomly turn at intersections
                if (canKeepGoing && (rand() % 10) < 7) {
                    // Keep dx, dy the same
                } else {
                    auto move = validMoves[rand() % validMoves.size()];
                    g.dx = move.first;
                    g.dy = move.second;
                }
            } else {
                // Dead end: reverse direction
                g.dx = -g.dx;
                g.dy = -g.dy;
            }

            g.x += g.dx;
            g.y += g.dy;

            // Tunnel wrap for ghosts
            if (g.x < 0) g.x = WIDTH - 1;
            else if (g.x >= WIDTH) g.x = 0;

            // 5. Collision Detection
            if (pac.x == g.x && pac.y == g.y) {
                if (scaredTimer > 0) {
                    // Eat ghost
                    score += 200;
                    g.x = g.startX;
                    g.y = g.startY;
                } else {
                    gameOver = true; // Pacman dies
                }
            }
        }

        // 6. Rendering
        string frame = "";
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                // Draw Pac-Man
                if (x == pac.x && y == pac.y) {
                    frame += "C";
                    continue;
                }
                
                // Draw Ghosts
                bool isGhost = false;
                for (const auto& g : ghosts) {
                    if (x == g.x && y == g.y) {
                        frame += (scaredTimer > 0) ? "W" : "M";
                        isGhost = true;
                        break;
                    }
                }
                if (isGhost) continue;

                // Draw Map
                frame += map[y][x];
            }
            frame += "\n";
        }

        frame += "\n SCORE: " + to_string(score) + "   |   Dots Left: " + to_string(totalDots) + "\n";
        frame += " CONTROLS: Arrow Keys to move. [X] to Exit.\n";

        setCursorPosition(0, 0);
        cout << frame;

        // FPS Control
        Sleep(120); 
    }

    // --- End Screen ---
    setCursorPosition(0, HEIGHT + 4);
    if (victory) {
        cout << "======================================\n";
        cout << "   🎉 YOU CLEARED THE MAZE! 🎉\n";
        cout << "   Final Score: " << score << "\n";
        cout << "======================================\n";
    } else {
        cout << "======================================\n";
        cout << "   💥 CAUGHT BY A GHOST! 💥\n";
        cout << "   Final Score: " << score << "\n";
        cout << "======================================\n";
    }

    return 0;
}
