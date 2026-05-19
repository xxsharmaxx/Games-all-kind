#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 20;

int playerX = WIDTH / 2;
int score = 0;
bool gameOver = false;

struct Bullet {
    int x, y;
};

struct Enemy {
    int x, y;
};

vector<Bullet> bullets;
vector<Enemy> enemies;

// ================= DRAW GAME =================

void draw() {

    system("cls");

    for(int y = 0; y < HEIGHT; y++) {

        for(int x = 0; x < WIDTH; x++) {

            bool printed = false;

            // Player
            if(x == playerX && y == HEIGHT - 1) {
                cout << "A";
                printed = true;
            }

            // Bullets
            for(auto b : bullets) {
                if(b.x == x && b.y == y) {
                    cout << "|";
                    printed = true;
                }
            }

            // Enemies
            for(auto e : enemies) {
                if(e.x == x && e.y == y) {
                    cout << "V";
                    printed = true;
                }
            }

            if(!printed)
                cout << ".";
        }

        cout << endl;
    }

    cout << "\n🚀 Score: " << score;
    cout << "\nControls: A = Left | D = Right | SPACE = Shoot";
}

// ================= INPUT =================

void input() {

    if(_kbhit()) {

        char ch = _getch();

        if(ch == 'a' || ch == 'A') {
            if(playerX > 0)
                playerX--;
        }

        else if(ch == 'd' || ch == 'D') {
            if(playerX < WIDTH - 1)
                playerX++;
        }

        else if(ch == ' ') {

            Bullet b;

            b.x = playerX;
            b.y = HEIGHT - 2;

            bullets.push_back(b);
        }
    }
}

// ================= UPDATE BULLETS =================

void updateBullets() {

    for(int i = 0; i < bullets.size(); i++) {

        bullets[i].y--;

        if(bullets[i].y < 0) {
            bullets.erase(bullets.begin() + i);
        }
    }
}

// ================= SPAWN ENEMIES =================

void spawnEnemy() {

    Enemy e;

    e.x = rand() % WIDTH;
    e.y = 0;

    enemies.push_back(e);
}

// ================= MOVE ENEMIES =================

void moveEnemies() {

    for(int i = 0; i < enemies.size(); i++) {

        enemies[i].y++;

        // Enemy hits player
        if(enemies[i].y == HEIGHT - 1 &&
           enemies[i].x == playerX) {

            gameOver = true;
        }

        // Remove enemy if off screen
        if(enemies[i].y >= HEIGHT) {
            enemies.erase(enemies.begin() + i);
        }
    }
}

// ================= COLLISION =================

void collision() {

    for(int i = 0; i < bullets.size(); i++) {

        for(int j = 0; j < enemies.size(); j++) {

            if(bullets[i].x == enemies[j].x &&
               bullets[i].y == enemies[j].y) {

                bullets.erase(bullets.begin() + i);
                enemies.erase(enemies.begin() + j);

                score += 10;

                break;
            }
        }
    }
}

// ================= MAIN =================

int main() {

    srand(time(0));

    int frame = 0;

    while(!gameOver) {

        draw();

        input();

        updateBullets();

        if(frame % 10 == 0) {
            spawnEnemy();
        }

        moveEnemies();

        collision();

        Sleep(100);

        frame++;
    }

    system("cls");

    cout << "\n💀 GAME OVER";
    cout << "\nFinal Score: " << score << endl;

    return 0;
}
