#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

const int screenWidth = 800;
const int screenHeight = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Breakout GUI - C++ & SFML");
    window.setFramerateLimit(60);

    // Paddle
    sf::RectangleShape paddle(sf::Vector2f(100.f, 20.f));
    paddle.setFillColor(sf::Color::White);
    paddle.setPosition(screenWidth / 2.f - 50.f, screenHeight - 40.f);
    float paddleSpeed = 8.f;

    // Ball
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Yellow);
    ball.setPosition(screenWidth / 2.f - 10.f, screenHeight / 2.f - 10.f);
    sf::Vector2f ballVelocity(-4.f, -4.f);

    // Bricks Setup
    const int brCols = 8;
    const int brRows = 4;
    std::vector<sf::RectangleShape> bricks;

    float brWidth = 85.f;
    float brHeight = 25.f;
    float brSpacing = 10.f;
    float leftOffset = 45.f;
    float topOffset = 50.f;

    for (int i = 0; i < brRows; ++i) {
        for (int j = 0; j < brCols; ++j) {
            sf::RectangleShape brick(sf::Vector2f(brWidth, brHeight));
            brick.setFillColor(sf::Color::Cyan);
            brick.setPosition(leftOffset + j * (brWidth + brSpacing), topOffset + i * (brHeight + brSpacing));
            bricks.push_back(brick);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Paddle Movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 0.f) {
            paddle.move(-paddleSpeed, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x + paddle.getSize().x < screenWidth) {
            paddle.move(paddleSpeed, 0.f);
        }

        // Ball Movement
        ball.move(ballVelocity);

        // Wall Collisions (Left, Right, Top)
        if (ball.getPosition().x < 0.f || ball.getPosition().x + 20.f > screenWidth) {
            ballVelocity.x = -ballVelocity.x;
        }
        if (ball.getPosition().y < 0.f) {
            ballVelocity.y = -ballVelocity.y;
        }

        // Bottom Collision (Game Over reset ball)
        if (ball.getPosition().y > screenHeight) {
            ball.setPosition(screenWidth / 2.f - 10.f, screenHeight / 2.f - 10.f);
            ballVelocity = sf::Vector2f(-4.f, -4.f);
        }

        // Paddle Collision
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            ballVelocity.y = -ballVelocity.y;
            // Add slight angle adjustment based on hit location
            float hitFactor = (ball.getPosition().x + 10.f - (paddle.getPosition().x + 50.f)) / 50.f;
            ballVelocity.x = hitFactor * 5.f;
        }

        // Brick Collisions
        for (auto it = bricks.begin(); it != bricks.end();) {
            if (ball.getGlobalBounds().intersects(it->getGlobalBounds())) {
                ballVelocity.y = -ballVelocity.y;
                it = bricks.erase(it); // Remove destroyed brick
            } else {
                ++it;
            }
        }

        // Render
        window.clear(sf::Color::Black);

        window.draw(paddle);
        window.draw(ball);
        for (const auto& brick : bricks) {
            window.draw(brick);
        }

        window.display();
    }

    return 0;
}
