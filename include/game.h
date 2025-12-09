#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <memory>

#include "ball.h"
#include "gameConstants.h"

#include <iostream>


class Game {
public:
    Game()
        : window_(sf::VideoMode(Constants::windowSize), "8 Ball Pool")
        , view_(window_.getDefaultView())
        , table_(Constants::tableSize)
        , pocket_(Constants::pocketRadius)
    {
        window_.setFramerateLimit(Constants::frameLimit);
        view_.setCenter(sf::Vector2f(0.f, 0.f));
        window_.setView(view_);

        if (!font_.openFromFile("arial.ttf")) {
            std::cout << "error loading font!!!\n";
            throw std::runtime_error("couldnt find font");
        }


        setupTable();
        setupBalls();
    }

    void run() {
        static sf::Clock clock;
        while (window_.isOpen()) {
            float dt = clock.restart().asSeconds();
            handleEvents();
            update(dt);
            render();
        }
    }

private:
    void handleEvents() {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            } else if (auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                
            } else if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    balls_.front().setVelocity(sf::Vector2f{650, 200});
                    std::cout << "Set velocity\n";
                }
            }
        }
    }

    void handleCushionCollision(Ball& ball) {
        auto& ballPos = ball.getPosition();
        auto& ballVel = ball.getVelocity();
        const float ballRadius = Constants::ballRadius;
        const auto& tableSize = Constants::tableSize;

        // if (ball.getType() != BallType::Cue) return;

        // std::cout << "Pos: " << ballPos.x << " " << ballPos.y
        // << " | Vel: " << ballVel.x << " " << ballVel.y
        // << " | Table: " << tableSize.x << " " << tableSize.y << "\n";

        if ((ballPos.x + ballRadius >= tableSize.x / 2.f && ballVel.x > 0)) {
            ballVel.x *= -1;
            ballPos.x = tableSize.x / 2.f - ballRadius;
        }

        if (ballPos.x - ballRadius <= -tableSize.x / 2.f && ballVel.x < 0) {
            ballVel.x *= -1;
            ballPos.x = -tableSize.x / 2.f + ballRadius;
        }

        if ((ballPos.y + ballRadius >= tableSize.y / 2.f && ballVel.y > 0)) {
            ballVel.y *= -1;
            ballPos.y = tableSize.y / 2.f - ballRadius;
        }

        if (ballPos.y - ballRadius <= -tableSize.y / 2.f && ballVel.y < 0) {
            ballVel.y *= -1;
            ballPos.y = -tableSize.y / 2.f + ballRadius;
        }
    }

    void handlePocketDetection(Ball& ball) {

    }

    void resolveBallCollision(Ball& a, Ball& b) {
        auto& pos_a = a.getPosition();
        auto& pos_b = b.getPosition();

        auto& vel_a = a.getVelocity();
        auto& vel_b = b.getVelocity();

        // Vector from A to B
        sf::Vector2f delta = pos_b - pos_a;
        float dist2 = delta.lengthSquared();

        const float minDist = 2.f * Constants::ballRadius;
        const float minDist2 = minDist * minDist;

        // Not overlapping → no collision
        if (dist2 >= minDist2 || dist2 == 0.f) {
            return;
        }

        float dist = std::sqrt(dist2);
        sf::Vector2f normal = delta / dist;  // unit vector along line of centres

        // Relative velocity
        sf::Vector2f relVel = vel_b - vel_a;
        // float relAlongNormal = sf::dot(relVel, normal);
        float relAlongNormal = relVel.x * normal.x + relVel.y * normal.y;

        // If balls are moving apart, no collision response
        if (relAlongNormal > 0.f) {
            return;
        }

        // Coefficient of restitution (1 = perfectly elastic, <1 = a bit loss)
        const float e = 0.98f;

        float j = -(1.f + e) * relAlongNormal / 2.f;

        // Impulse vector
        sf::Vector2f impulse = j * normal;

        // Apply impulse to velocities
        vel_a -= impulse;
        vel_b += impulse;

        // Positional correction to eliminate overlap (to prevent sinking/jitter)
        float overlap = minDist - dist;

        // Move proportional to inverse mass (lighter moves more)
        sf::Vector2f correctionA = -normal * (overlap / 2);
        sf::Vector2f correctionB =  normal * (overlap / 2);

        pos_a += correctionA;
        pos_b += correctionB;
    }

    void update(float dt) {
        for (auto& ball : balls_) {
            ball.update(dt);
        }

        for (auto& b : balls_) {
            handleCushionCollision(b);
        }

        for (int i = 0; i < balls_.size(); i++) {
            for (int j = i + 1; j < balls_.size(); j++) {
                resolveBallCollision(balls_[i], balls_[j]);
            }
        }

        for (auto& b : balls_) {
            handlePocketDetection(b);
        }
    }

    void render() {
        
        
        static std::array<sf::Vector2f, 6> pocketPositions {
            sf::Vector2f{-750.f, -375.f}, sf::Vector2f{0.f, -375.f}, sf::Vector2f{750.f, -375.f},
            sf::Vector2f{-750.f,  375.f}, sf::Vector2f{0.f,  375.f}, sf::Vector2f{750.f,  375.f},
        };

        window_.clear(sf::Color(50, 50, 50));

        window_.draw(table_);

        for (const auto& pos : pocketPositions) {
            pocket_.setPosition(pos);
            window_.draw(pocket_);
        }

        for (const auto& ball : balls_) {
            window_.draw(ball);
        }

        window_.display();
    }

    void setupTable() {
        table_.setFillColor(sf::Color(0, 100, 0));
        table_.setOutlineColor(sf::Color(139, 69, 19));
        table_.setOutlineThickness(Constants::tableOutlineThickness);
        table_.setOrigin(Constants::tableSize / 2.f);


        pocket_.setOrigin(sf::Vector2f{pocket_.getRadius(), pocket_.getRadius()});
        pocket_.setFillColor(sf::Color::Black);
    }

    void setupBalls() {
        balls_.reserve(16);

        // Add cue
        balls_.push_back(Ball{BallType::Cue, std::nullopt, sf::Vector2f{0.f, 0.f}, font_}); // dont need font??

        for (int i = 100; i < 500; i++) {
            balls_.push_back(Ball{BallType::Solid, i, sf::Vector2f{0.f, 0.f}, font_});
        }


        float ballRadius = Constants::ballRadius;
        float diameter = ballRadius * 2.f;
        
        // The vertical distance between rows in a hex packing
        // Formula: Radius * sqrt(3)
        float rowOffset = ballRadius * std::sqrt(3.f);

        // std::vector<sf::CircleShape> balls;
        
        
        // Define the start position (The Apex of the pyramid)
        // We place it on the right side of the table
        sf::Vector2f apexPos = table_.getPosition();
        apexPos.x += Constants::tableSize.x / 4.f; // Shift to the "foot spot" (right side)
        apexPos.y -= 0.f;               // Centered vertically

        int ballCount = 0; // To track which ball we are creating

        // Loop 5 Columns (Vertical Pyramid Rotated 90 degrees to point Left)
        // col 0 = 1 ball, col 1 = 2 balls, etc.
        for (int col = 0; col < 5; ++col) {
            for (int row = 0; row <= col; ++row) {

                sf::CircleShape ball(ballRadius);
                ball.setOrigin(sf::Vector2f{ballRadius, ballRadius}); // Center the ball origin


                // MATH FOR COORDINATES:
                // X grows as we add columns (moving right)
                float x = col * rowOffset;
                
                // Y is offset based on the row, centered around 0
                // (row - col / 2.0f) centers the row vertically
                float y = (row - col / 2.0f) * diameter;

                // Apply calculated offsets to the Apex position
                // Note: We subtract x to make the pyramid point LEFT (standard TV view)
                sf::Vector2f ballPosition{apexPos.x + x, apexPos.y + y};

                // COLOR LOGIC (Basic 8-Ball Rules)
                if (col == 2 && row == 1) {
                    ball.setFillColor(sf::Color::Black); // The 8-Ball (Center)
                    balls_.push_back(Ball{BallType::EightBall, 8, ballPosition, font_});
                } 
                else if (col == 4 && row == 0) {
                    //  ball.setFillColor(sf::Color::Yellow); // Corner 1 (Solid)
                    int ballNum = 0;
                    balls_.push_back(Ball{BallType::Solid, ballNum, ballPosition, font_});
                }
                else if (col == 4 && row == 4) {
                    // ball.setFillColor(sf::Color::Red);    // Corner 2 (Stripe)
                    int ballNum = 0;
                    balls_.push_back(Ball{BallType::Stripe, ballNum, ballPosition, font_});
                }
                else {
                    ball.setFillColor(sf::Color::Magenta);   // Rest are random
                    int ballNum = 0;
                    BallType type = BallType::Solid;
                    balls_.push_back(Ball{type, ballNum, ballPosition, font_});
                }

                ballCount++;
            }
        }
    }

    sf::RenderWindow window_;
    sf::View view_;

    // Rendering
    sf::RectangleShape table_;
    sf::CircleShape pocket_;

    sf::Font font_;

    std::vector<Ball> balls_;
};