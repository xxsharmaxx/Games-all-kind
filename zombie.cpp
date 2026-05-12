#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

int playerX = WIDTH / 2;
int playerY = HEIGHT / 2;

int health = 100;
int score = 0;

struct Zombie {
    int x;
    int y;
};

vector<Zombie> zombies;

void spawnZombie() {
    Zombie z;

    int side = rand() % 4;

    if(side == 0) {
        z.x = 0;
        z.y = rand() % HEIGHT;
    }
    else if(side == 1) {
        z.x = WIDTH - 1;
        z.y = rand() % HEIGHT;
    }
    else if(side == 2) {
        z.x = rand() % WIDTH;
        z.y = 0;
    }
    else {
        z.x = rand() % WIDTH;
        z.y = HEIGHT - 1;
    }

    zombies.push_back(z);
}

void draw() {
    system("cls");

    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {

            if(x == playerX && y == playerY) {
                cout << "P ";
            }
            else {
                bool zombieHere = false;

                for(auto z : zombies) {
                    if(z.x == x && z.y == y) {
                        cout << "Z ";
                        zombieHere = true;
                        break;
                    }
                }

                if(!zombieHere)
                    cout << ". ";
            }
        }
        cout << endl;
    }

    cout << "\nHealth: " << health;
    cout << "\nScore: " << score;
    cout << "\nControls: W A S D";
}

void moveZombies() {
    for(auto &z : zombies) {

        if(z.x < playerX) z.x++;
        else if(z.x > playerX) z.x--;

        if(z.y < playerY) z.y++;
        else if(z.y > playerY) z.y--;

        // Collision
        if(z.x == playerX && z.y == playerY) {
            health -= 10;
        }
    }
}

void shoot() {
    for(int i = 0; i < zombies.size(); i++) {

        int dx = abs(zombies[i].x - playerX);
        int dy = abs(zombies[i].y - playerY);

        if(dx <= 1 && dy <= 1) {
            zombies.erase(zombies.begin() + i);
            score += 10;
            break;
        }
    }
}

void input() {
    if(_kbhit()) {

        char ch = _getch();

        switch(ch) {
            case 'w':
                if(playerY > 0) playerY--;
                break;

            case 's':
                if(playerY < HEIGHT - 1) playerY++;
                break;

            case 'a':
                if(playerX > 0) playerX--;
                break;

            case 'd':
                if(playerX < WIDTH - 1) playerX++;
                break;

            case ' ':
                shoot();
                break;
        }
    }
}

int main() {
    srand(time(0));

    int frame = 0;

    while(health > 0) {

        draw();
        input();

        if(frame % 10 == 0) {
            spawnZombie();
        }

        moveZombies();

        Sleep(200);

        frame++;
    }

    system("cls");

    cout << "\n💀 GAME OVER";
    cout << "\nFinal Score: " << score << endl;

    return 0;
}
