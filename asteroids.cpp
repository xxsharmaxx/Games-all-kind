#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

const int screenWidth = 800;
const int screenHeight = 600;
const float PI = 3.14159265f;

struct Bullet {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

struct Asteroid {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float radius;
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Asteroids GUI - C++ & SFML");
    window.setFramerateLimit(60);

    // Player Ship (Triangle)
    sf::ConvexShape ship(3);
    ship.setPoint(0, sf::Vector2f(0.f, -15.f));
    ship.setPoint(1, sf::Vector2f(-10.f, 15.f));
    ship.setPoint(2, sf::Vector2f(10.f, 15.f));
    ship.setFillColor(sf::Color::Transparent);
    ship.setOutlineColor(sf::Color::White);
    ship.setOutlineThickness(2.f);
    ship.setOrigin(0.f, 0.f);
    ship.setPosition(screenWidth / 2.f, screenHeight / 2.f);

    float shipAngle = 0.f;
    sf::Vector2f shipVelocity(0.f, 0.f);
    const float friction = 0.98f;

    std::vector<Bullet> bullets;
    std::vector<Asteroid> asteroids;

    // Spawn Initial Asteroids
    for (int i = 0; i < 5; ++i) {
        Asteroid ast;
        ast.radius = 30.f;
        ast.shape.setRadius(ast.radius);
        ast.shape.setFillColor(sf::Color::Transparent);
        ast.shape.setOutlineColor(sf::Color(200, 200, 200));
        ast.shape.setOutlineThickness(2.f);
        ast.shape.setOrigin(ast.radius, ast.radius);
        ast.shape.setPosition(static_cast<float>(std::rand() % screenWidth), static_cast<float>(std::rand() % screenHeight));
        
        float angle = static_cast<float>(std::rand() % 360) * PI / 180.f;
        float speed = 1.5f + static_cast<float>(std::rand() % 3);
        ast.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        asteroids.push_back(ast);
    }

    sf::Clock shootCooldown;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- CONTROLS ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            shipAngle -= 4.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            shipAngle += 4.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            float rad = (shipAngle - 90.f) * PI / 180.f;
            shipVelocity += sf::Vector2f(std::cos(rad) * 0.3f, std::sin(rad) * 0.3f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootCooldown.getElapsedTime().asMilliseconds() > 200) {
            float rad = (shipAngle - 90.f) * PI / 180.f;
            Bullet b;
            b.shape.setRadius(3.f);
            b.shape.setFillColor(sf::Color::White);
            b.shape.setOrigin(3.f, 3.f);
            b.shape.setPosition(ship.getPosition());
            b.velocity = sf::Vector2f(std::cos(rad) * 10.f, std::sin(rad) * 10.f);
            bullets.push_back(b);
            shootCooldown.restart();
        }

        // Apply friction & update ship position
        shipVelocity *= friction;
        ship.move(shipVelocity);
        ship.setRotation(shipAngle);

        // Screen wrapping for ship
        sf::Vector2f pos = ship.getPosition();
        if (pos.x < 0) pos.x = screenWidth; if (pos.x > screenWidth) pos.x = 0;
        if (pos.y < 0) pos.y = screenHeight; if (pos.y > screenHeight) pos.y = 0;
        ship.setPosition(pos);

        // Update Bullets
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->shape.move(it->velocity);
            sf::Vector2f bPos = it->shape.getPosition();
            if (bPos.x < 0 || bPos.x > screenWidth || bPos.y < 0 || bPos.y > screenHeight) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }

        // Update Asteroids
        for (auto& ast : asteroids) {
            ast.shape.move(ast.velocity);
            sf::Vector2f aPos = ast.shape.getPosition();
            if (aPos.x < -ast.radius) aPos.x = screenWidth + ast.radius;
            if (aPos.x > screenWidth + ast.radius) aPos.x = -ast.radius;
            if (aPos.y < -ast.radius) aPos.y = screenHeight + ast.radius;
            if (aPos.y > screenHeight + ast.radius) aPos.y = -ast.radius;
            ast.shape.setPosition(aPos);
        }

        // Collision: Bullets vs Asteroids
        for (auto bIt = bullets.begin(); bIt != bullets.end();) {
            bool bulletRemoved = false;
            for (auto aIt = asteroids.begin(); aIt != asteroids.end();) {
                // Distance check collision
                float dist = std::hypot(bIt->shape.getPosition().x - aIt->shape.getPosition().x,
                                        bIt->shape.getPosition().y - aIt->shape.getPosition().y);
                if (dist < aIt->radius) {
                    aIt = asteroids.erase(aIt);
                    bulletRemoved = true;
                    break;
                } else {
                    ++aIt;
                }
            }
            if (bulletRemoved) {
                bIt = bullets.erase(bIt);
            } else {
                ++bIt;
            }
        }

        // --- RENDER ---
        window.clear(sf::Color::Black);

        window.draw(ship);

        for (const auto& b : bullets) {
            window.draw(b.shape);
        }

        for (const auto& ast : asteroids) {
            window.draw(ast.shape);
        }

        window.display();
    }

    return 0;
}
