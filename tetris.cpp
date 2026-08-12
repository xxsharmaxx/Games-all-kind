#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

const int M = 20; // Rows
const int N = 10; // Columns
int field[M][N] = {0};

struct Point { int x, y; } a[4], b[4];

int figures[7][4] = {
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

bool check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
        else if (field[a[i].y][a[i].x]) return 0;
    }
    return 1;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(320, 640), "Tetris GUI - C++ & SFML");
    window.setFramerateLimit(60);

    // Load textures/shapes using simple rectangles
    int blockSize = 32;

    int dx = 0;
    bool rotate = false;
    int colorNum = 1;
    float timer = 0, delay = 0.3f;
    sf::Clock clock;

    // Pick first figure
    int n = std::rand() % 7;
    for (int i = 0; i < 4; i++) {
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
    }

    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Up) rotate = true;
                else if (e.key.code == sf::Keyboard::Left) dx = -1;
                else if (e.key.code == sf::Keyboard::Right) dx = 1;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05f;

        // --- HORIZONTAL MOVEMENT ---
        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

        // --- ROTATION ---
        if (rotate) {
            Point p = a[1]; // center of rotation
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
        }

        // --- TICK / FALLING ---
        if (timer > delay) {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

            if (!check()) {
                for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + std::rand() % 7;
                int n = std::rand() % 7;
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
                // Check game over
                if (!check()) {
                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                            field[i][j] = 0;
                }
            }

            timer = 0;
        }

        // --- CHECK LINES ---
        int k = M - 1;
        for (int i = M - 1; i >= 0; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if (field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if (count < N) k--;
        }

        dx = 0; rotate = 0; delay = 0.3f;

        // --- RENDER ---
        window.clear(sf::Color::Black);

        // Draw Field Blocks
        sf::RectangleShape block(sf::Vector2f(static_cast<float>(blockSize - 1), static_cast<float>(blockSize - 1)));
        
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0) continue;
                block.setFillColor(sf::Color::Cyan);
                block.setPosition(static_cast<float>(j * blockSize), static_cast<float>(i * blockSize));
                window.draw(block);
            }
        }

        // Draw Active Figure
        for (int i = 0; i < 4; i++) {
            block.setFillColor(sf::Color(255, 165, 0)); // Orange
            block.setPosition(static_cast<float>(a[i].x * blockSize), static_cast<float>(a[i].y * blockSize));
            window.draw(block);
        }

        window.display();
    }

    return 0;
}
